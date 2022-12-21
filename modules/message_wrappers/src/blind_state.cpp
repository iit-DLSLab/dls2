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

#ifndef BLIND_STATE_CPP
#define BLIND_STATE_CPP

#include "dls2/msg_wrappers/blind_state.hpp"

using namespace dls;

BlindState::BlindState(const std::shared_ptr<robotlib::RobotBase> pRobot)
	: robot_name("")
	, joint_name(pRobot->makeJointDataMap<std::string>(""))
	, joint_position(pRobot->makeJointState())
	, joint_velocity(pRobot->makeJointState())
	, joint_acceleration(pRobot->makeJointState())
	, joint_effort(pRobot->makeJointState())
	, time(0)
{
	base_pos_world.setZero();
	base_lin_vel_world.setZero();
	base_ang_vel_world.setZero();
	base_lin_acc_world.setZero();
	base_ang_acc_world.setZero();

	base_ori_world.x() = 0.0;
	base_ori_world.y() = 0.0;
	base_ori_world.z() = 0.0;
	base_ori_world.w() = 1.0;
}

BlindState::BlindState(BlindState& from)
	: robot_name(from.robot_name)
	, joint_name(from.joint_name) 
	, joint_position(from.joint_position)
	, joint_velocity(from.joint_velocity)
	, joint_acceleration(from.joint_acceleration)
	, joint_effort(from.joint_effort)
	, base_pose_world(from.base_pose_world)
	, base_velocity_world(from.base_velocity_world)
	, base_acceleration_world(from.base_acceleration_world)
	, time(from.time)
{
	base_pos_world.setZero();
	base_lin_vel_world.setZero();
	base_ang_vel_world.setZero();
	base_lin_acc_world.setZero();
	base_ang_acc_world.setZero();

	base_ori_world.x() = 0.0;
	base_ori_world.y() = 0.0;
	base_ori_world.z() = 0.0;
	base_ori_world.w() = 1.0;
}

BlindState::~BlindState()
{}

BlindState::operator BlindStateMsg() const
{
    BlindStateMsg msg;

	msg.robot_name() = this->robot_name;

	int i = 0;
	for(auto &leg : this->joint_position)
	{
		for(auto &joint : *leg.data_)
		{
			msg.joint_name()[i] = this->joint_name[joint.key_];
			msg.joint_pos()[i] = this->joint_position[joint.key_];
			msg.joint_vel()[i] = this->joint_velocity[joint.key_];
			msg.joint_acc()[i] = this->joint_acceleration[joint.key_];
			msg.joint_eff()[i] = this->joint_effort[joint.key_]; 
			i++;
		}
	}

	for(int i=0; i<3;i++)
	{
		msg.base_pos_world()[i] = this->base_pos_world(i);
		msg.base_lin_vel_world()[i] = this->base_lin_vel_world(i);
		msg.base_ang_vel_world()[i] = this->base_ang_vel_world(i);
		msg.base_lin_acc_world()[i] = this->base_lin_acc_world(i);
		msg.base_ang_acc_world()[i] = this->base_ang_acc_world(i);
	}

	msg.base_ori_world()[0] = this->base_ori_world.x();
	msg.base_ori_world()[1] = this->base_ori_world.y();
	msg.base_ori_world()[2] = this->base_ori_world.z();
	msg.base_ori_world()[3] = this->base_ori_world.w();

  	msg.time(this->time);

    return msg;
}

BlindState& BlindState::operator= (BlindStateMsg& msg)
{
	this->robot_name = msg.robot_name();

	int i = 0;
	for(auto &leg : this->joint_position)
	{
		for(auto &joint : *leg.data_)
		{
			this->joint_name[joint.key_] = msg.joint_name()[i];
			this->joint_position[joint.key_] = msg.joint_pos()[i];
			this->joint_velocity[joint.key_] = msg.joint_vel()[i];
			this->joint_acceleration[joint.key_] = msg.joint_acc()[i];
			this->joint_effort[joint.key_] = msg.joint_eff()[i]; 
			i++;
		}
	}

	for(int i=0; i<3;i++)
	{
		this->base_pos_world(i) = msg.base_pos_world()[i];
		this->base_lin_vel_world(i) = msg.base_lin_vel_world()[i];
		this->base_ang_vel_world(i) = msg.base_ang_vel_world()[i];
		this->base_lin_acc_world(i) = msg.base_lin_acc_world()[i];
		this->base_ang_acc_world(i) = msg.base_ang_acc_world()[i];
	}

	this->base_ori_world.x() = msg.base_ori_world()[0];
	this->base_ori_world.y() = msg.base_ori_world()[1];
	this->base_ori_world.z() = msg.base_ori_world()[2];
	this->base_ori_world.w() = msg.base_ori_world()[3];

	this->time = msg.time();

	return *this;
}

#endif // BLIND_STATE_CPP