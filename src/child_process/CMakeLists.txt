find_package(roscpp REQUIRED)
find_package(std_msgs REQUIRED)
include_directories(SYSTEM ${roscpp_INCLUDE_DIRS})

# ==============================================================================
# Child Processes
# ==============================================================================
dls_configure(child_process_launcher.cpp.in CHILD_PROCESS_LAUNCHER)
add_executable(child_process_launcher
	${CHILD_PROCESS_LAUNCHER}
)
target_link_libraries(child_process_launcher
	fastcdr
	fastrtps
	dl
	dls_framework
	dls_controller_interface
	dls_gait_generator
	dls_estimator

	#PRIVATE
	dls_time
	doglib
	${roscpp_LIBRARIES}
)
dls_install(child_process_launcher)
