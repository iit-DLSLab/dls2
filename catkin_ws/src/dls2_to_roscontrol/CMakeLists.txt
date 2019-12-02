cmake_minimum_required(VERSION 3.5.1)
project(dls2_to_roscontrol)

message(WARNING "using a hack to point to root directory")
# This is a hack until the catkin_ws can be properly integrated with the rest of
# the project
set(ROOT_PROJECT_DIRECTORY "${CMAKE_SOURCE_DIR}/../..")
include_directories(
	${ROOT_PROJECT_DIRECTORY}/include
	${ROOT_PROJECT_DIRECTORY}/build_tools/include
)
include_directories(
	SYSTEM
	${ROOT_PROJECT_DIRECTORY}/gen/include
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(catkin REQUIRED
	roscpp
	controller_interface
	hardware_interface
	dls2_hardware_interface
	)

catkin_package(
  INCLUDE_DIRS include 
 CATKIN_DEPENDS roscpp controller_interface hardware_interface dls2_hardware_interface
  DEPENDS
)

include_directories(${PROJECT_SOURCE_DIR}/include SYSTEM ${catkin_INCLUDE_DIRS} ${ROOT_PROJECT_DIRECTORY}/gen/include)
link_directories(BEFORE ${ROOT_PROJECT_DIRECTORY}/bin/debug)

add_library(${PROJECT_NAME} src/dls2_to_roscontrol.cpp)

target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} fastrtps fastcdr dls_messaging) #dynamic_legged_systems_framework 

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  )

install(FILES dls2_to_roscontrol_plugin.xml DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})  

install(DIRECTORY launch/ DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch)

install(DIRECTORY config/ DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config)


if (CATKIN_ENABLE_TESTING)

endif()
