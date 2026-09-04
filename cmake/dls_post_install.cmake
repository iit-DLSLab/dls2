if(NOT EXISTS "/usr/lib/dls2/python/bin/python3")
	execute_process(
		COMMAND
			/usr/bin/python3 -m venv --copies /usr/lib/dls2/python
	)
endif()

execute_process(
        COMMAND
            setcap cap_sys_nice=eip /usr/bin/dls2/dynamic_legged_systems_framework
        COMMAND
            setcap cap_sys_nice=eip /usr/bin/dls2/child_process_launcher
        COMMAND
            setcap cap_sys_nice=eip /usr/lib/dls2/python/bin/python3
        COMMAND
            ldconfig
	)

if ( ${COMPILE_XENOMAI_CHILD_PROCESS} )
    execute_process(
        COMMAND
            setcap cap_sys_nice=eip /usr/bin/dls2/child_process_launcher_xeno
)