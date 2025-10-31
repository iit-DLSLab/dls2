# list(APPEND CMAKE_MODULE_PATH  "${CMAKE_SOURCE_DIR}/modules/messages/cmake") # when compiling together with the dls2 project
# include(dls_message)		# message generation function

add_subdirectory(catch)
add_subdirectory(topic_info)
# add_subdirectory(messages)
add_subdirectory(rt)
# add_subdirectory(ros2_interface)
add_subdirectory(communication)