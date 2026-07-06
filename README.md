# DLS2

# Introduction
This project contains the main dls framework
# Documentation and installation
Please refer to the repo  [dls2-barebone](https://github.com/iit-DLSLab/dls2-barebone.git) for documentation, dependencies and installation of the DLS2 framework.

Python deps
=============================
They can be installed through dockerfile or in the container directly using the following command: 
```bash
PYTHON_EXECUTABLE=/usr/lib/dls2/python/bin/python3 dls2/scripts/install_py_runtime_deps.sh /path/to/requirements.txt
```
When `dls2-runtime` is built and installed, a private interpreter (`/usr/lib/dls2/python/bin/python3`) appears. That specific interpreter has specific permissions allowing for realtime capabilities. More precisely, `/usr/lib/dls2/python/bin/python3` is a relocated interpreter binary, with an isolated Python environment. If `/usr/lib/dls2/python/bin/python3` does not exist yet when the cited script is executed, the script itself bootstraps the DLS2 virtual environment from `/usr/bin/python3`, installs the requirements into that isolated environment, and after `dls2-runtime` is installed the same interpreter path is reused with realtime-capable permissions.

# Mantainers
This repository is mantained by [Marco Marchitto](https://github.com/MMarcus95) and [Michele Pestarino](https://github.com/mich-pest).