list(APPEND CMAKE_MODULE_PATH  "${DLS_MESSAGE_FOLDER}/cmake") # when compiling together with the dls2 project
include(dls_message)		# message generation function

add_subdirectory(catch)
add_subdirectory(topic_info)
# add_subdirectory(messages)
add_subdirectory(rt)
add_subdirectory(ros2_interface)
