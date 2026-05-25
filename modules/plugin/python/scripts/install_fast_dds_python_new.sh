
# Python-related installation (put in dockerfile)
sudo apt update
sudo apt install swig4.1 libpython3-dev libtinyxml2-10
pip3 install -U colcon-common-extensions vcstool --break-system-packages

mkdir -p ~/fastdds_python_3p5_ws/src
cd ~/fastdds_python_3p5_ws/src
git clone --branch v2.5.0 --depth 1 https://github.com/eProsima/Fast-DDS-python.git fastdds_python
cd ..
colcon build \
  --packages-select fastdds_python \
  --cmake-args \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH=/usr/local \
    -Dfastdds_DIR=/usr/local/lib/cmake/fastdds \
    -Dfastcdr_DIR=/usr/local/lib/cmake/fastcdr \
    -DCMAKE_INSTALL_RPATH=/usr/local/lib


source ~/fastdds_python_3p5_ws/install/setup.bash
export FASTDDS_PYTHON_INSTALL_ROOT=~/fastdds_python_3p5_ws/install
export PYTHONPATH=~/fastdds_python_3p5_ws/install/fastdds_python/lib/python3.12/site-packages:$PYTHONPATH
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

python3 - <<'PY'
import fastdds
print("fastdds import ok:", fastdds.__file__)
PY