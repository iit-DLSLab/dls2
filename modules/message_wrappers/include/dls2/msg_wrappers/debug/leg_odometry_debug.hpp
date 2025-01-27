
#ifndef LEG_ODOMETRY_DEBUG_HPP
#define LEG_ODOMETRY_DEBUG_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/leg_odometry_debug.hpp"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class LegOdometryDebug : public Wrapper<LegOdometryDebugMsg>
	{
	public:
        template <typename SignalType>
        friend class SignalWriter;
        friend class Plugin;

		LegOdometryDebug(const std::shared_ptr<robotlib::RobotBase> robot);
		LegOdometryDebug(LegOdometryDebug& leg_odometry_debug);
		LegOdometryDebug() = delete;
        virtual ~LegOdometryDebug();

		operator LegOdometryDebugMsg() const override;
		LegOdometryDebug& operator= (const LegOdometryDebugMsg& leg_odometry_debug_msg) override;
        LegOdometryDebug& operator=(const LegOdometryDebug& leg_odometry_debug);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

        //GROUND TRUTH POSE FROM GAZEBO
        Eigen::Vector3d position_ground_truth_{};
        Eigen::Quaterniond orientation_ground_truth_{};

        //GROUND TRUTH VELOCITY FROM GAZEBO
        Eigen::Vector3d linear_velocity_ground_truth_{};
        Eigen::Vector3d angular_velocity_ground_truth_{};

        // MEAN
        Eigen::Vector3d position_mean_{};
        Eigen::Quaterniond orientation_mean_{};
        Eigen::Vector3d linear_velocity_mean_{};
        Eigen::Vector3d angular_velocity_mean_{};

        //ESTIMATION ERROR
        Eigen::Vector3d position_error_{};
        Eigen::Quaterniond orientation_error_{};
        Eigen::Vector3d linear_velocity_error_{};
        Eigen::Vector3d angular_velocity_error_{};

        //LEG ODOMETRY POSE
        robotlib::LegDataMap<Eigen::Vector3d> position_legs_;
        robotlib::LegDataMap<Eigen::Quaterniond> orientation_legs_;

        //LEG ODOMETRY VELOCITY
        robotlib::LegDataMap<Eigen::Vector3d> linear_velocity_legs_;
        robotlib::LegDataMap<Eigen::Vector3d> angular_velocity_legs_;

        //LEG ODOMETRY ERROR
        robotlib::LegDataMap<Eigen::Vector3d> linear_velocity_error_legs_;
        robotlib::LegDataMap<Eigen::Vector3d> angular_velocity_error_legs_;
    private:
        virtual void setDataFromWrapperBase(WrapperBase *wrapper_base) override;
    };
} // namespace dls

#endif