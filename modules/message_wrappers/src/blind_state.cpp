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
	int i = 0;
	for(auto &leg : this->joint_position)
	{
		for(auto &joint : *leg.data_)
		{
			this->joint_position[joint.key_] = msg.joint_pos()[i];
			this->joint_velocity[joint.key_] = msg.joint_vel()[i];
			this->joint_acceleration[joint.key_] = msg.joint_acc()[i];
			this->joint_effort[joint.key_] = msg.joint_eff()[i]; 
			i++;
		}
	}
			
	this->orientation.w() = msg.orientation()[0];
	this->orientation.x() = msg.orientation()[1];
	this->orientation.y() = msg.orientation()[2];
	this->orientation.z() = msg.orientation()[3];

	this->angular_velocity[0] = msg.ang_vel()[0];
	this->angular_velocity[1] = msg.ang_vel()[1];
	this->angular_velocity[2] = msg.ang_vel()[2];

	this->angular_acceleration[0] = msg.ang_vel()[0];
	this->angular_acceleration[1] = msg.ang_vel()[1];
	this->angular_acceleration[2] = msg.ang_vel()[2];

	// this->base_pose_world = msg.base_pose_world();
	// this->base_velocity_world = msg.base_velocity_world();
	// this->base_acceleration_world = msg.base_acceleration_world();

	this->time = msg.time();

	return *this;
}


BlindState::operator BlindStateMsg() const
{
    BlindStateMsg msg;

	int i = 0;
	for(auto &leg : this->joint_position)
	{
		for(auto &joint : *leg.data_)
		{
			msg.joint_pos()[i] = this->joint_position[joint.key_];
			msg.joint_vel()[i] = this->joint_velocity[joint.key_];
			msg.joint_acc()[i] = this->joint_acceleration[joint.key_];
			msg.joint_eff()[i] = this->joint_effort[joint.key_]; 
			i++;
		}
	}
	
	msg.orientation()[0] = this->orientation.w();
	msg.orientation()[1] = this->orientation.x();
	msg.orientation()[2] = this->orientation.y();
	msg.orientation()[3] = this->orientation.z();

	msg.ang_vel()[0] = this->angular_velocity[0];
	msg.ang_vel()[1] = this->angular_velocity[1];
	msg.ang_vel()[2] = this->angular_velocity[2];

	msg.ang_acc()[0] = this->angular_acceleration[0]; 
	msg.ang_acc()[1] = this->angular_acceleration[1];
	msg.ang_acc()[2] = this->angular_acceleration[2];

	// msg.base_pose_world(this->base_pose_world);
	// msg.base_velocity_world(this->base_velocity_world);
	// msg.base_acceleration_world(this->base_acceleration_world);
	
  	msg.time(this->time);

    return msg;
}
