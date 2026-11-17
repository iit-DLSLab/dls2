cmake_minimum_required(VERSION 3.15)

set(PLUGIN_NAME dummy_py_node)

project(${PLUGIN_NAME})

add_compile_options("-std=c++2a")

find_package(Python3 REQUIRED COMPONENTS Interpreter)

execute_process(
    COMMAND
        ${Python3_EXECUTABLE} -c "import sysconfig; schemes = sysconfig.get_scheme_names(); scheme = 'deb_system' if 'deb_system' in schemes else sysconfig.get_default_scheme(); print(sysconfig.get_path('purelib', scheme=scheme))"
    OUTPUT_VARIABLE
        DUMMY_PY_NODE_DLS_PYTHON_ROOT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/version.txt PROJECT_VERSION)
string(REPLACE "." ";" VERSION_ARRAY ${PROJECT_VERSION})
list(POP_FRONT VERSION_ARRAY
    PROJECT_VERSION_MAJOR
    PROJECT_VERSION_MINOR
    PROJECT_VERSION_PATCH
)
string(REPLACE "\n" "" PROJECT_VERSION_PATCH ${PROJECT_VERSION_PATCH})
string(JOIN "." PROJECT_VERSION
    ${PROJECT_VERSION_MAJOR}
    ${PROJECT_VERSION_MINOR}
    ${PROJECT_VERSION_PATCH}
)
unset(VERSION_ARRAY)
message(STATUS "${PLUGIN_NAME} PROJECT VERSION ${PROJECT_VERSION}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(DUMMY_PY_NODE_SHARED_PYTHON /usr/lib/dls2/python/bin/python3)
set(DUMMY_PY_NODE_RUNTIME_ROOT /usr/share/${PLUGIN_NAME})

configure_file(
    cmake/dummy_py_node.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}
    @ONLY
)

install(
    FILES       plugin/config/scheduler.yaml
    DESTINATION /usr/include/dls2/schedulers/
    COMPONENT   ${PROJECT_NAME}
    RENAME      ${PLUGIN_NAME}.yaml
)

install(
    PROGRAMS    ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}
    DESTINATION /usr/bin
    COMPONENT   ${PROJECT_NAME}
)

install(
    FILES       python/dummy_py_node.py
    DESTINATION ${DUMMY_PY_NODE_RUNTIME_ROOT}/python
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    COMPONENT   ${PROJECT_NAME}
)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CPACK_PACKAGE_VERSION_MAJOR             ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR             ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH             ${PROJECT_VERSION_PATCH})
    set(CPACK_DEBIAN_PACKAGE_VERSION            ${PROJECT_VERSION})
    set(CPACK_PACKAGE_VENDOR                    "dlslab")
    set(CPACK_PACKAGE_CONTACT                   "dlslab")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER         "dlslab")
    set(CPACK_VERBATIM_VARIABLES                ON)
    set(CPACK_GENERATOR                         "DEB")
    set(CPACK_DEB_COMPONENT_INSTALL             ON)
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS          ON)

    string(TOUPPER ${PROJECT_NAME} COMPONENT_NAME)

    set(CPACK_DEBIAN_${COMPONENT_NAME}_PACKAGE_NAME          "dls-${PROJECT_NAME}" CACHE INTERNAL "")
    set(CPACK_DEBIAN_${COMPONENT_NAME}_FILE_NAME             "dls-${PROJECT_NAME}-${PROJECT_VERSION}.deb" CACHE INTERNAL "")
    set(CPACK_DEBIAN_${COMPONENT_NAME}_PACKAGE_DEPENDS       "dls2-runtime" CACHE INTERNAL "")

    set(CPACK_DEBIAN_${COMPONENT_NAME}_DEV_PACKAGE_NAME      "dls-${PROJECT_NAME}-dev" CACHE INTERNAL "")
    set(CPACK_DEBIAN_${COMPONENT_NAME}_DEV_FILE_NAME         "dls-${PROJECT_NAME}-dev-${PROJECT_VERSION}.deb" CACHE INTERNAL "")
    set(CPACK_DEBIAN_${COMPONENT_NAME}_DEV_PACKAGE_DEPENDS   "dls2-dev, dls-${PROJECT_NAME}" CACHE INTERNAL "")

    get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
    list(REMOVE_ITEM CPACK_COMPONENTS_ALL "Unspecified")

    include(CPack)
endif()
