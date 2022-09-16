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

#include "dls2/gait_generator/gait_signal.hpp"

using namespace dls;

GaitSignal::GaitSignal(GaitSignal &from) :

    desired_com_pose_world(from.desired_com_pose_world),
    desired_com_velocity_world(from.desired_com_velocity_world),
    desired_com_acceleration_world(from.desired_com_acceleration_world),

    desired_base_pose_world(from.desired_base_pose_world),
    desired_base_velocity_world(from.desired_base_velocity_world),
    desired_base_acceleration_world(from.desired_base_acceleration_world),

	desired_joint_position(from.desired_joint_position),
    desired_joint_velocity(from.desired_joint_velocity),
    desired_joint_acceleration(from.desired_joint_acceleration),
    desired_joint_effort(from.desired_joint_effort),
	stance_legs(from.stance_legs),

	desired_base_wrench(from.desired_base_wrench)
{ }


GaitSignal::GaitSignal(const std::shared_ptr<robotlib::RobotBase> &pRobot) :

    desired_com_pose_world(),
    desired_com_velocity_world(),
    desired_com_acceleration_world(),

    desired_base_pose_world(),
    desired_base_velocity_world(),
    desired_base_acceleration_world(),

	desired_joint_position(pRobot->makeJointState()),
    desired_joint_velocity(pRobot->makeJointState()),
    desired_joint_acceleration(pRobot->makeJointState()),
    desired_joint_effort(pRobot->makeJointState()),
	stance_legs(pRobot->makeLegDataMap<bool>()),

	desired_base_wrench()
{ }

// =============================================================================
// RTPS Util
// =============================================================================
// -----------------------------------------------------------------------------
// Converting Constructor
// -----------------------------------------------------------------------------
// TODO ("stance feet")
GaitSignal::GaitSignal(const std::shared_ptr<robotlib::RobotBase> &pRobot, GaitSignalMsg msg) :
    desired_com_pose_world(msg.desired_com_pose_world()),
    desired_com_velocity_world(msg.desired_com_velocity_world()),
    desired_com_acceleration_world(msg.desired_com_velocity_world()),

    desired_base_pose_world(msg.desired_base_pose_world()),
    desired_base_velocity_world(msg.desired_base_velocity_world()),
    desired_base_acceleration_world(msg.desired_base_acceleration_world()),

	desired_joint_position(pRobot->makeJointState()),
    desired_joint_velocity(pRobot->makeJointState()),
    desired_joint_acceleration(pRobot->makeJointState()),
    desired_joint_effort(pRobot->makeJointState()),

	stance_legs(pRobot->makeLegDataMap<bool>()),

	desired_base_wrench(msg.desired_base_wrench())
{
    int i = 0;
    for(auto leg : *pRobot->getLegs())
    {
        this->desired_joint_position[leg] = msg.desired_joint_position()[i];
        this->desired_joint_velocity[leg] = msg.desired_joint_velocity()[i];
        this->desired_joint_acceleration[leg] = msg.desired_joint_acceleration()[i];
        this->desired_joint_effort[leg] = msg.desired_joint_effort()[i];

        this->stance_legs[leg] = msg.stance_feet()[i];
        i++;
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

	msg.desired_joint_position(this->desired_joint_position);
    msg.desired_joint_velocity(this->desired_joint_velocity);
    msg.desired_joint_acceleration(this->desired_joint_acceleration);
    msg.desired_joint_effort(this->desired_joint_effort);    

	msg.stance_feet(this->stance_legs);

	msg.desired_base_wrench(this->desired_base_wrench);

    return msg;
}

GaitSignal &GaitSignal::operator= (GaitSignalMsg &msg)
{
    desired_com_pose_world = msg.desired_com_pose_world();
    desired_com_velocity_world = msg.desired_com_velocity_world();
    desired_com_acceleration_world = msg.desired_com_velocity_world();

    desired_base_pose_world = msg.desired_base_pose_world();
    desired_base_velocity_world = msg.desired_base_velocity_world();
    desired_base_acceleration_world = msg.desired_base_acceleration_world();

	desired_joint_position = msg.desired_joint_position();
    desired_joint_velocity = msg.desired_joint_velocity();
    desired_joint_acceleration = msg.desired_joint_acceleration();
    desired_joint_effort = msg.desired_joint_effort(); 

	desired_base_wrench = msg.desired_base_wrench();

    stance_legs = msg.stance_feet();
	
    return *this;
}
