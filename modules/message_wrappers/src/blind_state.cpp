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
    joint_effort(pRobot->makeJointState()),
	base_pose_world(),
	base_velocity_world(),
	base_acceleration_world(),
	time()
{}

BlindState& BlindState::operator= (BlindStateMsg msg)
{
	this->joint_position = msg.joint_state().position();
    this->joint_velocity = msg.joint_state().velocity();
    this->joint_acceleration = msg.joint_state().acceleration();
    this->joint_effort = msg.joint_state().effort(); 
	
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
	
	msg.base_pose_world(this->base_pose_world);
	msg.base_velocity_world(this->base_velocity_world);
	msg.base_acceleration_world(this->base_acceleration_world);
  	msg.header().time().seconds(this->time);
    return msg;
}
