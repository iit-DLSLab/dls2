cmake_minimum_required(VERSION 3.5.1)
project(dls2_to_roscontrol)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(catkin REQUIRED
	roscpp
	controller_interface
	hardware_interface
	)

catkin_package(
  INCLUDE_DIRS include 
 CATKIN_DEPENDS roscpp controller_interface hardware_interface
  DEPENDS
)

include_directories(${PROJECT_SOURCE_DIR}/include SYSTEM ${catkin_INCLUDE_DIRS})

add_library(${PROJECT_NAME} src/dls2_to_roscontrol.cpp)

target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  )

install(FILES dls_supervisor_plugin.xml
DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})  

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
  PATTERN ".svn" EXCLUDE)


if (CATKIN_ENABLE_TESTING)

endif()
