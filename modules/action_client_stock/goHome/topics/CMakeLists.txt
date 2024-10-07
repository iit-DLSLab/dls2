# Add topics
add_library(${TOPICS_LIBRARY_NAME}
	SHARED
	src/topics.cpp
)
target_include_directories(${TOPICS_LIBRARY_NAME}
PUBLIC
    include
	${CMAKE_CURRENT_BINARY_DIR}/../messages/include
)
target_link_libraries(${TOPICS_LIBRARY_NAME}
	PUBLIC
	dls_messages
	#${MSGS_LIBRARY_NAME} 
	dls_topics
)
target_link_directories(${TOPICS_LIBRARY_NAME}
	PRIVATE
        /usr/lib/dls2
)
# ================================== Install =================================
install(
	DIRECTORY   include/
	DESTINATION ${DLS_DEFAULT_HEADER_DIR}
	COMPONENT   dls_dev
)

install(TARGETS ${TOPICS_LIBRARY_NAME}
	LIBRARY
		DESTINATION ${DLS_INSTALL_ACTION_DIR}/${ACTION_NAME}
		COMPONENT   dls_runtime
	ARCHIVE
		DESTINATION /usr/lib
		COMPONENT dls_runtime
	RUNTIME
		DESTINATION ${DLS_INSTALL_RUNTIME_DIR}
		COMPONENT dls_runtime
)
