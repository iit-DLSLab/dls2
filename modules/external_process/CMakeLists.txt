cmake_minimum_required(VERSION 3.15)

# Allow the process tests to build without the rest of DLS or ROS installed.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(dls_external_process LANGUAGES CXX)
    enable_testing()
endif()

find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Threads REQUIRED)

add_library(dls_external_process SHARED
	src/managed_external_process.cpp
)
target_compile_features(dls_external_process PUBLIC cxx_std_17)

target_include_directories(dls_external_process
	PUBLIC
		include
)

target_link_libraries(dls_external_process
	PUBLIC
		Boost::filesystem
		Boost::system
		Threads::Threads
)

if(COMMAND dls_install)
    dls_install(dls_external_process)
endif()

option(DLS_EXTERNAL_PROCESS_BUILD_TESTS "Build managed process integration tests" OFF)
if(DLS_EXTERNAL_PROCESS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
