cmake_minimum_required(VERSION 3.5.1)
project(dls2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include_directories(
	${CMAKE_SOURCE_DIR}/include
)

enable_testing()
# add_subdirectory(3rd_party)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)
