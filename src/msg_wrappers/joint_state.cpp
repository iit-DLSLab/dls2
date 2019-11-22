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

dls::JointState::JointState() :
	position(),
	velocity(),
	effort()
{ }

dls::JointState::JointState(JointStateMsg &msg) :
	position(Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(msg.position().data(), msg.position().size())),
	velocity(Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(msg.velocity().data(), msg.velocity().size())),
	effort(Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(msg.effort().data(), msg.effort().size()))
{ }

dls::JointState::operator JointStateMsg() const
{
	// TODO do not assign this here, do not resize this here
	JointStateMsg msg;

	msg.position().resize(this->position.size());
	msg.velocity().resize(this->velocity.size());
	msg.effort().resize(this->effort.size());

	Eigen::VectorXd::Map(&msg.position()[0], this->position.size()) = this->position;
	Eigen::VectorXd::Map(&msg.velocity()[0], this->velocity.size()) = this->velocity;
	Eigen::VectorXd::Map(&msg.effort()[0], this->effort.size()) = this->effort;

	return msg;
}

#endif /* end of include guard: JOINT_STATE_CPP_8TOBMLEP */
