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
	, foot_position(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, foot_velocity(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, foot_acceleration(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, stance_legs(pRobot->makeLegDataMap<bool>(false))
	, time(0)
{ }

BlindState::BlindState(BlindState& from)
	: robot_name(from.robot_name)
	, joint_name(from.joint_name) 
	, joint_position(from.joint_position)
	, joint_velocity(from.joint_velocity)
	, joint_acceleration(from.joint_acceleration)
	, joint_effort(from.joint_effort)
	, foot_position(from.foot_position)
	, foot_velocity(from.foot_velocity)
	, foot_acceleration(from.foot_acceleration)
	, base_pose_world(from.base_pose_world)
	, base_vel_world(from.base_vel_world)
	, base_acc_world(from.base_acc_world)
	, stance_legs(from.stance_legs)
	, time(from.time)
{ }

BlindState::~BlindState()
{ }

BlindState::operator BlindStateMsg() const
{
    BlindStateMsg msg;

	msg.robot_name() = this->robot_name;

	int leg_id = 0;
	int leg_joint_id = 0;
	for(auto &leg : this->joint_position)
	{
		leg_joint_id = 0;
		int idx = leg_id*leg.key_->getNJoints();
		for(auto &joint : *leg.data_)
		{
			msg.joint_name()[idx+leg_joint_id] = this->joint_name[joint.key_];
			msg.joint_pos()[idx+leg_joint_id] = this->joint_position[joint.key_];
			msg.joint_vel()[idx+leg_joint_id] = this->joint_velocity[joint.key_];
			msg.joint_acc()[idx+leg_joint_id] = this->joint_acceleration[joint.key_];
			msg.joint_eff()[idx+leg_joint_id] = this->joint_effort[joint.key_];
			leg_joint_id++;
		}

		int idx_xyz = leg_id*3;
		for(int i=0;i<3;i++)
		{
			msg.foot_position()[idx_xyz + i] = this->foot_position[leg.key_][i];
			msg.foot_velocity()[idx_xyz + i] = this->foot_velocity[leg.key_][i];
			msg.foot_acceleration()[idx_xyz + i] = this->foot_acceleration[leg.key_][i];
		}
		
		msg.stance_legs()[leg_id] = this->stance_legs[leg.key_];
		leg_id++;
	}

	for(int i=0; i<3;i++)
	{
		msg.base_pos_world()[i] = this->base_pose_world.toPosition()[i];
		msg.base_lin_vel_world()[i] = this->base_vel_world.getLinear()[i];
		msg.base_ang_vel_world()[i] = this->base_vel_world.getAngular()[i];
		msg.base_lin_acc_world()[i] = this->base_acc_world.getLinear()[i];
		msg.base_ang_acc_world()[i] = this->base_acc_world.getAngular()[i];
	}

	msg.base_ori_world()[0] = this->base_pose_world.toQuaternion().x();
	msg.base_ori_world()[1] = this->base_pose_world.toQuaternion().y();
	msg.base_ori_world()[2] = this->base_pose_world.toQuaternion().z();
	msg.base_ori_world()[3] = this->base_pose_world.toQuaternion().w();

	msg.time(this->time);

    return msg;
}

BlindState& BlindState::operator= (const BlindStateMsg& msg)
{
	this->robot_name = msg.robot_name();

	int leg_id = 0;
	for(auto &leg : this->joint_position)
	{
		int i = leg_id*leg.key_->getNJoints();
		for(auto &joint : *leg.data_)
		{
			this->joint_name[joint.key_] = msg.joint_name()[i];
			this->joint_position[joint.key_] = msg.joint_pos()[i];
			this->joint_velocity[joint.key_] = msg.joint_vel()[i];
			this->joint_acceleration[joint.key_] = msg.joint_acc()[i];
			this->joint_effort[joint.key_] = msg.joint_eff()[i];
			i++;
		}

		int idx_xyz = leg_id*3;
		for(int i=0;i<3;i++)
		{
			this->foot_position[leg.key_][i] = msg.foot_position()[idx_xyz+i];
			this->foot_velocity[leg.key_][i] = msg.foot_velocity()[idx_xyz+i];
			this->foot_acceleration[leg.key_][i] = msg.foot_acceleration()[idx_xyz+i];
		}

		this->stance_legs[leg.key_] = msg.stance_legs()[leg_id];
		leg_id++;
	}

	this->base_pose_world.set(Eigen::Vector3d(msg.base_pos_world().data()));
	this->base_pose_world.set(Eigen::Quaterniond(msg.base_ori_world()[3], msg.base_ori_world()[0], msg.base_ori_world()[1], msg.base_ori_world()[2]));
	this->base_vel_world.setLinear(Eigen::Vector3d(msg.base_lin_vel_world().data()));
	this->base_vel_world.setAngular(Eigen::Vector3d(msg.base_ang_vel_world().data()));
	this->base_acc_world.setLinear(Eigen::Vector3d(msg.base_lin_acc_world().data()));
	this->base_acc_world.setAngular(Eigen::Vector3d(msg.base_ang_acc_world().data()));

	this->time = msg.time();

	return *this;
}

#endif // BLIND_STATE_CPP