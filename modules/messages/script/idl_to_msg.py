#!/usr/bin/env python3
"""
idl_to_msg.py

Convert a subset of OMG IDL (.idl) commonly used for message structs into ROS2 .msg files.

Features:
- Default type mapping (customizable in-code)
- Input can be a directory (recursively finds *.idl) or a single .idl file.
- Output folder contains generated .msg files; IDL modules map to subfolders.

Supported IDL subset:
- module Foo { ... }
- struct Bar { <type> <name>; ... };
- Arrays:     <type> <name>[N];
- Sequences:  sequence<type> name; or sequence<type, N> name; (N ignored; ROS msg has no bounded arrays for sequences)
- Strings:    string name; or string<N> name; (bound ignored)
- Constant:   constanttype1 CONSTANTNAME1=constantvalue1
- Scoped names: module1::module2::Type
- Comments:   // comment (prev-line and inline comments support, with duplicates suppression)

Not supported (ignored or best-effort):
- enum, union, typedef, annotations, includes, #pragma, etc.

ROS2 .msg output:
- One .msg per struct
- Field lines: "<ros_type> <field_name>"
"""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional, Tuple, Dict


# -----------------------------
# Default IDL -> ROS2 type mapping
# -----------------------------
# ROS2 primitives: bool, byte, char, float32, float64, int8..int64, uint8..uint64, string, wstring
IDL_TO_ROS: Dict[str, str] = {
    "boolean": "bool",
    "bool": "bool",

    "octet": "uint8",
    "byte": "uint8",     # non-standard in IDL, but sometimes used informally

    "char": "int8",      # some ecosystems map char differently; adjust if needed
    "wchar": "uint16",

    "short": "int16",
    "unsigned short": "uint16",

    "long": "int32",              
    "unsigned long": "uint32",

    "long long": "int64",
    "unsigned long long": "uint64",

    "float": "float32",
    "double": "float64",

    "string": "string",
    "wstring": "wstring",
}


# -----------------------------
# Lexer
# -----------------------------
TOKEN_RE = re.compile(
    r"""
    (?P<WS>\s+)|
    (?P<COMMENT1>//[^\n]*)|
    (?P<COMMENT2>/\*.*?\*/)|

    (?P<SCOPE>::)|
    (?P<IDENT>[A-Za-z_]\w*)|
    (?P<NUMBER>\d+)|

    (?P<SYM>[{}();,<>\[\],=])
    """,
    re.VERBOSE | re.DOTALL,
)

KEYWORDS = {"module", "struct", "sequence", "string", "wstring", "unsigned", "long", "short", "float", "double", "char", "wchar", "boolean"}


@dataclass
class Token:
    kind: str
    value: str
    pos: int


def lex(text: str) -> List[Token]:
    tokens: List[Token] = []
    i = 0
    while i < len(text):
        m = TOKEN_RE.match(text, i)
        if not m:
            raise SyntaxError(f"Unexpected character at {i}: {text[i:i+20]!r}")
        kind = m.lastgroup or "?"
        val = m.group(kind)

        if kind == "WS":
            pass
        elif kind == "SYM":
            tokens.append(Token(val, val, i))
        else:
            tokens.append(Token(kind, val, i))

        i = m.end()
    return tokens


# -----------------------------
# Parser structures
# -----------------------------
@dataclass
class Field:
    idl_type: str # normalized IDL type string or scoped name
    name: str
    is_array: bool = False
    array_len: Optional[int] = None
    is_sequence: bool = False # sequence<T>
    # for ROS msg, sequences become dynamic arrays: "T[]"
    leading_comment: Optional[str] = None
    inline_comment: Optional[str] = None

@dataclass
class ConstDef:
    idl_type: str
    name: str
    value: str   # keep as text (e.g. "0", "28")

@dataclass
class StructDef:
    modules: List[str]
    name: str
    fields: List[Field]
    consts: List[ConstDef] 

class Parser:
    def __init__(self, tokens: List[Token], source_text: str):
        self.toks = tokens
        self.source_text = source_text
        self.i = 0
        self.pending_comment_lines: List[str] = []

    def _eat_comments(self) -> None:
        while self.i < len(self.toks):
            t = self.toks[self.i]
            if t.kind == "COMMENT1":
                self.pending_comment_lines.append(t.value[2:].strip())
                self.i += 1
                continue
            if t.kind == "COMMENT2":
                body = t.value[2:-2].strip()
                for line in body.splitlines():
                    line = line.strip().lstrip("*").strip()
                    if line:
                        self.pending_comment_lines.append(line)
                self.i += 1
                continue
            break

    def _take_pending_comment(self) -> Optional[str]:
        # remove empties
        lines = [x for x in (ln.strip() for ln in self.pending_comment_lines) if x]
        self.pending_comment_lines = []
        return "\n".join(lines) if lines else None

    def _consume_one_comment_as_lines(self) -> Optional[List[str]]:
        """Consume exactly one comment token at current index and return its lines; else None."""
        if self.i >= len(self.toks):
            return None
        t = self.toks[self.i]
        if t.kind == "COMMENT1":
            self.i += 1
            line = t.value[2:].strip()
            return [line] if line else []
        if t.kind == "COMMENT2":
            self.i += 1
            body = t.value[2:-2].strip()
            lines: List[str] = []
            for ln in body.splitlines():
                ln = ln.strip().lstrip("*").strip()
                if ln:
                    lines.append(ln)
            return lines
        return None
  
    def try_parse_const(self) -> Optional[ConstDef]:
        start_i = self.i
        try:
            t0 = self.peek()
            if not (t0 and t0.kind == "IDENT" and t0.value == "const"):
                return None
            self.pop()  # const

            ty, _ = self.parse_type()
            name = self.expect("IDENT").value
            self.expect("=")

            # allow number, ident, or a simple signed number like -1
            val_tok = self.pop()
            if val_tok.kind not in ("NUMBER", "IDENT"):
                raise SyntaxError(f"Expected const value at pos {val_tok.pos}")
            value = val_tok.value

            # optional: handle unary minus here

            t_semi = self.pop()
            if t_semi.value != ";":
                raise SyntaxError(f"Expected ; at pos {t_semi.pos}")

            return ConstDef(idl_type=ty, name=name, value=value)
        except Exception:
            self.i = start_i
            return None
    
    def collect_leading_comment(self) -> Optional[str]:
        """
        Consume consecutive comment tokens at the current position and return them as a single string.
        This is ONLY used right before parsing a field, so comments cannot 'float' away.
        """
        parts: List[str] = []
        while self.i < len(self.toks):
            t = self.toks[self.i]
            if t.kind == "COMMENT1":
                parts.append(t.value[2:].strip())
                self.i += 1
                continue
            if t.kind == "COMMENT2":
                body = t.value[2:-2].strip()
                for ln in body.splitlines():
                    ln = ln.strip().lstrip("*").strip()
                    if ln:
                        parts.append(ln)
                self.i += 1
                continue
            break

        parts = [p for p in (x.strip() for x in parts) if p]
        return "\n".join(parts) if parts else None
      
    def consume_one_comment_as_inline(self) -> str:
        """Consume exactly one comment token and return a single-line string."""
        if self.i >= len(self.toks):
            return ""
        t = self.toks[self.i]
        if t.kind == "COMMENT1":
            self.i += 1
            return t.value[2:].strip()
        if t.kind == "COMMENT2":
            self.i += 1
            body = t.value[2:-2].strip()
            # inline: squash to one line
            bits = []
            for ln in body.splitlines():
                ln = ln.strip().lstrip("*").strip()
                if ln:
                    bits.append(ln)
            return " ".join(bits).strip()
        return ""
  
    def peek(self) -> Optional[Token]:
        return self.toks[self.i] if self.i < len(self.toks) else None

    def pop(self) -> Token:
        if self.i >= len(self.toks):
            raise SyntaxError("Unexpected end of input")
        t = self.toks[self.i]
        self.i += 1
        return t

    def accept(self, val_or_kind: str) -> Optional[Token]:
        t = self.peek()
        if not t:
            return None
        if t.kind == val_or_kind or t.value == val_or_kind:
            return self.pop()
        return None

    def expect(self, val_or_kind: str) -> Token:
        t = self.accept(val_or_kind)
        if not t:
            p = self.peek().pos if self.peek() else -1
            raise SyntaxError(f"Expected {val_or_kind} at pos {p}")
        return t

    def parse(self) -> List[StructDef]:
        structs: List[StructDef] = []
        modules: List[str] = []
        const_stack: List[List[ConstDef]] = [[]]  # stack per module scope

        while self.peek():
            t = self.peek()
            if t.kind == "IDENT" and t.value == "module":
                self.pop()
                mod = self.expect("IDENT").value
                self.expect("{")
                modules.append(mod)

                const_stack.append([])  
                structs.extend(self.parse_block(modules, const_stack))
                self.accept(";")
                const_stack.pop()

                modules.pop()

            elif t.kind == "IDENT" and t.value == "struct":
                s = self.parse_struct(modules, consts=self._flatten_consts(const_stack))
                structs.append(s)

            else:
                # try const at top-level/module level too
                c = self.try_parse_const()
                if c:
                    const_stack[-1].append(c)
                else:
                    self.pop()

        return structs

    def _flatten_consts(self, const_stack: List[List[ConstDef]]) -> List[ConstDef]:
        out: List[ConstDef] = []
        for lvl in const_stack:
            out.extend(lvl)
        return out


    def parse_block(self, modules: List[str], const_stack: List[List[ConstDef]]) -> List[StructDef]:
        structs: List[StructDef] = []
        while self.peek() and self.peek().value != "}":
            t = self.peek()

            if t.kind == "IDENT" and t.value == "module":
                self.pop()
                mod = self.expect("IDENT").value
                self.expect("{")
                modules.append(mod)

                const_stack.append([])  
                structs.extend(self.parse_block(modules, const_stack))
                self.accept(";")
                const_stack.pop()

                modules.pop()

            elif t.kind == "IDENT" and t.value == "struct":
                s = self.parse_struct(modules, consts=self._flatten_consts(const_stack))
                structs.append(s)

            else:
                c = self.try_parse_const()
                if c:
                    const_stack[-1].append(c)
                else:
                    self.pop()

        self.expect("}")
        return structs

    def parse_struct(self, modules: List[str], consts: List[ConstDef]) -> StructDef:
        self.expect("IDENT")  # 'struct'
        name = self.expect("IDENT").value
        self.expect("{")
        fields: List[Field] = []
        while self.peek() and self.peek().value != "}":
            fld = self.try_parse_field()
            if fld:
                fields.append(fld)
                continue
            while self.peek() and self.peek().value not in (";", "}"):
                self.pop()
            self.accept(";")
        self.expect("}")
        self.expect(";")
        return StructDef(modules=list(modules), name=name, fields=fields, consts=list(consts))


    def try_parse_field(self) -> Optional[Field]:
        start_i = self.i
        try:
            leading_comment = self.collect_leading_comment()

            ty, is_seq = self.parse_type()
            name_tok = self.expect("IDENT")
            ros_name = ros_safe_field_name(name_tok.value)
            if ros_name != name_tok.value:
                print(f"[idl_to_msg] WARNING: Renamed field '{name_tok.value}' -> '{ros_name}'", file=sys.stderr)

            fld = Field(
                idl_type=ty,
                name=ros_name,
                is_sequence=is_seq,
                leading_comment=leading_comment,
            )

            if self.accept("["):
                n = int(self.expect("NUMBER").value)
                self.expect("]")
                fld.is_array = True
                fld.array_len = n

            # consume ';' without calling peek() (no side-effects)
            t_semi = self.pop()
            if t_semi.value != ";":
                raise SyntaxError(f"Expected ; at pos {t_semi.pos}")
            semi_pos = t_semi.pos

            # inline comment only if no newline between ';' and comment
            if self.i < len(self.toks) and self.toks[self.i].kind in ("COMMENT1", "COMMENT2"):
                comment_pos = self.toks[self.i].pos
                between = self.source_text[semi_pos + 1: comment_pos]
                is_inline = ("\n" not in between and "\r" not in between)
                if is_inline:
                    inline = self.consume_one_comment_as_inline()
                    if inline:
                        # dedup if inline repeats a leading line
                        leading_lines = set((fld.leading_comment or "").splitlines())
                        if inline not in leading_lines:
                            fld.inline_comment = inline

            return fld

        except Exception:
            self.i = start_i
            return None
      
    def parse_type(self) -> Tuple[str, bool]:
        t = self.peek()
        if not t:
            raise SyntaxError("Expected type, got EOF")

        if t.kind == "IDENT" and t.value == "sequence":
            self.pop()
            self.expect("<")
            inner, _ = self.parse_type()
            if self.accept(","):
                self.expect("NUMBER")  # ignore bound
            self.expect(">")
            return inner, True

        if t.kind == "IDENT" and t.value in ("string", "wstring"):
            base = self.pop().value
            if self.accept("<"):
                self.expect("NUMBER")
                self.expect(">")
            return base, False

        def parse_scoped_ident() -> str:
            segs = [self.expect("IDENT").value]
            while self.accept("SCOPE"):
                segs.append(self.expect("IDENT").value)
            return "::".join(segs)

        if t.kind == "IDENT" and t.value == "unsigned":
            parts = [self.pop().value]
            nxt = self.expect("IDENT").value
            parts.append(nxt)
            if nxt == "long":
                if self.peek() and self.peek().kind == "IDENT" and self.peek().value == "long":
                    parts.append(self.pop().value)
            return " ".join(parts), False

        if t.kind == "IDENT" and t.value == "long":
            parts = [self.pop().value]
            if self.peek() and self.peek().kind == "IDENT" and self.peek().value == "long":
                parts.append(self.pop().value)
            return " ".join(parts), False

        if t.kind == "IDENT":
            return parse_scoped_ident(), False

        raise SyntaxError(f"Expected type at pos {t.pos}")

# -----------------------------
# Conversion
# -----------------------------
def idl_type_to_ros(idl_type: str, current_modules: List[str]) -> str:
    """
    Strict conversion:
    - primitives via IDL_TO_ROS
    - numeric aliases (int32, uint64, ...)
    - scoped names A::B::Type -> A/Type (dropping 'msg')
    - otherwise: raise ValueError
    """
    idl_type_norm = " ".join(idl_type.split())

    if idl_type_norm in IDL_TO_ROS:
        return IDL_TO_ROS[idl_type_norm]

    alias_map = {
        "int8": "int8",
        "uint8": "uint8",
        "int16": "int16",
        "uint16": "uint16",
        "int32": "int32",
        "uint32": "uint32",
        "int64": "int64",
        "uint64": "uint64",
        "float32": "float32",
        "float64": "float64",
    }
    if idl_type_norm in alias_map:
        return alias_map[idl_type_norm]

    if "::" in idl_type_norm:
        parts = [p for p in idl_type_norm.split("::") if p != "msg"]
        return "/".join(parts)

    # Hard error, abort current file and go on
    raise ValueError(
        f"Unrecognized IDL type '{idl_type_norm}'. "
        f"Use a mapped primitive or a fully-scoped type (pkg::msg::Type) or add this type in IDL_TO_ROS mapping."
    )


def struct_to_msg_text(s: StructDef) -> str:
    lines: List[str] = []

    # constants first
    for c in s.consts:
        ros_ty = idl_type_to_ros(c.idl_type, s.modules)
        lines.append(f"{ros_ty} {c.name}={c.value}")
    if s.consts:
        lines.append("")

    # then fields (with leading + inline comments)
    for f in s.fields:
        if f.leading_comment:
            for ln in f.leading_comment.splitlines():
                ln = ln.strip()
                if ln:
                    lines.append(f"\n# {ln}")

        base_ros = idl_type_to_ros(f.idl_type, s.modules)
        ros_type = base_ros
        if f.is_sequence:
            ros_type = f"{base_ros}[]"
        if f.is_array:
            ros_type = f"{base_ros}[{f.array_len}]"

        if f.inline_comment:
            lines.append(f"{ros_type} {f.name}  # {f.inline_comment}")
        else:
            lines.append(f"{ros_type} {f.name}")

        # lines.append("")

    while lines and lines[-1] == "":
        lines.pop()

    return "\n".join(lines) + ("\n" if lines else "")



def output_path_for_struct(out_dir: Path, s: StructDef) -> Path:
    # Mirror modules into folders
    d = out_dir.joinpath(*s.modules) if s.modules else out_dir
    return d / f"{s.name}.msg"


def read_text(p: Path) -> str:
    return p.read_text(encoding="utf-8", errors="replace")

import re

def strip_preprocessor(text: str) -> str:
    """
    Remove preprocessor directives (include guards, #include, #ifdef...).
    Also removes trailing // comments to prevent edge-case lexer issues.
    """
    out_lines = []
    for line in text.splitlines():
        s = line.lstrip()
        if s.startswith("#"):
            continue
        out_lines.append(line)
    return "\n".join(out_lines)

def convert_idl_file(idl_path: Path, out_dir: Path) -> List[Path]:
    raw_text = read_text(idl_path)
    text = strip_preprocessor(raw_text)
    tokens = lex(text)
    parser = Parser(tokens, text)
    structs = parser.parse()

    written: List[Path] = []
    for s in structs:
        msg_text = struct_to_msg_text(s)
        out_path = output_path_for_struct(out_dir, s)
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(msg_text, encoding="utf-8")
        written.append(out_path)
    return written

def convert_idl_file_safe(idl_path: Path, out_dir: Path) -> List[Path]:
    try:
        return convert_idl_file(idl_path, out_dir)
    except Exception as e:
        print(
            f"[idl_to_msg] WARNING: skipping '{idl_path}' due to error:\n"
            f"  {type(e).__name__}: {e}",
            file=sys.stderr,
        )
        return []

def find_idl_files(input_dir: Path) -> List[Path]:
    return sorted([p for p in input_dir.rglob("*.idl") if p.is_file()])

import re

_ROS_FIELD_RE = re.compile(r'^(?!.*__)(?!.*_$)[a-z][a-z0-9_]*$')

def to_snake_case(name: str) -> str:
    # e.g. basePoseHF -> base_pose_hf
    s = re.sub(r'(.)([A-Z][a-z]+)', r'\1_\2', name)
    s = re.sub(r'([a-z0-9])([A-Z])', r'\1_\2', s)
    s = s.replace("-", "_")
    s = s.lower()
    # collapse multiple underscores and trim
    s = re.sub(r'__+', '_', s).strip('_')
    if not s:
        s = "field"
    # must start with a-z
    if not ('a' <= s[0] <= 'z'):
        s = "f_" + s
    # must end not underscore
    s = s.rstrip('_')
    # avoid double underscores again
    s = re.sub(r'__+', '_', s)
    return s

def ros_safe_field_name(name: str) -> str:
    out = to_snake_case(name)
    # last resort: ensure it matches regex
    if not _ROS_FIELD_RE.match(out):
        out = re.sub(r'[^a-z0-9_]', '_', out)
        out = re.sub(r'__+', '_', out).strip('_')
        if not out or not ('a' <= out[0] <= 'z'):
            out = "f_" + (out or "field")
        out = out.rstrip('_')
    return out

# -----------------------------
# CLI
# -----------------------------
def build_argparser() -> argparse.ArgumentParser:
    epilog = r"""
Examples:

  # Convert a directory of IDL files recursively into ./msg_out
  python3 idl_to_msg.py ./idl_definitions -o ./msg_out

  # Convert a single file
  python3 idl_to_msg.py ./idl_definitions/MyTypes.idl -o ./msg_out

Output layout:
  - Each IDL 'module X { ... }' becomes a subfolder 'X/' under the output directory.
  - Each 'struct Name { ... };' becomes 'Name.msg' in that folder.
  - Each output msg file is compliant with ROS requirements 
    (e.g. upper case in field names are forced to snake case lower case).

Type mapping:
  - A built-in mapping is used (see IDL_TO_ROS in the script).
"""
    return argparse.ArgumentParser(
        prog="idl_to_msg.py",
        description="Convert .idl structs into ROS2-compatible .msg files.",
        epilog=epilog,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )


def main(argv: List[str]) -> int:
    ap = build_argparser()
    ap.add_argument(
        "input",
        help="Path to a .idl file OR a directory containing .idl files (processed recursively).",
    )
    ap.add_argument(
        "-o", "--output-dir",
        default="msg_out",
        help="Output directory where .msg files will be written (default: ./msg_out).",
    )
    ap.add_argument(
        "-v", "--verbose",
        action="store_true",
        help="List successfully generated files.",
    )
    args = ap.parse_args(argv)

    inp = Path(args.input).expanduser().resolve()
    out_dir = Path(args.output_dir).expanduser().resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    if inp.is_file():
        if inp.suffix.lower() != ".idl":
            print(f"Error: input file must end with .idl: {inp}", file=sys.stderr)
            return 2
        written = convert_idl_file_safe(inp, out_dir)
        if not written:
            print(f"No structs found in {inp} (nothing generated).")
        else:
            if args.verbose:
                for p in written:
                    print(p)
        return 0

    if inp.is_dir():
        idl_files = find_idl_files(inp)
        if not idl_files:
            print(f"No .idl files found under: {inp}", file=sys.stderr)
            return 2
        all_written: List[Path] = []
        for f in idl_files:
            all_written.extend(convert_idl_file_safe(f, out_dir))
        if not all_written:
            print(f"No structs found in any .idl files under {inp} (nothing generated).")
        else:
            if args.verbose:
                for p in all_written:
                    print(p)
        return 0

    print(f"Error: input path does not exist: {inp}", file=sys.stderr)
    return 2


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
