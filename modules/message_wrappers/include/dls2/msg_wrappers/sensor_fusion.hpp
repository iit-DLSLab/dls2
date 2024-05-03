#ifndef SENSOR_FUSION_HPP
#define SENSOR_FUSION_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/sensor_fusion.h"
#include <Eigen/Dense>

namespace dls
{
    class SensorFusion : public Wrapper<SensorFusionMsg>
    {
    public:
        template <typename SignalType>
        friend class SignalWriter;
        friend class Plugin;

        SensorFusion();
        SensorFusion(SensorFusion& sensor_fusion);
        virtual ~SensorFusion();

        operator SensorFusionMsg() const override;
        SensorFusion& operator=(const SensorFusionMsg& sensor_fusion_msg) override;
        SensorFusion& operator=(const SensorFusion& sensor_fusion);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

        Eigen::Vector3d position_{Eigen::Vector3d::Zero()};
        Eigen::Vector3d linear_velocity_{Eigen::Vector3d::Zero()};
    private:
        virtual void setDataFromWrapperBase(WrapperBase *wrapper_base) override;
    };
} // namespace dls
#endif