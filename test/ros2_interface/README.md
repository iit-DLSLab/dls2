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
