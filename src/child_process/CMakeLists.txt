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
	dls_gaitgeneratorinterface
	dls_estimatorinterface

	#PRIVATE
	dls_time
	doglib
	${roscpp_LIBRARIES}
)

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
	dls_gaitgeneratorinterface
	dls_estimatorinterface

	#PRIVATE
	dls_time

	${roscpp_LIBRARIES}
)
