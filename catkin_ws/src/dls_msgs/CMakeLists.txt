cmake_minimum_required(VERSION 2.8.3)
project(dls_msgs)



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
    StringDoubleArray.msg
    HeightMap.msg
    HipHeightMap.msg
    DesiredFootholds.msg
    LegBoolMap.msg
    PoseTwistWrench.msg
    PoseTwistWrenchStamped.msg
    Point2D.msg
    )

add_service_files(
    FILES
    SetDesiredPose3D.srv
    SetDesiredPose4D.srv
    SetDesiredPose6D.srv
    SetDesiredPose6Dquat.srv
    SetDesiredPosition2D.srv
    SetDesiredPosition3D.srv
    SetDesiredTrajectoryFlag.srv
    GetDesiredTrajectoryFlag.srv
    )

 add_action_files(
     DIRECTORY action
     FILES
	FollowTrajectory.action
	GoToDesiredPose.action
	)

    include_directories(include ${catkin_INCLUDE_DIRS})
    

generate_messages(
   DEPENDENCIES
   std_msgs
   geometry_msgs
   actionlib_msgs
   )

catkin_package(
  INCLUDE_DIRS include ${catkin_INCLUDE_DIRS}
  #LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS message_runtime
  DEPENDS )

include_directories(include ${catkin_INCLUDE_DIRS})

install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
