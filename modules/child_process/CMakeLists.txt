find_package(roscpp REQUIRED)
find_package(std_msgs REQUIRED)
include_directories(SYSTEM ${roscpp_INCLUDE_DIRS})

# ==============================================================================
# Child Processes
# ==============================================================================
dls_configure(src/child_process_launcher.cpp.in CHILD_PROCESS_LAUNCHER)
add_executable(child_process_launcher
	${CHILD_PROCESS_LAUNCHER}
)
target_link_libraries(child_process_launcher
	PUBLIC
	fastcdr  # TODO link to project lib instead
	fastrtps # TODO link to project lib instead
	dl
	dls_framework
	dls_controller
	dls_gait_generator
	dls_estimator
	dls_class_loader

	#PRIVATE
	dls_path_prefixes
	dls_time
	doglib
	${roscpp_LIBRARIES}
)
target_include_directories(child_process_launcher
	PRIVATE
		src
)
dls_install(child_process_launcher)
