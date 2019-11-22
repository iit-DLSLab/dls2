cmake_minimum_required(VERSION 2.8.3)
project(dls_hw_sim)

add_definitions("-std=c++17 -Werror")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  gazebo_ros
  controller_manager 
  dls2_hardware_interface
  transmission_interface
  pluginlib
  gazebo_ros
  gazebo_ros_control
  realtime_tools
  dls2_msgs
  urdf)

# Depend on system install of Gazebo 
find_package(gazebo REQUIRED)

catkin_package(
  LIBRARIES dls_hw_sim
  CATKIN_DEPENDS
    roscpp
    gazebo_ros
	gazebo_ros_control
    controller_manager
    pluginlib
    transmission_interface
	dls2_hardware_interface
  INCLUDE_DIRS include)

link_directories(${GAZEBO_LIBRARY_DIRS})
include_directories(include
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS})
  
## Libraries
add_library(dls_hw_sim src/dls_hw_sim.cpp)
target_link_libraries(dls_hw_sim ${catkin_LIBRARIES} 
                      ${GAZEBO_LIBRARIES}
                      )

install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

install(TARGETS dls_hw_sim
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  )

install(FILES dls_hw_sim_plugins.xml
DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

