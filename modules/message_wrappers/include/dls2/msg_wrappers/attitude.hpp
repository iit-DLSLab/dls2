
#ifndef ATTITUDE_HPP
#define ATTITUDE_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/attitude.h"
#include <Eigen/Dense>

/// A struct representing the control signal that is output by a Controller
namespace dls
{
    class Attitude : public Wrapper<AttitudeMsg>
    {
    public:
        Attitude();
        Attitude(Attitude& attitude);
        virtual ~Attitude();

        operator AttitudeMsg() const override;
        Attitude& operator=(const AttitudeMsg& attitude_msg) override;
        Attitude& operator=(const Attitude& attitude);

        std::string frame_id_{};
        uint32_t sequence_id_{};
        double timestamp_{};

        Eigen::Quaterniond orientation_{Eigen::Quaterniond::Identity()};
        Eigen::Quaterniond orient_t265_{Eigen::Quaterniond::Identity()};

        Eigen::Vector3d angular_velocity_{Eigen::Vector3d::Zero()};
        Eigen::Vector3d euler_angles_t265_{Eigen::Vector3d::Zero()};
        Eigen::Vector3d euler_angles_estimation_{Eigen::Vector3d::Zero()};        
    };
} // namespace dls

#endif