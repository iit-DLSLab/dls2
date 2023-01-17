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
#ifndef ODOMETRY_HPP
#define ODOMETRY_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/t265_odometryPubSubTypes.h"

#include "robotlib/robot_base.hpp"

/// A struct representing the control signal that is output by a Controller
namespace dls
{
    class Odometry : public Wrapper<T265OdometryMsg>
    {
    public:
        Odometry();
        Odometry(Odometry&);
        ~Odometry();

        operator T265OdometryMsg() const override;
        Odometry& operator=(const T265OdometryMsg&) override;
        Odometry& operator=(const Odometry&);

        Eigen::Vector3d position;
        Eigen::Quaterniond orientation;

        Eigen::Vector3d linear_velocity;
        Eigen::Vector3d angular_velocity;
        double timestamp;
    };
} // end namespace dls
#endif /* end of include guard: ODOMETRY_HPP */
