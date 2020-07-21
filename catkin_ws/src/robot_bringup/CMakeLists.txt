cmake_minimum_required(VERSION 2.8.3)
project(robot_bringup)

find_package(catkin REQUIRED COMPONENTS
	dls2_to_roscontrol
	ros_control_to_dls2
	dls_hw_sim
)

catkin_package()

install(DIRECTORY launch/ DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch)
install(DIRECTORY config/ DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config)
