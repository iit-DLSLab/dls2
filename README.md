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

Release
--------------------------------------------------------------------------------

A release debian package can be generated using:

	$ mkdir build
	$ cd build
	$ cmake -DCMAKE_BUILD_TYPE=Release ..
	$ make
	$ cpack


# ROS2-DLS2 interface
Supported ROS2 middleware: **FastDDS**.

## Network 
To interface ROS2 with DLS2 and viceversa you have two ways:

### Fast way
- compile and install dls2-barebone
- configure the shell

	`source /usr/bin/dls2/scripts/setup_ros2_for_dls2.bash`

This scripts does what it explained in the [Long answare](#long-answare).

### Long answare
If you don't have dls2-barebone installed, do the following:
```bash	
export RMW_IMPLEMENTATION=rmw_fastrtps_cpp
export ROS_DISCOVERY_SERVER="127.0.0.1:11812;127.0.0.1:11813;127.0.0.1:11814;127.0.0.1:11818"
export ROS_SUPER_CLIENT=TRUE
ros2 daemon stop
ros2 daemon start
```
These commands:
- set FastDDS as ROS2 middleware
- set Discovery Server as discovery mechanism for ROS2, setting the ips and ports used by DLS2. Be aware that the order **MATTERS** and it has to follow the one you set in the servers.yaml configuration file (see [here](modules/ddscom/include/dls2/util/messaging/servers.yaml)).
- set the ROS client to SUPER_CLIENT. This is only needed for the ros2 CLI. You can set it to FALSE if you don't want to use the ros2 CLI.
- restart the ros2 daemon, which manages the cache of nodes, topics, and services. It makes ros2 CLI commands faster, but you need to stop and restart it to update it (for example, if you change the network, the discovery server, or the ROS_SUPER_CLIENT). Instead of stop and restart, you can use the ros2 CLI with the '--no-daemon' option.

### Topic convention
The DLS2 and ROS2 topic names must match to establish communication. By default, a ROS2 publisher/subscriber using FastDDS as middleware create FastDDS pub/sub prepended the namespace `rt`. For example a topic used in ros2 called `chatter` will became `rt/chatter` underneath. You then need to use `rt/chatter` as full topic name for your DLS2 publisher/subscriber. By default, the DLS2 ddsparticipant prepends `rt/` automatically. 


## Use ROS2 message in DLS2
Off-the-shelf ROS2 messages are supported by default (TBC).

To use custom ROS2 messages, do so you need to generate idl from .msg file:
1. Execute the command

	`rosidl translate --to idl -o . package_name message_name.msg`

	Be aware that this command can introduce comments with the `@verbatim` annotation that does not make you compile the message with FastDDSGen properly. Please remove manually in that case the comments.
2. generate fastdds message using the -typeros2 option (-cs is used to allow case-sentitive names):

	`fastddsgen -replace -cs -typeros2 message_name.idl`

	If you use `dls_add_message` in your CMakeLists for generating custom messages, this is automatically done.

## Use DLS2 message in ROS2
Use the [`idl_to_msg.py`](modules/messages/script/idl_to_msg.py) tool to convert an idl to a msg. E.g.

	`python3 idl_to_msg.py ../idls/BaseState.idl -o ./ `

You can now use the msg inside your ros2 node (TBC).