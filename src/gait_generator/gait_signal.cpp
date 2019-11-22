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
dls::GaitSignal::GaitSignal() :

    desired_com_pose(),
    desired_com_velocity(),
    desired_com_acceleration(),

    desired_base_pose(),
    desired_base_velocity(),
    desired_base_acceleration(),

    desired_joint_position(),
    desired_joint_velocity(),
    desired_joint_acceleration(),

    desired_feed_forward_torque()
{
    TODO("Robot is unimplemented")
    int joint_space_dimension = Robot::getJointSpaceDimension();

    desired_joint_position.resize(joint_space_dimension, 1);
	desired_joint_position   =   Eigen::MatrixXd::Zero(joint_space_dimension,	 1);

    desired_joint_velocity.resize(joint_space_dimension, 1);
	desired_joint_velocity	=  Eigen::MatrixXd::Zero(joint_space_dimension,  1);

    desired_joint_acceleration.resize(joint_space_dimension, 1);
    desired_joint_acceleration = Eigen::MatrixXd::Zero(joint_space_dimension, 1);

    desired_feed_forward_torque.resize(joint_space_dimension, 1);
    desired_feed_forward_torque = Eigen::MatrixXd::Zero(joint_space_dimension, 1);
}

// =============================================================================
// RTPS Util
// =============================================================================
// -----------------------------------------------------------------------------
// Converting Constructor
// -----------------------------------------------------------------------------
TODO("stance feet")
dls::GaitSignal::GaitSignal(GaitSignalMsg msg) :
    desired_com_pose(msg.desired_com_pose()),
    desired_com_velocity(msg.desired_com_velocity()),
    desired_com_acceleration(msg.desired_com_velocity()),

    desired_base_pose(msg.desired_base_pose()),
    desired_base_velocity(msg.desired_base_velocity()),
    desired_base_acceleration(msg.desired_base_acceleration()),

    desired_joint_position
    (
        Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>
        (
			msg.desired_joint_state().data(), msg.desired_joint_state().size()
        )
    ),
    desired_joint_velocity
    (
        Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>
        (
            msg.desired_joint_velocity().data(), msg.desired_joint_velocity().size()
        )
    ),
    desired_joint_acceleration
    (
        Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>
        (
            msg.desired_joint_acceleration().data(), msg.desired_joint_acceleration().size()
        )
    ),

    desired_feed_forward_torque
    (
        Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>
        (
            msg.desired_feed_forward_torque().data(), msg.desired_feed_forward_torque().size()
        )
    )
{ }

// -----------------------------------------------------------------------------
// Type Casting
// -----------------------------------------------------------------------------
dls::GaitSignal::operator GaitSignalMsg() const
{
    GaitSignalMsg msg;

    msg.desired_com_pose(this->desired_com_pose);
    msg.desired_com_velocity(this->desired_com_velocity);
    msg.desired_com_acceleration(this->desired_com_acceleration);

    msg.desired_base_pose(this->desired_base_pose);
    msg.desired_base_velocity(this->desired_base_velocity);
    msg.desired_base_acceleration(this->desired_base_acceleration);

    TODO("Robot is unimplemented")
    int joint_space_dimension = Robot::getJointSpaceDimension();

    const double *p = this->desired_joint_position.data();
	std::vector<double> desired_joint_state_temp;
	desired_joint_state_temp.resize(joint_space_dimension);
	std::copy(p, p + joint_space_dimension, desired_joint_state_temp.begin());
	msg.desired_joint_state(desired_joint_state_temp);

    p = this->desired_joint_velocity.data();
	std::vector<double> desired_joint_velocity_temp;
	desired_joint_velocity_temp.resize(joint_space_dimension);
	std::copy(p, p + joint_space_dimension, desired_joint_velocity_temp.begin());
	msg.desired_joint_velocity(desired_joint_velocity_temp);

    p = this->desired_joint_acceleration.data();
	std::vector<double> desired_joint_acceleration_temp;
	desired_joint_acceleration_temp.resize(joint_space_dimension);
	std::copy(p, p + joint_space_dimension, desired_joint_acceleration_temp.begin());
	msg.desired_joint_acceleration(desired_joint_acceleration_temp);

    p = this->desired_feed_forward_torque.data();
	std::vector<double> desired_feed_forward_torque_temp;
	desired_feed_forward_torque_temp.resize(joint_space_dimension);
	std::copy(p, p + joint_space_dimension, desired_feed_forward_torque_temp.begin());
	msg.desired_feed_forward_torque(desired_feed_forward_torque_temp);

    return msg;
}
