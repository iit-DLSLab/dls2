add_library(dls_gait_generator
	src/gait_generator.cpp
)

set_property(TARGET dls_gait_generator PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_gait_generator
	PUBLIC
		include
)


target_link_libraries(dls_gait_generator
	PUBLIC
		dls_framework_components
		dls_gait_signal
)

dls_install(dls_gait_generator)
