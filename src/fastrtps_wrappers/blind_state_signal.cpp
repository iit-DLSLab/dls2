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
#include "fastrtps_wrappers/blind_state_signal.hpp"

// =============================================================================
// Constructors
// =============================================================================
BlindStateSignal::BlindStateSignal() :
	joint_state(),
	body_pose(),
	body_velocity(),
	body_acceleration()
{
	// TODO do not hardcode size here
	joint_state.position.resize(12);
	joint_state.velocity.resize(12);
	joint_state.effort.resize(12);
}

// =============================================================================
// RTPS Util
// =============================================================================
// -----------------------------------------------------------------------------
// Converting Constructor
// -----------------------------------------------------------------------------
BlindStateSignal::BlindStateSignal(BlindStateMsg msg) :
	joint_state(msg.joint_state()),
	body_pose(msg.body_pose()),
	body_velocity(msg.body_velocity()),
	body_acceleration(msg.body_acceleration())
{ }

// -----------------------------------------------------------------------------
// Type Casting
// -----------------------------------------------------------------------------
BlindStateSignal::operator BlindStateMsg() const
{
    BlindStateMsg msg;
	msg.joint_state(this->joint_state);
	msg.body_pose(this->body_pose);
	msg.body_velocity(this->body_velocity);
	msg.body_acceleration(this->body_acceleration);

    return msg;
}
