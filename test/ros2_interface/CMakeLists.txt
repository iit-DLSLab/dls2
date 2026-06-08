find_package(fastcdr 2 REQUIRED)
find_package(fastdds 3 REQUIRED)

# Create message library
add_library(fastdds_ros2_msgs SHARED)
dls_add_message(string_msg_from_ROS2msg fastdds_ros2_msgs)
dls_add_message(RobotState fastdds_ros2_msgs)
dls_add_message(Test fastdds_ros2_msgs)
dls_add_message(Time fastdds_ros2_msgs)
dls_add_message(Duration fastdds_ros2_msgs)
dls_add_message(Header fastdds_ros2_msgs)
dls_add_message(Image fastdds_ros2_msgs)
dls_add_message(ColorRGBA fastdds_ros2_msgs)
dls_add_message(Point fastdds_ros2_msgs)
dls_add_message(Quaternion fastdds_ros2_msgs)
dls_add_message(Vector3 fastdds_ros2_msgs)
dls_add_message(Pose fastdds_ros2_msgs)
dls_add_message(Transform fastdds_ros2_msgs)
dls_add_message(TransformStamped fastdds_ros2_msgs)
dls_add_message(TFMessage fastdds_ros2_msgs)
dls_add_message(MeshFile fastdds_ros2_msgs)
dls_add_message(UVCoordinate fastdds_ros2_msgs)
dls_add_message(CompressedImage fastdds_ros2_msgs)
dls_add_message(Marker fastdds_ros2_msgs)
dls_add_message(JointState fastdds_ros2_msgs)
# dls_add_message(example_for_ros2 fastdds_ros2_msgs)

target_link_libraries(fastdds_ros2_msgs
PUBLIC
    robotlib
    dls_msg_wrapper
)

# create fastdds-ros2 reader/writer
set(READER dds_reader_for_ros2)
set(WRITER dds_writer_for_ros2)
set(IMAGE_WRITER dds_writer_image_for_ros2)
set(JOINT_STATE_WRITER dds_writer_joint_state_for_ros2)

add_executable(${READER}
    src/dds_reader.cpp
)
target_link_libraries(${READER}
    dls_com
    robotlib
    fastdds_ros2_msgs
)

add_executable(${WRITER}
    src/dds_writer_marker.cpp
)
target_link_libraries(${WRITER}
    dls_com
    robotlib
    fastdds_ros2_msgs
)

add_executable(${IMAGE_WRITER}
    src/dds_writer.cpp
)
target_link_libraries(${IMAGE_WRITER}
    dls_com
    robotlib
    fastdds_ros2_msgs
)

add_executable(${JOINT_STATE_WRITER}
    src/dds_writer_joint_state.cpp
)
target_link_libraries(${JOINT_STATE_WRITER}
    dls_com
    robotlib
    fastdds_ros2_msgs
)
