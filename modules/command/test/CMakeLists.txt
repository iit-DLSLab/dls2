set(DLS_TEST_SCOPE command)

dls_register_test(command_manager
	SOURCE
		command_manager.cpp
	LINK
		dls_command
)
