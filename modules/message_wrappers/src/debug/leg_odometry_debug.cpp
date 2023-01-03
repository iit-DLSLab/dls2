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

#ifndef LEG_ODOMETRY_DEBUG_CPP
#define LEG_ODOMETRY_DEBUG_CPP

#include "dls2/msg_wrappers/debug/leg_odometry_debug.hpp"

using namespace dls;

LegOdometryDebug::LegOdometryDebug(const std::shared_ptr<robotlib::RobotBase> pRobot)
	: position_gt(Eigen::Vector3d::Zero())
	, orientation_gt(Eigen::Quaterniond::Identity())
	, lin_vel_gt(Eigen::Vector3d::Zero())
	, ang_vel_gt(Eigen::Vector3d::Zero())
	, position_mean(Eigen::Vector3d::Zero())
	, orientation_mean(Eigen::Quaterniond::Identity())
	, lin_vel_mean(Eigen::Vector3d::Zero())
	, ang_vel_mean(Eigen::Vector3d::Zero())
	, position_error(Eigen::Vector3d::Zero())
	, orientation_error(Eigen::Quaterniond::Identity())
	, lin_vel_error(Eigen::Vector3d::Zero())
	, ang_vel_error(Eigen::Vector3d::Zero())	
	, position_legs(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , orientation_legs((pRobot->makeLegDataMap<Eigen::Quaterniond>(Eigen::Quaterniond::Identity())))
    , lin_vel_legs((pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())))
    , ang_vel_legs((pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())))
    , lin_vel_error_legs((pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())))
    , ang_vel_error_legs((pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())))
{ }

LegOdometryDebug::LegOdometryDebug(LegOdometryDebug& from)
	: position_gt(from.position_gt)
	, orientation_gt(from.orientation_gt)
	, lin_vel_gt(from.lin_vel_gt)
	, ang_vel_gt(from.ang_vel_gt)
	, position_mean(from.position_mean)
	, orientation_mean(from.orientation_mean)
	, lin_vel_mean(from.lin_vel_mean)
	, ang_vel_mean(from.ang_vel_mean)
	, position_error(from.position_error)
	, orientation_error(from.orientation_error)
	, lin_vel_error(from.lin_vel_error)
	, ang_vel_error(from.ang_vel_error)
    , position_legs(from.position_legs)
    , orientation_legs(from.orientation_legs)
    , lin_vel_legs(from.lin_vel_legs)
    , ang_vel_legs(from.ang_vel_legs)
    , lin_vel_error_legs(from.lin_vel_error_legs)
    , ang_vel_error_legs(from.ang_vel_error_legs)
{ }

LegOdometryDebug::~LegOdometryDebug()
{}

LegOdometryDebug::operator LegOdometryDebugMsg() const
{
    LegOdometryDebugMsg msg;

	int i_leg = 0;
	for(auto &leg : this->position_legs)
	{
        msg.orientation_legs()[i_leg*4] = orientation_legs[leg.key_].x();
        msg.orientation_legs()[i_leg*4+1] = orientation_legs[leg.key_].y();
        msg.orientation_legs()[i_leg*4+2] = orientation_legs[leg.key_].z();
        msg.orientation_legs()[i_leg*4+3] = orientation_legs[leg.key_].w();
		for(int i=0; i<3; i++)
		{
			msg.position_legs()[i_leg*3 + i] = position_legs[leg.key_][i];
            msg.lin_vel_legs()[i_leg*3 + i] = lin_vel_legs[leg.key_][i];
            msg.ang_vel_legs()[i_leg*3 + i] = ang_vel_legs[leg.key_][i];
            msg.lin_vel_error_legs()[i_leg*3 + i] = lin_vel_error_legs[leg.key_][i];
            msg.ang_vel_error_legs()[i_leg*3 + i] = ang_vel_error_legs[leg.key_][i];

		}
		i_leg++;
	}

	msg.orientation_gt()[0] = this->orientation_gt.x();
	msg.orientation_gt()[1] = this->orientation_gt.y();
	msg.orientation_gt()[2] = this->orientation_gt.z();
	msg.orientation_gt()[3] = this->orientation_gt.w();
	msg.orientation_error()[0] = this->orientation_error.x();
	msg.orientation_error()[1] = this->orientation_error.y();
	msg.orientation_error()[2] = this->orientation_error.z();
	msg.orientation_error()[3] = this->orientation_error.w();
	msg.orientation_mean()[0] = this->orientation_mean.x();
	msg.orientation_mean()[1] = this->orientation_mean.y();
	msg.orientation_mean()[2] = this->orientation_mean.z();
	msg.orientation_mean()[3] = this->orientation_mean.w();

	for(int i=0; i<3;i++)
	{
		msg.position_gt()[i] = this->position_gt(i);
		msg.lin_vel_gt()[i] = this->lin_vel_gt(i);
		msg.ang_vel_gt()[i] = this->ang_vel_gt(i);
		msg.position_mean()[i] = this->position_mean(i);
		msg.lin_vel_mean()[i] = this->lin_vel_mean(i);
		msg.ang_vel_mean()[i] = this->ang_vel_mean(i);
		msg.position_error()[i] = this->position_error(i);
		msg.lin_vel_error()[i] = this->lin_vel_error(i);
		msg.ang_vel_error()[i] = this->ang_vel_error(i);
	}

    return msg;
}

LegOdometryDebug& LegOdometryDebug::operator= (LegOdometryDebugMsg& msg)
{
	int i_leg = 0;
	for(auto &leg : this->position_legs)
	{
        orientation_legs[leg.key_].x() = msg.orientation_legs()[i_leg*4];
        orientation_legs[leg.key_].y() = msg.orientation_legs()[i_leg*4+1];
        orientation_legs[leg.key_].z() = msg.orientation_legs()[i_leg*4+2];
        orientation_legs[leg.key_].w() = msg.orientation_legs()[i_leg*4+3];

		for(int i=0;i<3;i++)
		{
			position_legs[leg.key_][i] = msg.position_legs()[i_leg*3+i];
			lin_vel_legs[leg.key_][i] = msg.lin_vel_legs()[i_leg*3+i];
			ang_vel_legs[leg.key_][i] = msg.ang_vel_legs()[i_leg*3+i];
			lin_vel_error_legs[leg.key_][i] = msg.lin_vel_error_legs()[i_leg*3+i];
			ang_vel_error_legs[leg.key_][i] = msg.ang_vel_error_legs()[i_leg*3+i];
		}

		i_leg++;
	}

	this->orientation_gt.x() = msg.orientation_gt()[0];
	this->orientation_gt.y() = msg.orientation_gt()[1];
	this->orientation_gt.z() = msg.orientation_gt()[2];
	this->orientation_gt.w() = msg.orientation_gt()[3];
	this->orientation_error.x() = msg.orientation_error()[0];
	this->orientation_error.y() = msg.orientation_error()[1];
	this->orientation_error.z() = msg.orientation_error()[2];
	this->orientation_error.w() = msg.orientation_error()[3];
	this->orientation_mean.x() = msg.orientation_mean()[0];
	this->orientation_mean.y() = msg.orientation_mean()[1];
	this->orientation_mean.z() = msg.orientation_mean()[2];
	this->orientation_mean.w() = msg.orientation_mean()[3];

	this->position_gt = Eigen::Vector3d(msg.position_gt().data());
	this->lin_vel_gt = Eigen::Vector3d(msg.lin_vel_gt().data());
	this->ang_vel_gt = Eigen::Vector3d(msg.ang_vel_gt().data());
	this->position_mean = Eigen::Vector3d(msg.position_mean().data());
	this->lin_vel_mean = Eigen::Vector3d(msg.lin_vel_mean().data());
	this->ang_vel_mean = Eigen::Vector3d(msg.ang_vel_mean().data());
	this->position_error = Eigen::Vector3d(msg.position_error().data());
	this->lin_vel_error = Eigen::Vector3d(msg.lin_vel_error().data());
	this->ang_vel_error = Eigen::Vector3d(msg.ang_vel_error().data());

	return *this;
}

#endif // LEG_ODOMETRY_DEBUG_CPP