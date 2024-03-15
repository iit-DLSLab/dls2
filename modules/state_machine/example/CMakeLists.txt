add_compile_options("-std=c++2a")

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM SRC_FILES "main.cpp")

add_library(washing_machine
	SHARED
		${SRC_FILES}
)

set_property(TARGET washing_machine PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(washing_machine
	PUBLIC
		include
)

target_link_libraries(washing_machine
	PUBLIC
		state_machine
)

add_executable(washing_machine_test
	src/main.cpp
)
target_link_libraries(washing_machine_test
	PUBLIC
		washing_machine
)

dls_install(washing_machine)
dls_install(washing_machine_test)