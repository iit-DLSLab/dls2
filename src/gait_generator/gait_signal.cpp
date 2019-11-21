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
// =============================================================================
// Includes
// =============================================================================
#include "gait_generator/gait_signal.hpp"
#include "robot/robot.hpp"

// =============================================================================
// Constructors
// =============================================================================
GaitSignal::GaitSignal() :

    desired_com_pose_world(),
    desired_com_velocity_world(),
    desired_com_acceleration_world(),

    desired_base_pose_world(),
    desired_base_velocity_world(),
    desired_base_acceleration_world(),

	desired_joint_state()
{ }

// =============================================================================
// RTPS Util
// =============================================================================
// -----------------------------------------------------------------------------
// Converting Constructor
// -----------------------------------------------------------------------------
TODO("stance feet")
GaitSignal::GaitSignal(GaitSignalMsg msg) :
    desired_com_pose_world(msg.desired_com_pose_world()),
    desired_com_velocity_world(msg.desired_com_velocity_world()),
    desired_com_acceleration_world(msg.desired_com_velocity_world()),

    desired_base_pose_world(msg.desired_base_pose_world()),
    desired_base_velocity_world(msg.desired_base_velocity_world()),
    desired_base_acceleration_world(msg.desired_base_acceleration_world()),

    desired_joint_state(msg.desired_joint_state())
{ }

// -----------------------------------------------------------------------------
// Type Casting
// -----------------------------------------------------------------------------
GaitSignal::operator GaitSignalMsg() const
{
    GaitSignalMsg msg;

    msg.desired_com_pose_world(this->desired_com_pose_world);
    msg.desired_com_velocity_world(this->desired_com_velocity_world);
    msg.desired_com_acceleration_world(this->desired_com_acceleration_world);

    msg.desired_base_pose_world(this->desired_base_pose_world);
    msg.desired_base_velocity_world(this->desired_base_velocity_world);
    msg.desired_base_acceleration_world(this->desired_base_acceleration_world);

	msg.desired_joint_state(this->desired_joint_state);

    return msg;
}
