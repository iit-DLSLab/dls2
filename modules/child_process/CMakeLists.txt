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

dls_install(child_process_launcher)
