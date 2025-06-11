find_package(fastcdr 2 REQUIRED)
find_package(fastdds 3 REQUIRED)

# Create message library
add_library(fastdds_ros2_msgs SHARED)
dls_add_message(string_msg fastdds_ros2_msgs)

target_link_libraries(fastdds_ros2_msgs
PUBLIC
    robotlib
    dls_msg_wrapper
)

# create fastdds-ros2 reader/writer
set(READER dds_reader_for_ros2)
set(WRITER dds_writer_for_ros2)

add_executable(${READER}
    src/dds_reader.cpp
)
target_link_libraries(${READER}
    dls_com
    robotlib
    fastdds_ros2_msgs
)

add_executable(${WRITER}
    src/dds_writer.cpp
)
target_link_libraries(${WRITER}
    dls_com
    robotlib
    fastdds_ros2_msgs
)