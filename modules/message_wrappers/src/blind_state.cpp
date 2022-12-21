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

BlindState::BlindState(const std::shared_ptr<robotlib::RobotBase>& pRobot)
	: joint_position(pRobot->makeJointState())
	, joint_velocity(pRobot->makeJointState())
	, joint_acceleration(pRobot->makeJointState())
	, joint_effort(pRobot->makeJointState())
{}

BlindState::BlindState(BlindState& from)
	: joint_position(from.joint_position)
	, joint_velocity(from.joint_velocity)
	, joint_acceleration(from.joint_acceleration)
	, joint_effort(from.joint_effort)
	, base_pose_world(from.base_pose_world)
	, base_velocity_world(from.base_velocity_world)
	, base_acceleration_world(from.base_acceleration_world)
	, time(from.time)
{}

BlindState::~BlindState()
{}

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

	for(int i = 0; i < 3; i++)
	{
		msg.base_pos_world()[i] = this->base_pose_world.toPosition()[i];
		msg.base_lin_vel_world()[i] = this->base_velocity_world.getLinear()[i];
		msg.base_ang_vel_world()[i] = this->base_velocity_world.getAngular()[i];
		msg.base_lin_acc_world()[i] = this->base_acceleration_world.getLinear()[i];
		msg.base_ang_acc_world()[i] = this->base_acceleration_world.getAngular()[i];
	}

	msg.base_ori_world()[0] = this->base_pose_world.toQuaternion().x();
	msg.base_ori_world()[1] = this->base_pose_world.toQuaternion().y();
	msg.base_ori_world()[2] = this->base_pose_world.toQuaternion().z();
	msg.base_ori_world()[3] = this->base_pose_world.toQuaternion().w();
	
  	msg.time(this->time);

    return msg;
}

BlindState& BlindState::operator= (BlindStateMsg& msg)
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

	this->base_pose_world.set(Eigen::Vector3d(msg.base_pos_world().data()));
	this->base_pose_world.set(Eigen::Quaterniond(msg.base_ori_world().data()));

	this->base_velocity_world.setLinear(Eigen::Vector3d(msg.base_lin_vel_world().data()));
	this->base_velocity_world.setAngular(Eigen::Vector3d(msg.base_ang_vel_world().data()));
	
	this->base_acceleration_world.setLinear(Eigen::Vector3d(msg.base_lin_acc_world().data()));
	this->base_acceleration_world.setAngular(Eigen::Vector3d(msg.base_ang_acc_world().data()));

	this->time = msg.time();

	return *this;
}

#endif // BLIND_STATE_CPP