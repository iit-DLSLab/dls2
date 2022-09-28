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

#include "dls2/msg_wrappers/gait_signal.hpp"

using namespace dls;

GaitSignal::GaitSignal(GaitSignal &from) :

    desired_com_pose_world(from.desired_com_pose_world),
    desired_com_velocity_world(from.desired_com_velocity_world),
    desired_com_acceleration_world(from.desired_com_acceleration_world),

    // desired_base_pose_world(from.desired_base_pose_world),
    // desired_base_velocity_world(from.desired_base_velocity_world),
    // desired_base_acceleration_world(from.desired_base_acceleration_world),

	desired_joint_position(from.desired_joint_position),
    desired_joint_velocity(from.desired_joint_velocity),
    desired_joint_acceleration(from.desired_joint_acceleration),
    desired_joint_effort(from.desired_joint_effort),
	stance_legs(from.stance_legs)

	// desired_base_wrench(from.desired_base_wrench)
{ }


GaitSignal::GaitSignal(const std::shared_ptr<robotlib::RobotBase> &pRobot) :

    desired_com_pose_world(),
    desired_com_velocity_world(),
    desired_com_acceleration_world(),

    // desired_base_pose_world(),
    // desired_base_velocity_world(),
    // desired_base_acceleration_world(),

	desired_joint_position(pRobot->makeJointState()),
    desired_joint_velocity(pRobot->makeJointState()),
    desired_joint_acceleration(pRobot->makeJointState()),
    desired_joint_effort(pRobot->makeJointState()),
	stance_legs(pRobot->makeLegDataMap<bool>(false))

	// desired_base_wrench()
{ }

// =============================================================================
// RTPS Util
// =============================================================================
// -----------------------------------------------------------------------------
// Converting Constructor
// -----------------------------------------------------------------------------
// TODO ("stance feet")
GaitSignal::GaitSignal(const std::shared_ptr<robotlib::RobotBase> &pRobot, GaitSignalMsg msg) :
    desired_com_pose_world(Eigen::Vector3d(msg.com_pos().data()), Eigen::Quaterniond(msg.com_ori().data())),
    desired_com_velocity_world(Eigen::Vector3d(msg.com_lin_vel().data()), Eigen::Vector3d(msg.com_ang_vel().data())),
    desired_com_acceleration_world(Eigen::Vector3d(msg.com_lin_acc().data()), Eigen::Vector3d(msg.com_ang_acc().data())),

    // desired_base_pose_world(msg.desired_base_pose_world()),
    // desired_base_velocity_world(msg.desired_base_velocity_world()),
    // desired_base_acceleration_world(msg.desired_base_acceleration_world()),

	desired_joint_position(pRobot->makeJointState()),
    desired_joint_velocity(pRobot->makeJointState()),
    desired_joint_acceleration(pRobot->makeJointState()),
    desired_joint_effort(pRobot->makeJointState()),

	stance_legs(pRobot->makeLegDataMap<bool>(false))

	// desired_base_wrench(msg.desired_base_wrench())
{
    int i = 0;
    for(auto &leg : this->desired_joint_position)
    {
        for(auto &joint : *leg.data_)
        {
            this->desired_joint_position[joint.key_] = msg.joint_pos()[i];
            this->desired_joint_velocity[joint.key_] = msg.joint_vel()[i];
            this->desired_joint_acceleration[joint.key_] = msg.joint_acc()[i];
            this->desired_joint_effort[joint.key_] = msg.joint_eff()[i];

            i++;
        }
    }

    i = 0;
    for(auto &leg_pair : this->stance_legs)
	{
    	*leg_pair.data_ = msg.stance_feet()[i];
        i++;
    }
	
    
}

// -----------------------------------------------------------------------------
// Type Casting
// -----------------------------------------------------------------------------
GaitSignal::operator GaitSignalMsg() const
{
    GaitSignalMsg msg;

    const double *p = this->desired_com_pose_world.toQuaternion().coeffs().data();
	std::copy(p, p + 4, msg.com_ori().begin());
    
    for(int i = 0; i < 3; i++)
    {
        msg.com_pos()[i] = this->desired_com_pose_world.toPosition()[i];
        msg.com_lin_vel()[i] = this->desired_com_velocity_world.getLinear()[i];
        msg.com_ang_vel()[i] = this->desired_com_velocity_world.getAngular()[i];
        msg.com_lin_acc()[i] = this->desired_com_acceleration_world.getLinear()[i];
        msg.com_ang_acc()[i] = this->desired_com_acceleration_world.getAngular()[i];
    }
    
    // msg.desired_base_pose_world(this->desired_base_pose_world);
    // msg.desired_base_velocity_world(this->desired_base_velocity_world);
    // msg.desired_base_acceleration_world(this->desired_base_acceleration_world);

    int i = 0;
	for(auto &leg_pair : this->desired_joint_position)
	{
		for(auto &joint : *leg_pair.data_)
        {
            msg.joint_pos()[i] = this->desired_joint_position[joint.key_];
            msg.joint_vel()[i] = this->desired_joint_velocity[joint.key_];
            msg.joint_acc()[i] = this->desired_joint_acceleration[joint.key_];
            msg.joint_eff()[i] = this->desired_joint_effort[joint.key_];
            i++;
        }
    }

    i = 0;
    for(auto &leg_pair : this->stance_legs)
	{
    	msg.stance_feet()[i] = *leg_pair.data_;
        i++;
    }

	// msg.desired_base_wrench(this->desired_base_wrench);

    return msg;
}

GaitSignal &GaitSignal::operator= (GaitSignalMsg &msg)
{
    desired_com_pose_world.set(Eigen::Vector3d(msg.com_pos().data()), Eigen::Quaterniond(msg.com_ori().data())),
    desired_com_velocity_world.setLinear(Eigen::Vector3d(msg.com_lin_vel().data()));
    desired_com_velocity_world.setAngular(Eigen::Vector3d(msg.com_ang_vel().data()));
    desired_com_acceleration_world.setLinear(Eigen::Vector3d(msg.com_lin_acc().data()));
    desired_com_acceleration_world.setAngular(Eigen::Vector3d(msg.com_ang_acc().data()));

    // desired_base_pose_world = msg.desired_base_pose_world();
    // desired_base_velocity_world = msg.desired_base_velocity_world();
    // desired_base_acceleration_world = msg.desired_base_acceleration_world();

    int i = 0;
    for(auto &leg_pair : this->desired_joint_position)
	{
		for(auto &joint : *leg_pair.data_)
        {
            this->desired_joint_position[joint.key_] = msg.joint_pos()[i];
            this->desired_joint_velocity[joint.key_] = msg.joint_vel()[i];
            this->desired_joint_acceleration[joint.key_] = msg.joint_acc()[i];
            this->desired_joint_effort[joint.key_] = msg.joint_eff()[i];
            i++;
        }
    }

    i = 0;
    for(auto &leg_pair : this->stance_legs)
	{
    	msg.stance_feet()[i] = *leg_pair.data_;
        i++;
    }
	
    return *this;
}
