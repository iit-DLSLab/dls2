#ifndef TRANSLATION_HPP
#define TRANSLATION_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/sensor_fusionPubSubTypes.h"

#include <Eigen/Dense>

namespace dls
{
    class SensorFusion : public Wrapper<SensorFusionMsg>
    {
    public:
        SensorFusion();
        SensorFusion(SensorFusion&);
        ~SensorFusion() = default;

        operator SensorFusionMsg() const override;
        SensorFusion& operator=(const SensorFusionMsg&) override;
        SensorFusion& operator=(const SensorFusion&);

        Eigen::Vector3d position{Eigen::Vector3d::Zero()};
        Eigen::Vector3d linear_velocity{Eigen::Vector3d::Zero()};

        double timestamp;
    };
} // end namespace dls
#endif /* end of include guard:TRANSLATION_HPP */