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
#ifndef LEG_ODOMETRY_DEBUG_HPP
#define LEG_ODOMETRY_DEBUG_HPP

#include "robotlib/robot_base.hpp"

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/debug_leg_odometry.h"

namespace dls
{
	class LegOdometryDebug : public Wrapper<LegOdometryDebugMsg>
	{
	public:
		LegOdometryDebug(const std::shared_ptr<robotlib::RobotBase>);
		LegOdometryDebug(LegOdometryDebug&);
		LegOdometryDebug() = delete;
        ~LegOdometryDebug();

		operator LegOdometryDebugMsg() const override;
		LegOdometryDebug& operator= (const LegOdometryDebugMsg&) override;

        //GROUND TRUTH POSE FROM GAZEBO
        Eigen::Vector3d position_gt;
        Eigen::Quaterniond orientation_gt;

        //GROUND TRUTH VELOCITY FROM GAZEBO
        Eigen::Vector3d lin_vel_gt;
        Eigen::Vector3d ang_vel_gt;

        // MEAN
        Eigen::Vector3d position_mean;
        Eigen::Quaterniond orientation_mean;
        Eigen::Vector3d lin_vel_mean;
        Eigen::Vector3d ang_vel_mean;

        //ESTIMATION ERROR
        Eigen::Vector3d position_error;
        Eigen::Quaterniond orientation_error;
        Eigen::Vector3d lin_vel_error;
        Eigen::Vector3d ang_vel_error;

        //LEG ODOMETRY POSE
        robotlib::LegDataMap<Eigen::Vector3d> position_legs;
        robotlib::LegDataMap<Eigen::Quaterniond> orientation_legs;

        //LEG ODOMETRY VELOCITY
        robotlib::LegDataMap<Eigen::Vector3d> lin_vel_legs;
        robotlib::LegDataMap<Eigen::Vector3d> ang_vel_legs;

        //LEG ODOMETRY ERROR
        robotlib::LegDataMap<Eigen::Vector3d> lin_vel_error_legs;
        robotlib::LegDataMap<Eigen::Vector3d> ang_vel_error_legs;
    };
} // end namespace dls

#endif /* end of include guard: LEG_ODOMETRY_DEBUG_HPP */