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
#ifndef IMU_SIGNAL_HPP
#define IMU_SIGNAL_HPP


#include "robotlib/robot_base.hpp"

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/imu.h"

namespace dls
{
	class Imu : public Wrapper<ImuMsg>
	{
	public:
		Imu();
		Imu(Imu&);

        ~Imu();

		operator ImuMsg() const override;
		Imu& operator= (const ImuMsg&) override;

		Eigen::Quaterniond orientation;
		Eigen::Matrix3d orientation_covariance;
		Eigen::Vector3d angular_velocity;
		Eigen::Matrix3d angular_velocity_covariance;
		Eigen::Vector3d linear_acceleration;
		Eigen::Matrix3d linear_acceleration_covariance;

		double time;
		std::string frame_id;
	};
} // end namespace dls

#endif /* end of include guard: IMU_SIGNAL_HPP */
