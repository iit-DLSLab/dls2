#!/usr/bin/env bash
set -euo pipefail
shopt -s nullglob

src_dir="../idls" dst_dir="./ros_msgs" && \

mkdir -p "$dst_dir"

for f in "$src_dir"/*.idl; do
  out="$dst_dir/$(basename "$f")"

  awk '
    BEGIN {ifndef=0; define=0; endif_lines=0}
    # mark guard start (skip it)
    /^[[:space:]]*#ifndef[[:space:]]+/ && ifndef==0 {ifndef=1; next}
    # skip first #define after the first #ifndef
    ifndef==1 && /^[[:space:]]*#define[[:space:]]+/ && define==0 {define=1; next}
    # buffer all lines; we will drop the last #endif later
    {lines[++n]=$0}
    END {
      # find last line that is a plain #endif (guard close)
      last_endif=0
      for (i=n; i>=1; --i) {
        if (lines[i] ~ /^[[:space:]]*#endif([[:space:]]*.*)?$/) { last_endif=i; break }
      }
      for (i=1; i<=n; ++i) {
        if (i==last_endif && ifndef==1) continue
        print lines[i]
      }
    }
  ' "$f" > "$out"
done

echo "Files created at: $dst_dir (absolute: $(pwd)/$dst_dir)"