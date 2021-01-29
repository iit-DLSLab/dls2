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
	dl
	dls_framework
	${roscpp_LIBRARIES}
)
target_include_directories(child_process_launcher
	PRIVATE
		src
)
dls_install(child_process_launcher)
