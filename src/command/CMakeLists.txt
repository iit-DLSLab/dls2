add_library(dls_command
	SHARED

	remote_command.cpp
	command_base.cpp
	command.cpp
)
dls_install(dls_command)
