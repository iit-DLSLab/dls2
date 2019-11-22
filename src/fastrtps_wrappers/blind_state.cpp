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
#include "fastrtps_wrappers/blind_state.hpp"

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
BlindState::BlindState() :
	joint_state(),
	base_pose_world(),
	base_velocity_world(),
	base_acceleration_world()
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
BlindState::BlindState(BlindStateMsg msg) :
	joint_state(msg.joint_state()),
	base_pose_world(msg.base_pose_world()),
	base_velocity_world(msg.base_velocity_world()),
	base_acceleration_world(msg.base_acceleration_world())
{ }

// -----------------------------------------------------------------------------
// Type Casting
// -----------------------------------------------------------------------------
BlindState::operator BlindStateMsg() const
{
    BlindStateMsg msg;
	msg.joint_state(this->joint_state);
	msg.base_pose_world(this->base_pose_world);
	msg.base_velocity_world(this->base_velocity_world);
	msg.base_acceleration_world(this->base_acceleration_world);

    return msg;
}
