
#ifndef T265_ODOMETRY_HPP
#define T265_ODOMETRY_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/t265_odometry.h"
#include "robotlib/robot_base.hpp"

namespace dls
{
    class T265Odometry : public Wrapper<T265OdometryMsg>
    {
    public:
	    template <typename SignalType>
		friend class SignalWriter;
		friend class PeriodicPluginBase;

        T265Odometry();
        T265Odometry(T265Odometry& t265_odometry);
        virtual ~T265Odometry();

        operator T265OdometryMsg() const override;
        T265Odometry& operator=(const T265OdometryMsg& t265_odometr_msg) override;
        T265Odometry& operator=(const T265Odometry& t265_odometry);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

        Eigen::Vector3d position_{Eigen::Vector3d::Zero()};
        Eigen::Quaterniond orientation_{Eigen::Quaterniond::Identity()};
        Eigen::Vector3d orientation_rpy_{Eigen::Vector3d::Zero()};

        Eigen::Vector3d linear_velocity_{Eigen::Vector3d::Zero()};
        Eigen::Vector3d angular_velocity_{Eigen::Vector3d::Zero()};
    private:
		virtual void setDataFromWrapperBase(WrapperBase* wrapper_base) override;
    };
} // namespace dls

#endif