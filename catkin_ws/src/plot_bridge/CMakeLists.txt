cmake_minimum_required(VERSION 3.0.2)
project(plot_bridge)

add_compile_options(-std=c++11)

set(ROOT_PROJECT_DIRECTORY "${CMAKE_SOURCE_DIR}/../..")
set(ROOT_DEPPLOY_PROJECT_DIRECTORY "${ROOT_PROJECT_DIRECTORY}/..")

include_directories(
	include/${PROJECT_NAME}
	${PROJECT_SOURCE_DIR}/include
	${ROOT_PROJECT_DIRECTORY}/include
	${ROOT_PROJECT_DIRECTORY}/build/msg/gen/include
	${ROOT_PROJECT_DIRECTORY}/gen/include
	${ROOT_PROJECT_DIRECTORY}/build_tools/include
	SYSTEM
	${catkin_INCLUDE_DIRS}
	${ROOT_DEPPLOY_PROJECT_DIRECTORY}/build/dls2/msg/gen/include
	)
link_directories(BEFORE
	${ROOT_PROJECT_DIRECTORY}/bin/
	${ROOT_PROJECT_DIRECTORY}/bin/debug/
	${ROOT_PROJECT_DIRECTORY}/bin/release/
	${ROOT_PROJECT_DIRECTORY}/bin/Debug/
	${ROOT_PROJECT_DIRECTORY}/bin/Release/
)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
)

# Generate messages in the 'msg' folder
 add_message_files(
   FILES
   joint_state.msg
   total_desired_torque.msg
 )

# Generate added messages and services with any dependencies listed here
generate_messages(
	DEPENDENCIES
	std_msgs  # Or other packages containing msgs
)

catkin_package(
  INCLUDE_DIRS include
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

 add_executable(plot_bridge src/plot_bridge.cpp)
 target_link_libraries(plot_bridge 
	 ${catkin_LIBRARIES}
		dls_framework
		dls_messaging
		dls_command
		dls_logging
		fastrtps
		fastcdr
		dls_dls2_messaging
		)
