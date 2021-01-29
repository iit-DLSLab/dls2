add_library(dls_gait_generator
	SHARED
	src/gait_generator.cpp
)
target_include_directories(dls_gait_generator PUBLIC include)
dls_install(dls_gait_generator)

target_link_libraries(dls_gait_generator
	dls_framework_components
	dls_gait_signal
)
