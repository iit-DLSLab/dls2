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
#ifndef ATTITUDE_HPP
#define ATTITUDE_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/attitude_estimationPubSubTypes.h"

#include <Eigen/Dense>

/// A struct representing the control signal that is output by a Controller
namespace dls
{
    class Attitude : public Wrapper<AttitudeMsg>
    {
    public:
        Attitude();
        Attitude(Attitude&);
        ~Attitude() = default;

        operator AttitudeMsg() const override;
        Attitude& operator=(const AttitudeMsg&) override;
        Attitude& operator=(const Attitude&);

        Eigen::Quaterniond orientation;

        Eigen::Vector3d angular_velocity;

        Eigen::Vector3d euler_angles_imu;
        Eigen::Vector3d euler_angles_est;
        
        double timestamp;
    };
} // end namespace dls
#endif /* end of include guard: ATTITUDE_HPP */
