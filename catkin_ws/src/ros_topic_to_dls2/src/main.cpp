/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
// Ros subscription
#include <message_filters/subscriber.h>
// #include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>

// Fastrtps publishing
#include "util/messaging/publisher_base.hpp"
#include "msg/blind_statePubSubTypes.h"
#include "msg/timePubSubTypes.h"
#include "topics/low_level_estimation/blind_state.hpp"
#include "topics/simulation_time.hpp"

// cpp
#include <memory>

// project
#include "geometry/pose.hpp"

#include <dls2_msgs/BlindState.h>
#include <rosgraph_msgs/Clock.h>
// TODO temp
#include <std_msgs/Empty.h>
std::shared_ptr<ros::Publisher> pEmpty_pub;
using namespace dls;

#ifndef NDEBUG
#include "util/log/log.hpp"
#include <sstream>
#endif

// needs to be a pointer, else it crashes at launch
std::shared_ptr<PublisherBase<BlindStateMsgPubSubType>>
	pState_pub;

std::shared_ptr<PublisherBase<TimeMsgPubSubType>>
	pClock_pub;

void callback
(
	const dls2_msgs::BlindState::ConstPtr &msg
);

void clock_callback
(
	const rosgraph_msgs::Clock::ConstPtr &msg
);

int main(int argc, char** argv)
{

	pState_pub = std::make_shared<PublisherBase<BlindStateMsgPubSubType>>
		(topics::low_level_estimation::blind_state);


	pClock_pub = std::make_shared<PublisherBase<TimeMsgPubSubType>>
		(topics::simulation_time);

	ros::init(argc, argv, "roscontrol_to_dls2");
	ros::NodeHandle node_handle;

	pEmpty_pub = std::make_shared<ros::Publisher>
		(
			node_handle.advertise<std_msgs::Empty>("empty_roscontrol_to_dls2", 1000)
		);

	ros::Subscriber sub = node_handle.subscribe("/hyq/blind_state", 1000, callback);
	ros::Subscriber clock_sub = node_handle.subscribe("/clock", 1000, clock_callback);

	ros::spin();
}

// TODO temp include
#include <iostream>

void callback
(
	const dls2_msgs::BlindState::ConstPtr &msg
)
{
	// TODO temp remove
	// std_msgs::Empty empty_msg;
	// pEmpty_pub->publish(empty_msg);

	//std::cout << "callback hit" << std::endl;

	BlindStateMsg blind_state_msg;

	// =========================== Fill Header =================================
	blind_state_msg.header().time().seconds() = double(msg->header.stamp.sec) + double(msg->header.stamp.nsec)/1e9;
	blind_state_msg.header().seq() = msg->header.seq;

	// =========================== Filll Joint State ===========================
	JointStateMsg joint_state_msg;
	joint_state_msg.position().resize(msg->joint_state.position.size());
	joint_state_msg.velocity().resize(msg->joint_state.velocity.size());
	joint_state_msg.acceleration().resize(msg->joint_state.acceleration.size());
	joint_state_msg.effort().resize(msg->joint_state.effort.size());

	for(size_t i = 0; i < msg->joint_state.position.size(); ++i)
	{
		joint_state_msg.position()[i] = msg->joint_state.position[i];
	}
	for(size_t i = 0; i < msg->joint_state.velocity.size(); ++i)
	{
		joint_state_msg.velocity()[i] = msg->joint_state.velocity[i];
	}
	for(size_t i = 0; i < msg->joint_state.acceleration.size(); ++i)
	{
		joint_state_msg.acceleration()[i] = msg->joint_state.acceleration[i];
	}
	for(size_t i = 0; i < msg->joint_state.effort.size(); ++i)
	{
		joint_state_msg.effort()[i] = msg->joint_state.effort[i];
	}

	blind_state_msg.joint_state(joint_state_msg);

	// ============================== Fill Pose ===============================
	blind_state_msg.base_pose_world().position
		(
			{
				msg->base_pose_world.position[0],
				msg->base_pose_world.position[1],
				msg->base_pose_world.position[2]
			}
		);

	blind_state_msg.base_pose_world().quaternion
		(
			{
				msg->base_pose_world.quaternion[0],
				msg->base_pose_world.quaternion[1],
				msg->base_pose_world.quaternion[2],
				msg->base_pose_world.quaternion[3]
			}
		);

	// ========================== Fill Base Velocity ===========================
	for(size_t i = 0 ; i != msg->base_velocity_world.linear.size(); ++i)
	{
		blind_state_msg.base_velocity_world().linear()[i] =
			msg->base_velocity_world.linear[i];
	}
	for(size_t i = 0 ; i != msg->base_velocity_world.angular.size(); ++i)
	{
		blind_state_msg.base_velocity_world().angular()[i] =
			msg->base_velocity_world.angular[i];
	}


	// ======================== Fill Base Acceleration =========================
	for(size_t i = 0 ; i != msg->base_acceleration_world.linear.size(); ++i)
	{
		blind_state_msg.base_acceleration_world().linear()[i] =
			msg->base_acceleration_world.linear[i];
	}
	for(size_t i = 0 ; i != msg->base_acceleration_world.angular.size(); ++i)
	{
		blind_state_msg.base_acceleration_world().angular()[i] =
			msg->base_acceleration_world.angular[i];
	}

	pState_pub->publish(blind_state_msg);
// #ifndef NDEBUG
// 	{
// 		auto end_time = ros::Time::now();
// 		double end_time_seconds = end_time.sec + end_time.nsec * 1e-9;
// 		double msg_time_seconds = msg->header.stamp.sec + msg->header.stamp.nsec * 1e-9;
// 		std::stringstream ss;
// 		ss << "ros_topic_to_dls2 delay: " << (end_time_seconds - msg_time_seconds)*1000000 << " useconds";
// 		dls::logging::cout << ss.str() << std::endl;
// 	}
// #endif
}

void clock_callback
(
	const rosgraph_msgs::Clock::ConstPtr &msg
)
{
	TimeMsg rtps_time;
	rtps_time.seconds(ros::Time::now().toSec());
	pClock_pub->publish(rtps_time);
}
