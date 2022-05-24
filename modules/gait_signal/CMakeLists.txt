# TODO this might work better inside of the messaging library, it's just put
# here now for refactoring
add_library(dls_gait_signal
	src/gait_signal.cpp
)

set_property(TARGET dls_gait_signal PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(dls_gait_signal
	PUBLIC
		include
)

target_link_libraries(dls_gait_signal
	PUBLIC
		doglib
	PRIVATE
		dls_message_wrappers
		dls_geometry
)

dls_install(dls_gait_signal)
