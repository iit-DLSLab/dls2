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
#ifndef JOINT_STATE_CPP_8TOBMLEP
#define JOINT_STATE_CPP_8TOBMLEP

#include "msg_wrappers/joint_state.hpp"
#include "robot/robot.hpp"

using namespace dls;
JointState::JointState() :
	position(),
	velocity(),
	effort()
{
    TODO("Robot is unimplemented")
    /*int joint_space_dimension = Robot::getJointSpaceDimension();

    position.resize(joint_space_dimension, 1);
	position   =   Eigen::MatrixXd::Zero(joint_space_dimension, 1);

    velocity.resize(joint_space_dimension, 1);
	velocity	=  Eigen::MatrixXd::Zero(joint_space_dimension,  1);

    acceleration.resize(joint_space_dimension, 1);
    acceleration = Eigen::MatrixXd::Zero(joint_space_dimension, 1);

    effort.resize(joint_space_dimension, 1);
    effort = Eigen::MatrixXd::Zero(joint_space_dimension, 1);*/
}

JointState::JointState(JointStateMsg &msg) :
	position(Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(msg.position().data(), msg.position().size())),
	velocity(Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(msg.velocity().data(), msg.velocity().size())),
	acceleration(Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(msg.acceleration().data(), msg.acceleration().size())),
	effort(Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(msg.effort().data(), msg.effort().size()))
{ }

JointState::operator JointStateMsg() const
{
	// TODO do not assign this here, do not resize this here
	JointStateMsg msg;

	msg.position().resize(this->position.size());
	msg.velocity().resize(this->velocity.size());
	msg.acceleration().resize(this->acceleration.size());
	msg.effort().resize(this->effort.size());

	Eigen::VectorXd::Map(&msg.position()[0], this->position.size()) = this->position;
	Eigen::VectorXd::Map(&msg.velocity()[0], this->velocity.size()) = this->velocity;
	Eigen::VectorXd::Map(&msg.acceleration()[0], this->acceleration.size()) = this->acceleration;
	Eigen::VectorXd::Map(&msg.effort()[0], this->effort.size()) = this->effort;

	return msg;
}

#endif /* end of include guard: JOINT_STATE_CPP_8TOBMLEP */
