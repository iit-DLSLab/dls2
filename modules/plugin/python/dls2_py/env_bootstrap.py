from __future__ import annotations

import os
import sys
import sysconfig
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Iterable


@dataclass(frozen=True)
class BootstrapPaths:
    workspace_root: Path | None
    install_prefix: Path
    package_root: Path
    package_module_dir: Path
    entry_module_dir: Path | None
    scheduler_dir: Path
    safety_config: Path
    message_dir: Path
    plugin_python_dir: Path | None
    fastdds_python_site: Path | None
    fastdds_library_dirs: tuple[Path, ...]


def setup_environment(
    package_name: str,
    *,
    module_file: str | Path | None = None,
    required_message_modules: Iterable[str] = (),
) -> Any:
    paths = _prepare_environment(
        package_name,
        module_file=module_file,
        required_message_modules=required_message_modules,
    )

    from dls2_periodic_app_bindings import PeriodicApp as _PeriodicApp

    from .periodic_app_plugin import wrap_periodic_app_plugin

    class _BoundPeriodicApp(_PeriodicApp):
        def __init__(self) -> None:
            super().__init__(package_name)

    def _configure_environment() -> None:
        _prepare_environment(
            package_name,
            module_file=module_file,
            required_message_modules=required_message_modules,
        )

    periodic_app_plugin = wrap_periodic_app_plugin(
        _BoundPeriodicApp,
        configure_environment=_configure_environment,
    )
    periodic_app_plugin.bootstrap_paths = paths
    return periodic_app_plugin


def _prepare_environment(
    package_name: str,
    *,
    module_file: str | Path | None = None,
    required_message_modules: Iterable[str] = (),
) -> BootstrapPaths:
    required_message_modules = tuple(required_message_modules)
    paths = resolve_paths(
        package_name,
        module_file=module_file,
        required_message_modules=required_message_modules,
    )

    _prepend_sys_path(paths.package_module_dir)
    _prepend_sys_path(paths.message_dir)
    if paths.plugin_python_dir is not None:
        _prepend_sys_path(paths.plugin_python_dir)
    if paths.fastdds_python_site is not None:
        _prepend_sys_path(paths.fastdds_python_site)

    for library_dir in paths.fastdds_library_dirs:
        _prepend_env_path("LD_LIBRARY_PATH", library_dir)

    if paths.fastdds_python_site is not None:
        _prepend_env_path("PYTHONPATH", paths.fastdds_python_site)
    _prepend_env_path("PYTHONPATH", paths.message_dir)
    if paths.plugin_python_dir is not None:
        _prepend_env_path("PYTHONPATH", paths.plugin_python_dir)

    os.environ["DLS_SCHEDULER_PATH"] = str(paths.scheduler_dir)
    os.environ["DLS_SAFETY_LAYER_PATH"] = str(paths.safety_config)
    _validate_scheduler_config(package_name, paths.scheduler_dir)

    for module_name in required_message_modules:
        module_path = paths.message_dir / f"{module_name}.py"
        if not module_path.exists():
            raise FileNotFoundError(
                f"Missing generated message module {module_path}. "
                f"Set {package_name.upper()}_TYPES_PATH or rebuild the workspace."
            )

    return paths


def configure_runtime_configs(package_name: str, *, module_file: str | Path | None = None) -> BootstrapPaths:
    return _prepare_environment(package_name, module_file=module_file)


def resolve_paths(
    package_name: str,
    *,
    module_file: str | Path | None = None,
    required_message_modules: Iterable[str] = (),
) -> BootstrapPaths:
    package_module_dir = Path(__file__).resolve().parent
    entry_module_dir = Path(module_file).resolve().parent if module_file else None
    package_root = package_module_dir.parent

    workspace_root = _resolve_workspace_root(package_root)
    install_prefix = Path(os.environ.get("PEGASUS_INSTALL_PREFIX", "/usr/local")).resolve()

    scheduler_source_dir = package_root / "plugin" / "config"
    scheduler_source_file = scheduler_source_dir / "scheduler.yaml"
    scheduler_dir = _resolve_scheduler_dir(package_name, scheduler_source_dir, scheduler_source_file)

    safety_config = _resolve_safety_config(workspace_root, install_prefix)
    message_dir = _resolve_message_dir(
        package_name,
        package_module_dir,
        workspace_root,
        install_prefix,
        tuple(required_message_modules),
    )
    plugin_python_dir = _resolve_plugin_python_dir(workspace_root)
    fastdds_python_site, fastdds_library_dirs = _resolve_fastdds_paths(workspace_root)

    return BootstrapPaths(
        workspace_root=workspace_root,
        install_prefix=install_prefix,
        package_root=package_root,
        package_module_dir=package_module_dir,
        entry_module_dir=entry_module_dir,
        scheduler_dir=scheduler_dir,
        safety_config=safety_config,
        message_dir=message_dir,
        plugin_python_dir=plugin_python_dir,
        fastdds_python_site=fastdds_python_site,
        fastdds_library_dirs=fastdds_library_dirs,
    )


def _resolve_workspace_root(package_root: Path) -> Path | None:
    env_root = os.environ.get("PEGASUS_WORKSPACE_ROOT")
    if env_root:
        candidate = Path(env_root).resolve()
        if not candidate.exists():
            raise FileNotFoundError(
                f"PEGASUS_WORKSPACE_ROOT points to {candidate}, but that path does not exist."
            )
        return candidate

    for candidate in (package_root, *package_root.parents):
        if (candidate / "dls2-barebone").is_dir() and (candidate / "CMakeLists.txt").exists():
            return candidate
    return None


def _resolve_scheduler_dir(package_name: str, _source_dir: Path, source_file: Path) -> Path:
    configured_dir = Path(os.environ.get("DLS_SCHEDULER_PATH", "/usr/include/dls2/schedulers")).resolve()
    configured_file = configured_dir / f"{package_name}.yaml"
    if configured_file.exists():
        return configured_dir

    if source_file.exists():
        alias_dir = Path(tempfile.gettempdir()) / "pegasus_scheduler_configs" / package_name
        alias_dir.mkdir(parents=True, exist_ok=True)
        alias_path = alias_dir / f"{package_name}.yaml"
        if alias_path.exists() or alias_path.is_symlink():
            alias_path.unlink()
        alias_path.symlink_to(source_file)
        return alias_dir

    raise FileNotFoundError(
        f"Missing scheduler config for {package_name}. "
        f"Expected {configured_file} or {source_file}."
    )


def _validate_scheduler_config(package_name: str, scheduler_dir: Path) -> None:
    expected_config = scheduler_dir / f"{package_name}.yaml"
    if not expected_config.exists():
        raise FileNotFoundError(
            f"Missing DLS scheduler config {expected_config}. "
            "Install the package or set DLS_SCHEDULER_PATH to a directory "
            f"containing {package_name}.yaml."
        )


def _resolve_safety_config(workspace_root: Path | None, install_prefix: Path) -> Path:
    env_path = os.environ.get("DLS_SAFETY_LAYER_PATH")
    if env_path:
        candidate = Path(env_path).resolve()
        if candidate.exists():
            return candidate
        raise FileNotFoundError(
            f"DLS_SAFETY_LAYER_PATH points to {candidate}, but that file does not exist."
        )

    candidates: list[Path] = []
    if workspace_root is not None:
        candidates.append(
            workspace_root / "dls2-barebone" / "dls2" / "modules" / "supervisor" / "data" / "safety_layer.yaml"
        )
        candidates.append(
            workspace_root / "dls2" / "modules" / "supervisor" / "data" / "safety_layer.yaml"
        )
    candidates.append(install_prefix / "include" / "dls2" / "supervisor" / "data" / "safety_layer.yaml")
    candidates.append(Path("/usr/include/dls2/supervisor/data/safety_layer.yaml"))

    return _first_existing(
        candidates,
        "Missing safety layer config. Set DLS_SAFETY_LAYER_PATH or install dls2 supervisor data.",
    )


def _resolve_message_dir(
    package_name: str,
    package_module_dir: Path,
    workspace_root: Path | None,
    install_prefix: Path,
    required_message_modules: tuple[str, ...],
) -> Path:
    env_var = f"{package_name.upper()}_TYPES_PATH"
    env_path = os.environ.get(env_var)
    if env_path:
        candidate = Path(env_path).resolve()
        if candidate.exists():
            return candidate
        raise FileNotFoundError(f"{env_var} points to {candidate}, but that path does not exist.")

    candidates: list[Path] = []
    candidates.append(package_module_dir)
    if workspace_root is not None:
        candidates.append(workspace_root / "build" / "python_dist_packages" / package_name)
        candidates.append(
            workspace_root
            / "build"
            / "dls2-barebone"
            / "dls2"
            / "modules"
            / "messages"
            / "include"
            / "dls_messages"
            / "dds"
        )

    for site_dir in (
        sysconfig.get_path("purelib"),
        sysconfig.get_path("platlib"),
        install_prefix / "lib" / f"python{sys.version_info.major}.{sys.version_info.minor}" / "dist-packages",
        install_prefix / "lib" / f"python{sys.version_info.major}.{sys.version_info.minor}" / "site-packages",
        install_prefix / "lib" / "python",
        install_prefix / "lib" / "python3",
        Path("/usr/lib/python3/dist-packages"),
    ):
        if site_dir:
            site_path = Path(site_dir).resolve()
            candidates.append(site_path)
            candidates.append(site_path / "dls2")

    error_message = (
        f"Missing message bindings for {package_name}. "
        f"Set {env_var} or build/install the generated Python types."
    )
    if not required_message_modules:
        return _first_existing(candidates, error_message)

    for candidate in candidates:
        if candidate.exists() and all((candidate / f"{module_name}.py").exists() for module_name in required_message_modules):
            return candidate

    raise FileNotFoundError(error_message)


def _resolve_plugin_python_dir(workspace_root: Path | None) -> Path | None:
    if workspace_root is None:
        return None

    candidate = workspace_root / "dls2-barebone" / "dls2" / "modules" / "plugin" / "python"
    if candidate.exists():
        return candidate
    return None


def _resolve_fastdds_paths(workspace_root: Path | None) -> tuple[Path | None, tuple[Path, ...]]:
    env_root = os.environ.get("FASTDDS_PYTHON_INSTALL_ROOT")
    candidate_roots: list[Path] = []
    if env_root:
        candidate_roots.append(Path(env_root).resolve())
    if workspace_root is not None:
        candidate_roots.append(workspace_root / "build" / "fastdds_python_install")

    fastdds_python_site: Path | None = None
    library_dirs: list[Path] = []
    for root in candidate_roots:
        if not root.exists():
            continue
        for candidate in (
            root / "fastdds_python" / "lib" / f"python{sys.version_info.major}.{sys.version_info.minor}" / "site-packages",
            root / "fastdds_python" / "lib" / f"python{sys.version_info.major}.{sys.version_info.minor}" / "dist-packages",
            root / "lib" / f"python{sys.version_info.major}.{sys.version_info.minor}" / "site-packages",
            root / "lib" / f"python{sys.version_info.major}.{sys.version_info.minor}" / "dist-packages",
        ):
            if (candidate / "fastdds").exists():
                fastdds_python_site = candidate
                break

        for lib_dir in (root / "fastdds" / "lib", root / "fastcdr" / "lib"):
            if lib_dir.exists():
                library_dirs.append(lib_dir)

        if fastdds_python_site is not None or library_dirs:
            break

    return fastdds_python_site, tuple(library_dirs)


def _first_existing(candidates: Iterable[Path], error_message: str) -> Path:
    for candidate in candidates:
        if candidate.exists():
            return candidate
    raise FileNotFoundError(error_message)


def _prepend_env_path(var_name: str, value: Path) -> None:
    current = os.environ.get(var_name, "")
    parts = [part for part in current.split(":") if part]
    value_str = str(value)
    if value_str not in parts:
        parts.insert(0, value_str)
    os.environ[var_name] = ":".join(parts)


def _prepend_sys_path(path: Path) -> None:
    path_str = str(path)
    if path_str not in sys.path:
        sys.path.insert(0, path_str)
