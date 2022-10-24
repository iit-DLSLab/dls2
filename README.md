```
                                                        ,----,
                                                      .'   .' \ 
                                                    ,----,'    |
                ________  ___       ________        |    :  .  ;
               |\   ___ \|\  \     |\   ____\       ;    |.'  /
               \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;
                \ \  \ \\ \ \  \    \ \_____  \       /  ;  /
                 \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,
                  \ \_______\ \_______\____\_\  \   /  /  /.`|
                   \|_______|\|_______|\_________\./__;      :
                                      \|_________||   :    .'
                                                  ;   | .'
                                                  `---'

```

Introduction
================================================================================

This project contains the main dls framework

Building
================================================================================

Development
--------------------------------------------------------------------------------
To develop please use docker it will save a lot of time. So, if you have not setup your docker installation yet you are invited to visit the [dls_docker](https://gitlab.advr.iit.it/dls-lab/dls_docker) repository, there you will find everithing to get it working.

To enter in the fabulous world of DLS2 you can use the server-harbor:80/dls2/dls2-dev:22.04 image and follow the commands below.

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

Release
--------------------------------------------------------------------------------

A release debian package can be generated using:

	$ mkdir build
	$ cd build
	$ cmake -DCMAKE_BUILD_TYPE=Release ..
	$ make
	$ cpack
