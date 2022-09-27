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
*******************************************************************************/
#include "dls2/msg_wrappers/blind_state.hpp"

using namespace dls;

BlindState::BlindState(const std::shared_ptr<robotlib::RobotBase> &pRobot) :
	joint_position(pRobot->makeJointState()),
    joint_velocity(pRobot->makeJointState()),
    joint_acceleration(pRobot->makeJointState()),
    joint_effort(pRobot->makeJointState())
{}

BlindState& BlindState::operator= (BlindStateMsg msg)
{
	this->joint_position = msg.joint_state().position();
    this->joint_velocity = msg.joint_state().velocity();
    this->joint_acceleration = msg.joint_state().acceleration();
    this->joint_effort = msg.joint_state().effort(); 
	
	this->orientation.w() = msg.imu().orientation()[0];
	this->orientation.x() = msg.imu().orientation()[1];
	this->orientation.y() = msg.imu().orientation()[2];
	this->orientation.z() = msg.imu().orientation()[3];

	this->angular_velocity[0] = msg.imu().angular_velocity()[0];
	this->angular_velocity[1] = msg.imu().angular_velocity()[1];
	this->angular_velocity[2] = msg.imu().angular_velocity()[2];

	this->angular_acceleration[0] = msg.imu().angular_velocity()[0];
	this->angular_acceleration[1] = msg.imu().angular_velocity()[1];
	this->angular_acceleration[2] = msg.imu().angular_velocity()[2];

	this->base_pose_world = msg.base_pose_world();
	this->base_velocity_world = msg.base_velocity_world();
	this->base_acceleration_world = msg.base_acceleration_world();

	this->time = msg.header().time().seconds();

	return *this;
}


BlindState::operator BlindStateMsg() const
{
    BlindStateMsg msg;

	msg.joint_state().position(this->joint_position);
    msg.joint_state().velocity(this->joint_velocity);
    msg.joint_state().acceleration(this->joint_acceleration);
    msg.joint_state().effort(this->joint_effort); 
	
	msg.imu().orientation()[0] = this->orientation.w();
	msg.imu().orientation()[1] = this->orientation.x();
	msg.imu().orientation()[2] = this->orientation.y();
	msg.imu().orientation()[3] = this->orientation.z();

	msg.imu().angular_velocity()[0] = this->angular_velocity[0];
	msg.imu().angular_velocity()[1] = this->angular_velocity[1];
	msg.imu().angular_velocity()[2] = this->angular_velocity[2];

	msg.imu().angular_acceleration()[0] = this->angular_acceleration[0]; 
	msg.imu().angular_acceleration()[1] = this->angular_acceleration[1];
	msg.imu().angular_acceleration()[2] = this->angular_acceleration[2];

	msg.base_pose_world(this->base_pose_world);
	msg.base_velocity_world(this->base_velocity_world);
	msg.base_acceleration_world(this->base_acceleration_world);
	
  	msg.header().time().seconds(this->time);

    return msg;
}
