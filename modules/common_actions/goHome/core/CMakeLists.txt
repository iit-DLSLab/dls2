# Create plugin library
add_library(${PROJECT_NAME} SHARED
	src/go_home.cpp
)
target_include_directories(${PROJECT_NAME}
	PUBLIC
		include
		# add other include directories here
)
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		dls_action_server_base
		dls_message_wrappers
		#${MSGS_WRAPPERS_LIBRARY_NAME}
		${TOPICS_LIBRARY_NAME}
		# add other linked libraries here
)
target_link_directories(${PROJECT_NAME}
	PRIVATE
        /usr/lib/dls2
		/usr/lib/dls2/actions/${PROJECT_NAME}
)

# ================================== Install =================================

install(
	DIRECTORY   include/
	DESTINATION /usr/include/dls2
	COMPONENT   ${PROJECT_NAME}_dev
)

install(
	TARGETS             ${PROJECT_NAME}
	LIBRARY DESTINATION /usr/lib/dls2/actions
	COMPONENT           ${PROJECT_NAME}
)