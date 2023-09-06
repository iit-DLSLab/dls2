
#ifndef BLIND_STATE_HPP
#define BLIND_STATE_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/blind_state.h"
#include "dls2/msg_wrappers/pose.hpp"
#include "dls2/msg_wrappers/screw.hpp"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class BlindState : public Wrapper<BlindStateMsg>
	{
	public:
		BlindState(const std::shared_ptr<robotlib::RobotBase> robot);
		BlindState(BlindState& blind_state);
		BlindState() = delete;
		virtual ~BlindState();

		operator BlindStateMsg() const override;
		BlindState& operator=(const BlindStateMsg& blind_state_msg) override;
		BlindState& operator=(const BlindState& blind_state);

        std::string frame_id_{};
        uint32_t sequence_id_{};
        double timestamp_{};

		std::string robot_name_{};

		robotlib::JointDataMap<std::string> joints_name_;
		robotlib::JointState joints_position_;
		robotlib::JointState joints_velocity_;
		robotlib::JointState joints_acceleration_;
		robotlib::JointState joints_effort_;
		robotlib::JointState joints_temperature_;

		robotlib::LegDataMap<Eigen::Vector3d> feet_position_;
		robotlib::LegDataMap<Eigen::Vector3d> feet_velocity_;
		robotlib::LegDataMap<Eigen::Vector3d> feet_acceleration_;

		Eigen::Vector3d terrain_inclination_{Eigen::Vector3d::Zero()};
		Eigen::Vector3d surface_normal_{Eigen::Vector3d::Zero()};

		dls::Pose base_pose_world_{};
		Eigen::Vector3d base_orientation_world_rpy_{Eigen::Vector3d::Zero()};
		dls::Screw base_velocity_world_{};
		dls::Screw base_acceleration_world_{};

		robotlib::LegDataMap<bool> stance_legs_;
	};
} // namespace dls

#endif