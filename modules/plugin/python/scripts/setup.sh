#!/bin/bash

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
PYTHON_MODULE_ROOT=$(cd -- "$SCRIPT_DIR/.." && pwd)
REPO_ROOT=$(cd -- "$SCRIPT_DIR/../../../../../.." && pwd)
MESSAGE_BUILD_DIR="$REPO_ROOT/build/dls2-barebone/dls2/modules/messages/include/dls_messages/dds"

DEFAULT_FASTDDS_PYTHON_ROOT="$HOME/fastdds_python_3p5_ws/install"

prepend_path() {
    local var_name=$1
    local new_path=$2

    if [ ! -d "$new_path" ]; then
        return
    fi

    local current_value="${!var_name:-}"
    if [ -z "$current_value" ]; then
        printf -v "$var_name" '%s' "$new_path"
    elif [[ ":$current_value:" != *":$new_path:"* ]]; then
        printf -v "$var_name" '%s:%s' "$new_path" "$current_value"
    fi
    export "$var_name"
}

find_fastdds_python_path() {
    local root=$1
    local candidate

    for candidate in \
        "$root"/fastdds_python/lib/python*/site-packages \
        "$root"/fastdds_python/lib/python*/dist-packages \
        "$root"/lib/python*/site-packages \
        "$root"/lib/python*/dist-packages; do
        if [ -d "$candidate/fastdds" ]; then
            printf '%s\n' "$candidate"
            return 0
        fi
    done

    return 1
}

if [ -z "${FASTDDS_PYTHON_INSTALL_ROOT:-}" ]; then
    if [ -f "$DEFAULT_FASTDDS_PYTHON_ROOT/setup.bash" ]; then
        FASTDDS_PYTHON_INSTALL_ROOT="$DEFAULT_FASTDDS_PYTHON_ROOT"
    else
        FASTDDS_PYTHON_INSTALL_ROOT=""
    fi
fi
export FASTDDS_PYTHON_INSTALL_ROOT

if [ -n "$FASTDDS_PYTHON_INSTALL_ROOT" ] && [ -f "$FASTDDS_PYTHON_INSTALL_ROOT/setup.bash" ]; then
    # shellcheck disable=SC1090
    export COLCON_TRACE="${COLCON_TRACE-}"
    export AMENT_TRACE_SETUP_FILES="${AMENT_TRACE_SETUP_FILES-}"
    source "$FASTDDS_PYTHON_INSTALL_ROOT/setup.bash"
fi

PYTHON_SITE_PACKAGES=$(python3 - <<'PY'
import sysconfig
print(sysconfig.get_path("purelib"))
PY
)

prepend_path PYTHONPATH "$PYTHON_MODULE_ROOT"
prepend_path PYTHONPATH "$MESSAGE_BUILD_DIR"
prepend_path PYTHONPATH "$PYTHON_SITE_PACKAGES"

if [ -n "$FASTDDS_PYTHON_INSTALL_ROOT" ]; then
    FASTDDS_PYTHON_SITE=""
    if FASTDDS_PYTHON_SITE=$(find_fastdds_python_path "$FASTDDS_PYTHON_INSTALL_ROOT"); then
        prepend_path PYTHONPATH "$FASTDDS_PYTHON_SITE"
    fi
    prepend_path LD_LIBRARY_PATH "$FASTDDS_PYTHON_INSTALL_ROOT/fastdds/lib"
    prepend_path LD_LIBRARY_PATH "$FASTDDS_PYTHON_INSTALL_ROOT/fastcdr/lib"
fi

prepend_path LD_LIBRARY_PATH /usr/local/lib
