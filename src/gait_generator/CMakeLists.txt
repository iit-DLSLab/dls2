add_library(
	dls_gait_generator
	SHARED
	gait_generator.cpp
	# TODO this should be moved to the messaging library
	gait_signal.cpp
)

# list(APPEND PROJECT_TARGETS dls_gait_generator)
dls_install(dls_gait_generator)

target_link_libraries(dls_gait_generator dls_geometry doglib)
add_library(
	dls_dummy_gait_generator
	SHARED
	dummy_gait_generator.cpp
)
add_dependencies(dls_dummy_gait_generator messagelib)
# list(APPEND PROJECT_TARGETS dls_dummy_gait_generator)
dls_install(dls_dummy_gait_generator)
target_link_libraries(dls_dummy_gait_generator
	doglib
)

add_library(
	dls_prep_gait_generator
	SHARED
	prep_gait_generator.cpp
)
add_dependencies(dls_prep_gait_generator messagelib doglib)
# list(APPEND PROJECT_TARGETS dls_prep_gait_generator)
dls_install(dls_prep_gait_generator)
