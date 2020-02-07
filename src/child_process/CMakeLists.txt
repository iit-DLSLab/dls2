find_package(roscpp REQUIRED)
find_package(std_msgs REQUIRED)
include_directories(SYSTEM ${roscpp_INCLUDE_DIRS})

# ==============================================================================
# Child Processes
# ==============================================================================
configure_file(controller/controller_process.cpp.in
	${CMAKE_BINARY_DIR}/src/child_process/controller/controller_process.cpp
)

add_executable( dls_controller_process
	${CMAKE_BINARY_DIR}/src/child_process/controller/controller_process.cpp
)

target_link_libraries(dls_controller_process
	fastcdr
	fastrtps
	dl
	dls_framework
	dls_controller
	dls_gait_generator
	dls_estimator

	#PRIVATE
	dls_time
	doglib
	${roscpp_LIBRARIES}
)
dls_install(dls_controller_process)

configure_file(gait_generator/gait_generator_process.cpp.in
	${CMAKE_BINARY_DIR}/src/child_process/gait_generator_process.cpp
)

add_executable(dls_gait_generator_process
	${CMAKE_BINARY_DIR}/src/child_process/gait_generator_process.cpp
)

target_link_libraries(dls_gait_generator_process
	dls_framework
	fastcdr
	fastrtps
	dl
	dls_controller
	dls_gait_generator
	dls_estimator

	#PRIVATE
	dls_time

	${roscpp_LIBRARIES}
)
dls_install(dls_gait_generator_process)

# list(APPEND PROJECT_TARGETS dls_controller_process)
# list(APPEND PROJECT_TARGETS dls_gait_generator_process)
