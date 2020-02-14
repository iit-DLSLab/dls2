cmake_minimum_required(VERSION 3.5.1)
project(ros_control_to_dls2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(ROOT_PROJECT_DIRECTORY "${CMAKE_SOURCE_DIR}/../..")

find_package(catkin REQUIRED
	roscpp
	controller_interface
	hardware_interface
	dls2_hardware_interface
	dls2_msgs
	)

catkin_package(
  INCLUDE_DIRS include 
 CATKIN_DEPENDS roscpp controller_interface hardware_interface dls2_hardware_interface
  DEPENDS
)

include_directories(
	include/${PROJECT_NAME}
	${PROJECT_SOURCE_DIR}/include 
	${ROOT_PROJECT_DIRECTORY}/include
	${ROOT_PROJECT_DIRECTORY}/gen/include
	${ROOT_PROJECT_DIRECTORY}/build_tools/include
	SYSTEM
	${catkin_INCLUDE_DIRS}
	)
link_directories(BEFORE 
	${ROOT_PROJECT_DIRECTORY}/bin/debug/
	${ROOT_PROJECT_DIRECTORY}/bin/release/
	${ROOT_PROJECT_DIRECTORY}/bin/Debug/
	${ROOT_PROJECT_DIRECTORY}/bin/Release/
)
message(WARNING "linking to debug build of libraries")


add_library(${PROJECT_NAME}_joint_state_controller src/joint_state_controller.cpp)
add_library(${PROJECT_NAME}_blind_state_controller src/blind_state_controller.cpp)

target_link_libraries(${PROJECT_NAME}_joint_state_controller
	${catkin_LIBRARIES}
	dls_framework
	dls_messaging
	)

target_link_libraries(${PROJECT_NAME}_blind_state_controller
	${catkin_LIBRARIES}
	dls_framework
	dls_messaging
	fastrtps
	fastcdr
	)

install(TARGETS ${PROJECT_NAME}_joint_state_controller ${PROJECT_NAME}_blind_state_controller
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  )

install(FILES ros_control_to_dls2_plugin.xml DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})  

install(DIRECTORY launch/ DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch)

install(DIRECTORY config/ DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config)


if (CATKIN_ENABLE_TESTING)

endif()
