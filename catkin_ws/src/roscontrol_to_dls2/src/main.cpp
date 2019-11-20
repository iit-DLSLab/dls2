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

// needs to be a pointer, else it crashes at launch
std::shared_ptr<PublisherBase<BlindStateMsgPubSubType>>
	pState_pub;

void callback
(
	const sensor_msgs::JointState &joint_states,
	// const geometry_msgs::PoseWithCovarianceStamped &pose,
	// const sensor_msgs::Imu &imu,
	const nav_msgs::Odometry &ground_truth
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

	message_filters::Subscriber<sensor_msgs::JointState>
		joint_state_sub(node_handle, "/hyq/joint_states", 1);

	// message_filters::Subscriber<geometry_msgs::PoseWithCovarianceStamped>
	// 	pose_sub(node_handle, "/hyq/pose", 1);

	// message_filters::Subscriber<sensor_msgs::Imu>
	// 	imu_sub(node_handle, "/hyq/imu", 1);

	message_filters::Subscriber<nav_msgs::Odometry>
		ground_truth_sub(node_handle, "/hyq/ground_truth", 1);


	message_filters::Synchronizer<policy> sync(policy(10), joint_state_sub, ground_truth_sub);
	// message_filters::sync_policies::ApproximateTime
	// 	<
	// 		sensor_msgs::JointState,
	// 		// geometry_msgs::PoseWithCovarianceStamped,
	// 		// sensor_msgs::Imu,
	// 		nav_msgs::Odometry
	// 	>
	// 	sync
	// 	(
	// 		joint_state_sub,
	// 		// pose_sub,
	// 		// imu_sub,
	// 		ground_truth_sub,
	// 		10
	// 	);

	sync.registerCallback(callback);

	ros::spin();
}

#include <iostream>

void callback
(
	const sensor_msgs::JointState &joint_states,
	// const geometry_msgs::PoseWithCovarianceStamped &pose,
	// const sensor_msgs::Imu &imu,
	const nav_msgs::Odometry &ground_truth
)
{
	std::cout << "callback hit" << std::endl;
	BlindStateMsg blind_state_msg;
	JointStateMsg joint_state_msg;

	// Fill the joint states
	joint_state_msg.position(joint_states.position);
	joint_state_msg.velocity(joint_states.velocity);
	joint_state_msg.effort(joint_states.effort);
	blind_state_msg.joint_state(joint_state_msg);

	// // Fill the body pose
	// // pose.pose.pose because ros is poorly designed
	// Eigen::Vector3d position;
	// position << pose.pose.pose.position.x,
	// 			pose.pose.pose.position.y,
	// 			pose.pose.pose.position.z;

	// Eigen::Quaterniond quat
	// 	(
	// 		pose.pose.pose.orientation.w,
	// 		pose.pose.pose.orientation.x,
	// 		pose.pose.pose.orientation.y,
	// 		pose.pose.pose.orientation.z
	// 	);

	// Pose p(position, quat);
	// blind_state_msg.body_pose(p);

	// Fill the body velocity
	blind_state_msg.body_velocity_world().linear(
			{
				ground_truth.twist.twist.linear.x,
				ground_truth.twist.twist.linear.y,
				ground_truth.twist.twist.linear.z
			}
		);
	blind_state_msg.body_velocity_world().angular(
			{
				ground_truth.twist.twist.angular.x,
				ground_truth.twist.twist.angular.y,
				ground_truth.twist.twist.angular.z
			}
		);

	// Fill the body acceleration
	// TODO this is currently zero
	blind_state_msg.body_acceleration_world().linear(
			{
				0,
				0,
				0
			}
		);
	blind_state_msg.body_acceleration_world().angular(
			{
				0,
				0,
				0
			}
		);

	pState_pub->publish(blind_state_msg);
}
