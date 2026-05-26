#!/bin/bash

if [[ "${BASH_SOURCE[0]}" != "$0" ]]; then
    echo "Run this script, do not source it: ${BASH_SOURCE[0]}" >&2
    return 1
fi

set -euo pipefail

WORKSPACE_ROOT="${HOME}/fastdds_python_3p5_ws"
FASTDDS_PYTHON_TAG="v2.5.0"
PYTHON_SITE_PACKAGES="${WORKSPACE_ROOT}/install/fastdds_python/lib/python3.12/site-packages"
FASTDDS_PYTHON_INSTALL_ROOT="${WORKSPACE_ROOT}/install"

sudo apt update
sudo apt install -y swig4.1 libpython3-dev libtinyxml2-10
pip3 install -U colcon-common-extensions vcstool --break-system-packages

mkdir -p "${WORKSPACE_ROOT}/src"
cd "${WORKSPACE_ROOT}/src"

if [ ! -d fastdds_python/.git ]; then
    git clone --branch "${FASTDDS_PYTHON_TAG}" --depth 1 \
        https://github.com/eProsima/Fast-DDS-python.git fastdds_python
fi

cd "${WORKSPACE_ROOT}"
colcon build \
  --packages-select fastdds_python \
  --cmake-args \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH=/usr/local \
    -Dfastdds_DIR=/usr/local/lib/cmake/fastdds \
    -Dfastcdr_DIR=/usr/local/lib/cmake/fastcdr \
    -DCMAKE_INSTALL_RPATH=/usr/local/lib

FASTDDS_PYTHON_INSTALL_ROOT="${FASTDDS_PYTHON_INSTALL_ROOT}" \
PYTHONPATH="${PYTHON_SITE_PACKAGES}:${PYTHONPATH:-}" \
LD_LIBRARY_PATH="/usr/local/lib:${LD_LIBRARY_PATH:-}" \
python3 - <<'PY'
import fastdds
print("fastdds import ok:", fastdds.__file__)
PY

export FASTDDS_PYTHON_INSTALL_ROOT=${FASTDDS_PYTHON_INSTALL_ROOT}
export PYTHONPATH=${PYTHON_SITE_PACKAGES}:\$PYTHONPATH
export LD_LIBRARY_PATH=/usr/local/lib:\$LD_LIBRARY_PATH

cat <<EOF
For runtime, use:
  source dls2-barebone/dls2/modules/plugin/python/scripts/setup.sh
EOF
