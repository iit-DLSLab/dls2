add_library(dls_command SHARED
	src/remote_command.cpp
	src/command_base.cpp
	src/command.cpp
)

target_include_directories(dls_command
	PUBLIC
		include
)

target_link_libraries(dls_command
	PUBLIC
		dls_messaging
		dls_topics
)

add_dependencies(dls_command
	dls_dls2_messaging
)

dls_install(dls_command)
