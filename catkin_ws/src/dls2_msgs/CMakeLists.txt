cmake_minimum_required(VERSION 2.8.3)
project(dls2_msgs)

find_package(catkin REQUIRED COMPONENTS
	roscpp
	rospy
	geometry_msgs
	std_msgs
	message_generation
	actionlib_msgs
)

add_message_files(
	FILES
	BlindState.msg
	JointState.msg
	Pose.msg
	Screw.msg
	SensorsCombined.msg
)

# add_service_files(
# 	FILES
# 	)

#  add_action_files(
# 	DIRECTORY action
# 	FILES
# 	)

include_directories(
	include
	${catkin_INCLUDE_DIRS}
)


generate_messages(
   DEPENDENCIES
   std_msgs
   geometry_msgs
   actionlib_msgs
   )

catkin_package(
INCLUDE_DIRS
#include
${catkin_INCLUDE_DIRS}
CATKIN_DEPENDS
message_runtime
)

include_directories(include ${catkin_INCLUDE_DIRS})

install(DIRECTORY include/${PROJECT_NAME}/ DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
