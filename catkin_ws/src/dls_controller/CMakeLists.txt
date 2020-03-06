cmake_minimum_required(VERSION 2.8.3)
project(dls_controller)

find_package(catkin REQUIRED
  roscpp
  dwl
  terrain_server
  dwl_rviz_plugin
  dwl_msgs
  dls_msgs
  pluginlib
  dls_map_interface
  locomotion_utils
  iit_commons)

catkin_package(
  INCLUDE_DIRS include ${dwl_INCLUDE_DIRS} 
  LIBRARIES ${PROJECT_NAME} 
  CATKIN_DEPENDS roscpp dwl terrain_server dwl_rviz_plugin dwl_msgs dls_msgs pluginlib dls_map_interface locomotion_utils iit_commons
  DEPENDS
)

# Setting flags for optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(CMAKE_BUILD_TYPE "Release")

# Including the directories
set(INCLUDE_DIRS  ${PROJECT_SOURCE_DIR}/include
          ${catkin_INCLUDE_DIRS}
          ${dwl_INCLUDE_DIRS}
	  ${dls_msgs_INCLUDE_DIRS}
	  )
include_directories(${INCLUDE_DIRS})

## Declare a cpp library
add_library(${PROJECT_NAME}
            ${PROJECT_SOURCE_DIR}/src/Controller.cpp
            ${PROJECT_SOURCE_DIR}/src/support/ConsoleUtility.cpp
            ${PROJECT_SOURCE_DIR}/src/BaseState.cpp)
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} readline)

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  )

install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

