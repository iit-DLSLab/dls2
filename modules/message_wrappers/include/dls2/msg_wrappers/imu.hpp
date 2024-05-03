#ifndef IMU_SIGNAL_HPP
#define IMU_SIGNAL_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/imu.h"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class Imu : public Wrapper<ImuMsg>
	{
	public:
	    template <typename SignalType>
		friend class SignalWriter;
		friend class PluginBase;

		Imu();
		Imu(Imu& imu);
        virtual ~Imu();

		operator ImuMsg() const override;
		Imu& operator=(const ImuMsg& imu_msg) override;
        Imu& operator=(const Imu& imu);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

		Eigen::Quaterniond orientation_{Eigen::Quaterniond::Identity()};
		Eigen::Matrix3d orientation_covariance_{Eigen::Matrix3d::Zero()};
		Eigen::Vector3d angular_velocity_{Eigen::Vector3d::Zero()};
		Eigen::Matrix3d angular_velocity_covariance_{Eigen::Matrix3d::Zero()};
		Eigen::Vector3d linear_acceleration_{Eigen::Vector3d::Zero()};
		Eigen::Matrix3d linear_acceleration_covariance_{Eigen::Matrix3d::Zero()};

	private:
		virtual void setDataFromWrapperBase(WrapperBase* wrapper_base) override;

	};
} // namespace dls

#endif