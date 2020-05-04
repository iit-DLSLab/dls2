add_library(dls_command
	SHARED

	remote_command.cpp
	command_base.cpp
	command.cpp
)

target_link_libraries(dls_command
	dls_messaging
	dls_${PROJECT_NAME}_messaging
)
dls_install(dls_command)

add_dependencies(dls_command dls_dls2_messaging)
