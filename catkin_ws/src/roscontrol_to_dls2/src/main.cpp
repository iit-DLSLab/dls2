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
#include "topics/low_level_estimation/blind_state.hpp"

// cpp
#include <memory>

// project
#include "geometry/pose.hpp"

#include <dls2_msgs/BlindState.h>
// TODO temp
#include <std_msgs/Empty.h>
std::shared_ptr<ros::Publisher> pEmpty_pub;
using namespace dls;
// needs to be a pointer, else it crashes at launch
std::shared_ptr<PublisherBase<BlindStateMsgPubSubType>>
	pState_pub;

void callback
(
	const dls2_msgs::BlindState::ConstPtr &msg
);

typedef message_filters::sync_policies::ApproximateTime
<
	sensor_msgs::JointState,
	// geometry_msgs::PoseWithCovarianceStamped,
	// sensor_msgs::Imu,
	nav_msgs::Odometry
> policy;

int main(int argc, char** argv)
{

	pState_pub = std::make_shared<PublisherBase<BlindStateMsgPubSubType>>
		(topics::low_level_estimation::blind_state);


	ros::init(argc, argv, "roscontrol_to_dls2");
	ros::NodeHandle node_handle;

	pEmpty_pub = std::make_shared<ros::Publisher>
		(
			node_handle.advertise<std_msgs::Empty>("empty_roscontrol_to_dls2", 1000)
		);

	ros::Subscriber sub = node_handle.subscribe("/hyq/blind_state", 1000, callback);

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
	std_msgs::Empty empty_msg;
	pEmpty_pub->publish(empty_msg);

	std::cout << "callback hit" << std::endl;

	BlindStateMsg blind_state_msg;
	JointStateMsg joint_state_msg;

	// =========================== Filll Joint State ===========================
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

// ================================== OLD ======================================
	// BlindStateMsg blind_state_msg;
	// JointStateMsg joint_state_msg;

	// // Fill the joint states
	// joint_state_msg.position(joint_states.position);
	// joint_state_msg.velocity(joint_states.velocity);
	// joint_state_msg.effort(joint_states.effort);
	// blind_state_msg.joint_state(joint_state_msg);

	// // Fill the base pose
	// // pose.pose because ros is poorly designed
	// Eigen::Vector3d position;
	// position << ground_truth.pose.pose.position.x,
	// 			ground_truth.pose.pose.position.y,
	// 			ground_truth.pose.pose.position.z;

	// Eigen::Quaterniond quat
	// 	(
	// 		ground_truth.pose.pose.orientation.w,
	// 		ground_truth.pose.pose.orientation.x,
	// 		ground_truth.pose.pose.orientation.y,
	// 		ground_truth.pose.pose.orientation.z
	// 	);

	// Pose p(position, quat);
	// blind_state_msg.base_pose_world(p);

	// // Fill the base velocity
	// blind_state_msg.base_velocity_world().linear(
	// 		{
	// 			ground_truth.twist.twist.linear.x,
	// 			ground_truth.twist.twist.linear.y,
	// 			ground_truth.twist.twist.linear.z
	// 		}
	// 	);
	// blind_state_msg.base_velocity_world().angular(
	// 		{
	// 			ground_truth.twist.twist.angular.x,
	// 			ground_truth.twist.twist.angular.y,
	// 			ground_truth.twist.twist.angular.z
	// 		}
	// 	);

	// // Fill the base acceleration
	// // TODO this is currently zero
	// blind_state_msg.base_acceleration_world().linear(
	// 		{
	// 			0,
	// 			0,
	// 			0
	// 		}
	// 	);
	// blind_state_msg.base_acceleration_world().angular(
	// 		{
	// 			0,
	// 			0,
	// 			0
	// 		}
	// 	);

	// pState_pub->publish(blind_state_msg);
}
