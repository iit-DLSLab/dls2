cmake_minimum_required(VERSION 3.5.1)
project(dls2 VERSION 0.0.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO remove references to MAJOR, MINOR and PATCH and use the long forms instead
set(MAJOR ${PROJECT_VERSION_MAJOR})
set(MINOR ${PROJECT_VERSION_MINOR})
set(PATCH ${PROJECT_VERSION_PATCH})

string(TIMESTAMP DLS_COMPILE_DATE "%d-%m-%Y")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(dls_options)
# include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dls_options.cmake)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/doc/dls.1.in
	${CMAKE_BINARY_DIR}/doc/dls.1
	@ONLY
)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/doc/dls.2.in
	${CMAKE_BINARY_DIR}/doc/dls.2
	@ONLY
)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/scripts/launch_framework.in
	${CMAKE_BINARY_DIR}/scripts/dls
)

# TODO add install command for this file
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/dls_install.cmake.in
	${CMAKE_BINARY_DIR}/cmake/dlsConfig.cmake
	@ONLY
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(BUILD_PREFIX "release")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(BUILD_PREFIX "debug")
else()
	message(STATUS "Build type supplied: ${CMAKE_BUILD_TYPE}")
	message(WARNING "Build type not supported. Setting to Debug")
	set(CMAKE_BUILD_TYPE "Debug")
	set(BUILD_PREFIX "debug")
endif()
message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

# IF(NOT $ENV{EIGEN3})
# 	MESSAGE(WARNING "EIGEN3 environment variable not set. Assuming /usr/include/eigen3")
# 	set(EIGEN_DIR /usr/include/eigen3)
# else()
# 	set(EIGEN_DIR $ENV{EIGEN3})
# ENDIF()
set(EIGEN_DIR /usr/include/eigen3)

string(LENGTH "{CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/build_tools/include
)
include_directories(
	SYSTEM
	${EIGEN_DIR}
	${CMAKE_SOURCE_DIR}/3rd_party/iit_commons/include
	${CMAKE_SOURCE_DIR}/3rd_party/doglib/include
)
include_directories(
	SYSTEM
	${CMAKE_SOURCE_DIR}/gen/include
)

add_subdirectory(3rd_party)
add_subdirectory(src)

# if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	enable_testing()
	add_subdirectory(test)
	add_subdirectory(doc)
# endif()

add_custom_target(
	todos
	COMMAND ${CMAKE_SOURCE_DIR}/build_tools/list_todos
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	include(InstallRequiredSystemLibraries)
	set(CPACK_GENERATOR "DEB")
	set(CPACK_PACKAGE_VENDOR "dlslab")
	set(CPACK_PACKAGE_CONTACT "dlslab")
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "dlslab")
	set(CPACK_PACKAGE_VERSION_MAJOR ${MAJOR})
	set(CPACK_PACKAGE_VERSION_MINOR ${MINOR})
	set(CPACK_PACKAGE_VERSION_PATCH ${PATCH})
	include(CPack)
# endif()

install(
	FILES ${CMAKE_BINARY_DIR}/scripts/dls
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
	DESTINATION /usr/bin
)

install(
	FILES ${CMAKE_BINARY_DIR}/cmake/dlsConfig.cmake
	DESTINATION lib/cmake/dls
)

install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/scripts/dls-completion.bash
	DESTINATION /etc/bash_completion.d
)

install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/scripts/dls-completion.zsh
	DESTINATION /usr/share/zsh/vendor-completions
	RENAME _dls
)

install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/scripts/xdg-open
	DESTINATION ${DLS_INSTALL_SCRIPTS_DIR}
	PERMISSIONS
		WORLD_EXECUTE WORLD_READ
		OWNER_READ OWNER_WRITE
)

if(DLS_INSTALL_DOCUMENTATION)

	install(
		DIRECTORY ${CMAKE_SOURCE_DIR}/doc/html
		DESTINATION ${DLS_INSTALL_DEVELOPMENT_DOCS_DIR}
	)
endif()

# Install manpages
add_custom_target(man ALL DEPENDS ${CMAKE_BINARY_DIR}/doc/dls.1.gz ${CMAKE_BINARY_DIR}/doc/dls.2.gz)
add_custom_command(
	OUTPUT
		${CMAKE_BINARY_DIR}/doc/dls.1.gz
		${CMAKE_BINARY_DIR}/doc/dls.2.gz
	COMMAND gzip ${CMAKE_BINARY_DIR}/doc/dls.1
	COMMAND gzip ${CMAKE_BINARY_DIR}/doc/dls.2
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
)
install(
	FILES ${CMAKE_BINARY_DIR}/doc/dls.1.gz
	DESTINATION /usr/man/man1
)
install(
	FILES ${CMAKE_BINARY_DIR}/doc/dls.2.gz
	DESTINATION /usr/man/man2
)
