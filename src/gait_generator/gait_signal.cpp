#include "gait_generator/gait_signal.hpp"
#include "robot/robot.hpp"

GaitSignal::GaitSignal() :

	desired_com_pose(),
	desired_com_velocity(),
	desired_com_acceleration(),

	desired_base_pose(),
	desired_base_velocity(),
	desired_base_acceleration(),

	desired_joint_state(),
	desired_joint_velocity(),
	desired_joint_acceleration(),

	desired_feed_forward_torque()
{
	int joint_space_dimension = Robot::getJointSpaceDimension();

	desired_joint_state.resize(joint_space_dimension, 1);
	desired_joint_state = Eigen::MatrixXd::Zero(joint_space_dimension, 1);

	desired_joint_velocity.resize(joint_space_dimension, 1);
	desired_joint_velocity = Eigen::MatrixXd::Zero(joint_space_dimension, 1);

	desired_joint_acceleration.resize(joint_space_dimension, 1);
	desired_joint_acceleration = Eigen::MatrixXd::Zero(joint_space_dimension, 1);

	desired_feed_forward_torque.resize(joint_space_dimension, 1);
	desired_feed_forward_torque = Eigen::MatrixXd::Zero(joint_space_dimension, 1);
}
