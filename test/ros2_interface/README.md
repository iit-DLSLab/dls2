# Run DLS2 example
- open dls2 docker image with `-ex` option
- build and install dls2_deploy
- attach new terminal to dls2 container, then run server

    `fastdds discovery -p 11811 --server-id 0 -l 127.0.0.1`
- run talker

    `../dls2/bin/Release/dds_writer_for_ros2`
- run listener

    `../dls2/bin/Release/dds_reader_for_ros2`

# Run ROS2 example
- Talker/listener [source](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html)
- open ROS2 docker image

    `docker rm -f ros2 && docker run -it --name ros2 --net=host -v ~/dls_ws_home:/root osrf/ros:jazzy-desktop`
- build example
    
    ```
    cd /root/dls2_deploy/dls2/test/ros2_interface/ros2_ws
    colcon build --packages-select dls2_interfaces
    source install/setup.bash
    colcon build
    ```
- source example

    `. install/setup.bash`
- attach new bash to docker container, and source example

    ```
    docker exec -it ros2 bash
    cd /root/dls2_deploy/dls2/test/ros2_interface/ros2_ws
    . install/setup.bash
    ```
- run talker in first terminal

    ```
    export ROS_DISCOVERY_SERVER=127.0.0.1:11811
    ros2 run cpp_pubsub talker
    ```
- run listener in the second terminal

    ```
    export ROS_DISCOVERY_SERVER=127.0.0.1:11811
    ros2 run cpp_pubsub listener
    ```

Remember that a server is specified by IP + port. A value of _--server-id_ different from _0_ is just used in case of redundat servers (see [here](https://docs.ros.org/en/jazzy/Tutorials/Advanced/Discovery-Server/Discovery-Server.html#server-redundancy)).
Please refere [here](../../modules/ddscom/README.md) for checking the _server\_id_ used by DLS2.

# ROS2-DLS2 interface
1. generate .idl from .msg file:
     rosidl translate --to idl -o . package_name message_name.msg
    
    - Another option is to create a ROS2 package just for the messages, and in this way you can use the .idl files directly. See [dls2_interfaces](ros2_ws/src/dls2_intefaces) package.
    - An additional option, in order to have full ros compatibility, is the `idl_to_msg.py` tool (e.g. `python3 idl_to_msg.py ../idls/BaseState.idl -o ./ `) 

2. generate fastdds message using the -typeros2 option (-cs is used to allow case-sentitive names): 
    fastddsgen -replace -cs -typeros2 message_name.idl

    if you use `dls_add_message` in your CMakeLists for generating custom messages, this is automatically done.
3. create a fastdds reader/writer connected to the ROS 2 topic name, for example `chatter`.
    NB: a reader/writer is configured to interface with ROS2. This means that the `rt/` DDS prefix is automatically added before the topic name.
4. Network configuration

    **short answer**
        
        `source /usr/bin/dls2/scripts/setup_ros2_for_dls2`
    
    **long answer**

    
    To interface your ROS2 node with DLS2 you need to set server ips and ports with

        export ROS_DISCOVERY_SERVER=<server_ip>:<server_port>

    Remember that if you want to use the ROS2 CLI (like ros2 topic list or similar) you need to export all discovery servers exposed by DLS2.
    Each discovery server gets assigned a unique integer id. This allows to automatically receive topics from multiple domains on ROS simply exporting semicolon separated ips. Be aware that restarting the ros daemon is needed as well.

    The following is an example of the whole procedure:
    ```bash
    ros2 daemon stop
    export ROS_DISCOVERY_SERVER="127.0.0.1:11812;127.0.0.1:11813;127.0.0.1:11814;127.0.0.1:11818"
    export ROS_SUPER_CLIENT=TRUE
    ros2 daemon start
    ```
    The ros2 daemon manages the cache of nodes, topics, and services. It makes CLI commands faster, but you need to stop and restart it to update it (for example, if you change the network, the discovery server, or the ROS_SUPER_CLIENT). Instead of stop and restart, you can use the '--no-daemon' option.
