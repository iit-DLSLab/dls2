set(DLS_TEST_SCOPE command)

dls_register_test(command_manager
	SOURCE
		command_manager.cpp
	LINK
		dls_command
		fastcdr
		fastdds
)

dls_register_test(command
	SOURCE
		command.cpp
	LINK
		dls_command
		fastcdr
		fastdds
)
