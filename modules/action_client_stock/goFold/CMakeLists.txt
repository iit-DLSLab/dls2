# set configuration variables
set(ACTION_NAME goFold)
#set(MSGS_LIBRARY_NAME ${ACTION_NAME}_msgs)
#set(MSGS_WRAPPERS_LIBRARY_NAME ${ACTION_NAME}_msgs_wrappers)
set(TOPICS_LIBRARY_NAME ${ACTION_NAME}_topics)

add_subdirectory(core)
add_subdirectory(topics)