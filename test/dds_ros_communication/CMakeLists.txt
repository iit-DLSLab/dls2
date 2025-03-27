add_compile_options("-std=c++20")

add_executable(dds_ros_communication_main
	src/dds_ros_communication_main.cpp
	src/dds_ros_publisher.cpp
	src/dds_ros_subscriber.cpp
)

target_include_directories(dds_ros_communication_main
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(dds_ros_communication_main
	PRIVATE
		dls_messages
		fastdds
		fastcdr
)