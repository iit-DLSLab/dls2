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

First, setup the git hooks:

	$ ./setup.sh

Build the debug project using:

	$ mkdir Debug
	$ cd Debug
	$ cmake ..
	$ make

Tests can be performed with:

	$ make tests
	$ make test

Documentation can be generated with:

	$ make doc

A convenience run target is available:

	$ make run

Release
--------------------------------------------------------------------------------

A release debian package can be generated using:

	$ mkdir Release
	$ cd Release
	$ cmake -DCMAKE_BUILD_TYPE=Release ..
	$ make
	$ cpack
