set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compilation time
string(TIMESTAMP DLS_COMPILE_DATE "%d-%m-%Y")

# Length of the path to the route directory of the project
string(LENGTH "{CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)

add_compile_options(
	# Standards
	-std=c++14

	# Warnings
	-Wall
	-Wextra
	-pedantic
	-g

	# Custom defintions
	-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}
	-include ${PROJECT_SOURCE_DIR}/include/dls2/util/debug/debug.hpp
)
