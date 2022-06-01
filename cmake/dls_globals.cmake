message(STATUS "## Reading dls_globals.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compilation time
string(TIMESTAMP DLS_COMPILE_DATE "%d-%m-%Y")

# Length of the path to the route directory of the project
string(LENGTH "{CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)

add_compile_options(
	# Standards
	-std=c++2a

	# Warnings
	-Wall
	-Wextra
	-pedantic
	-g

	# Custom defintions
	-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}
)
