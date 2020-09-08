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
// =============================================================================
// Includes
// =============================================================================
#include "dls2/gait_generator/gait_signal.hpp"
#include "dls2/robot/robot.hpp"

using namespace dls;
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

	desired_joint_state(),
	stance_legs(),

	desired_base_wrench()
{ }

// =============================================================================
// RTPS Util
// =============================================================================
// -----------------------------------------------------------------------------
// Converting Constructor
// -----------------------------------------------------------------------------
// TODO ("stance feet")
GaitSignal::GaitSignal(GaitSignalMsg msg) :
    desired_com_pose_world(msg.desired_com_pose_world()),
    desired_com_velocity_world(msg.desired_com_velocity_world()),
    desired_com_acceleration_world(msg.desired_com_velocity_world()),

    desired_base_pose_world(msg.desired_base_pose_world()),
    desired_base_velocity_world(msg.desired_base_velocity_world()),
    desired_base_acceleration_world(msg.desired_base_acceleration_world()),

    desired_joint_state(msg.desired_joint_state()),

	stance_legs(),

	desired_base_wrench(msg.desired_base_wrench())
{
	// TODO this loop is bad, but I'm forced by legacy code. Remove the
	// pointless leg data map class, or improve it for move symantics, then fix
	// this
	for(unsigned int i = 0; i != msg.stance_feet().size(); ++i)
	{
		this->stance_legs[i] = msg.stance_feet()[i];
	}

}

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

	// TODO this loop is bad, but I'm forced by legacy code. Remove the
	// pointless leg data map class, or improve it for move symantics, then fix
	// this
	msg.stance_feet().resize(4);
	for(unsigned int i = 0; i != 4; ++i)
	{
		msg.stance_feet()[i] = this->stance_legs[i];
	}

	msg.desired_base_wrench(this->desired_base_wrench);

    return msg;
}
