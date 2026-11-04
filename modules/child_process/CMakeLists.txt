dls_configure(src/child_process_launcher.cpp.in CHILD_PROCESS_LAUNCHER)

add_executable(child_process_launcher
	${CHILD_PROCESS_LAUNCHER}
)

target_include_directories(child_process_launcher
	PRIVATE
		src
)

target_link_libraries(child_process_launcher
	PUBLIC
		dl
		urdfdom_model
		urdfdom_model_state
		urdfdom_world
		urdfdom_sensor
		dls_framework
		fastcdr
		fastdds
		periodic_app_plugin
		app_plugin		
)

set (COMPILE_XENOMAI_CHILD_PROCESS OFF CACHE BOOL "Compile a child process executable with Xenomai support")
if ( ${COMPILE_XENOMAI_CHILD_PROCESS} )
    find_package(Xenomai REQUIRED)
    if(TARGET Xenomai::cobalt)
        set(USE_COBALT True)
    endif()
	add_executable(child_process_launcher_xeno src/child_process_launcher.cpp)
	target_link_libraries(child_process_launcher_xeno PRIVATE
		PUBLIC
			dls_framework
			fastcdr
			fastdds
			periodic_app_plugin
			app_plugin
		PRIVATE
		Xenomai::posix   # or the skin you actually use
	)

	if ( Xenomai_FOUND )
		xenomai_target_bootstrap(child_process_launcher_xeno)
	endif()
	dls_install(child_process_launcher_xeno)
endif()



dls_install(child_process_launcher)

