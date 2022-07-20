add_library(dls_simulator
    SHARED
        src/simulator_base.cpp
)

set_property(TARGET dls_simulator PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_simulator
	PUBLIC
		include
	PRIVATE
		src
)

target_link_libraries(dls_simulator
        PUBLIC
                dls_command
                dls_framework
)

dls_install("dls_simulator")