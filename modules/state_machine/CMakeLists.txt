add_compile_options("-std=c++2a")

add_library(state_machine
	SHARED
		src/state_machine.cpp
)
set_property(TARGET state_machine PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(state_machine
	PUBLIC
		include
)
target_link_libraries(state_machine
	dls_com
)

add_library(state_machine_watcher
	SHARED
		src/state_machine_watcher.cpp
)
set_property(TARGET state_machine_watcher PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(state_machine_watcher
	PUBLIC
		include
)
target_link_libraries(state_machine_watcher
	dls_com
	dls_utils
)

add_subdirectory(example)

dls_install(state_machine)
dls_install(state_machine_watcher)
