find_package(fastcdr 2 REQUIRED)
find_package(fastdds 3 REQUIRED)

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
    dls_messages
)

add_executable(${WRITER}
    src/dds_writer_marker.cpp
)
target_link_libraries(${WRITER}
    dls_com
    robotlib
    dls_messages
)

add_executable(${IMAGE_WRITER}
    src/dds_writer.cpp
)
target_link_libraries(${IMAGE_WRITER}
    dls_com
    robotlib
    dls_messages
)

add_executable(${JOINT_STATE_WRITER}
    src/dds_writer_joint_state.cpp
)
target_link_libraries(${JOINT_STATE_WRITER}
    dls_com
    robotlib
    dls_messages
)
