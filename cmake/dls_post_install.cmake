execute_process(
        COMMAND
            setcap cap_sys_nice=eip /usr/bin/dls2/dynamic_legged_systems_framework
        COMMAND
            setcap cap_sys_nice=eip /usr/bin/dls2/child_process_launcher
        COMMAND
            ldconfig
	)