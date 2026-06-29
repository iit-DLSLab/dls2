Introduction
================================================================================

This project contains the main dls framework

Building
================================================================================

Development
--------------------------------------------------------------------------------
To develop please use docker it will save a lot of time. So, if you have not setup your docker installation yet you are invited to visit the [dls_docker](https://gitlab.advr.iit.it/dls-lab/dls_docker) repository, there you will find everithing to get it working.

To enter in the fabulous world of DLS2 you can use the `ghcr.io/iit-dlslab/dls2-dev:latest` image and follow the commands below.

Build the debug project using:

	$ mkdir build
	$ cd build
	$ cmake ..
	$ make

Tests can be performed with:

	$ make tests
	$ make test

Documentation can be generated with:

	$ make doc

A convenience run target is available:

	$ make run

There is also a target to attach a debugger:

	$ make debug

Python deps
=============================
They can be installed through dockerfile or in the container directly using the following command: 
```bash
PYTHON_EXECUTABLE=/usr/lib/dls2/python/bin/python3 /path/to/dls_docker/scripts/install_py_runtime_deps.sh /path/to/requirements.txt
```
When `dls2-runtime` is built and installed, a private interpreter (`/usr/lib/dls2/python/bin/python3`) appears. That specific interpreter has specific permissions allowing for realtime capabilities. More precisely, `/usr/lib/dls2/python/bin/python3` is a relocated interpreter binary, with an isolated Python environment. If `/usr/lib/dls2/python/bin/python3` does not exist yet when the cited script is executed, the script itself bootstraps the DLS2 virtual environment from `/usr/bin/python3`, installs the requirements into that isolated environment, and after `dls2-runtime` is installed the same interpreter path is reused with realtime-capable permissions.

Release
--------------------------------------------------------------------------------

A release debian package can be generated using:

	$ mkdir build
	$ cd build
	$ cmake -DCMAKE_BUILD_TYPE=Release ..
	$ make
	$ cpack
