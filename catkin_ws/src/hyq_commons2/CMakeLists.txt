# Project configuration
cmake_minimum_required(VERSION 2.8)
project(hyq_commons2)

SET(CMAKE_CXX_FLAGS "-std=c++11")

#set(HEADER_INSTALL_ROOT /usr/local/include/ CACHE PATH "Where to install the headers")
#set(LIB_INSTALL_ROOT /usr/local/lib/ CACHE PATH "Where to install the shared library")
find_package(PkgConfig)
pkg_check_modules(EIGEN3 eigen3 REQUIRED)

find_package(catkin REQUIRED iit_commons2)

set(BUILD_DOC false  CACHE BOOL "Build Doxygen documentation")

if(${BUILD_DOC})
    # check if Doxygen is installed
    find_package(Doxygen QUIET)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        # note the option ALL which allows to build the docs together with the application
        add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
    else (DOXYGEN_FOUND)
        message(WARNING "Doxygen need to be installed to generate the Doxygen documentation")
    endif (DOXYGEN_FOUND)
endif()

# Include directories
include_directories(include ${EIGEN3_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})

set(LIB_NAME iithyqcommons2)

catkin_package(
   INCLUDE_DIRS include
   CATKIN_DEPENDS iit_commons2
   LIBRARIES ${LIB_NAME}
   DEPENDS
)

# Source code folder
set(SOURCES # Common code
            src/hydraulics.cpp
            src/joints_pid_control.cpp
            src/cylinder_lever_arm.cpp
            src/internals.cpp
            src/inverse_kinematics.cpp
            src/feet_contact_forces.cpp
            src/robot_limits.cpp
            # Generated code
            src/transforms.cpp
            src/jacobians.cpp
            src/jsim.cpp
            src/inverse_dynamics.cpp
            src/forward_dynamics.cpp
            src/inertia_properties.cpp)


# Add the shared library
add_library(${LIB_NAME} SHARED ${SOURCES})
target_link_libraries(${LIB_NAME} ${catkin_LIBRARIES})

set(BUILD_TESTS false CACHE BOOL "Built tests")

if (CATKIN_ENABLE_TESTING)
    find_package(catkin REQUIRED COMPONENTS rostest)
    include_directories(include ${catkin_INCLUDE_DIRS})

    catkin_add_gtest(leverarm-test tests/leverarm.cpp src/cylinder_lever_arm.cpp)
    target_link_libraries(leverarm-test ${LIB_NAME} ${catkin_LIBRARIES})

    catkin_add_gtest(fk-test tests/fk.cpp )
    target_link_libraries(fk-test ${LIB_NAME} ${catkin_LIBRARIES})

    catkin_add_gtest(ik-test tests/ik.cpp )
    target_link_libraries(ik-test ${LIB_NAME} ${catkin_LIBRARIES})

    catkin_add_gtest(jac-test  tests/jacs.cpp )
    target_link_libraries(jac-test ${LIB_NAME} ${catkin_LIBRARIES})

    catkin_add_gtest(fcf-test tests/feet_contact_forces_test.cpp )
    target_link_libraries(fcf-test ${LIB_NAME} ${catkin_LIBRARIES})
endif()

install(DIRECTORY include/
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
  PATTERN ".svn" EXCLUDE
)

install(TARGETS ${LIB_NAME}
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
