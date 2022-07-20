add_library("dls_simulator"
    SHARED
        src/simulator_base.cpp
)

add_library("dls_gazebo"
    SHARED
        src/ros_sim.cpp
)

set_property(TARGET dls_simulator PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET dls_gazebo PROPERTY POSITION_INDEPENDENT_CODE ON)

include_directories(
	PUBLIC
		include
)

target_link_libraries("dls_simulator"
	PUBLIC
        dls_command
        dls_framework
)

target_link_libraries("dls_gazebo"
	PUBLIC
        dls_simulator
        roscpp
)

dls_install("dls_gazebo")
dls_install("dls_simulator")