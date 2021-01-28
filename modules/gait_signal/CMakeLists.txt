# TODO this might work better inside of the messaging library, it's just put
# here now for refactoring
add_library(dls_gait_signal SHARED
	src/gait_signal.cpp
)

target_include_directories(dls_gait_signal PUBLIC include)

target_link_libraries(dls_gait_signal
	PUBLIC
		dls_geometry
		dls_robot
		dls_messaging
)
