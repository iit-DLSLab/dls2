#ifndef LEGS_POSE_HPP
#define LEGS_POSE_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/legs_pose.h"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class LegsPose : public Wrapper<LegsPoseMsg>
	{
	public:
		LegsPose(const std::shared_ptr<robotlib::RobotBase> robot);
		LegsPose(LegsPose& legs_pose);
		LegsPose() = delete;
        virtual ~LegsPose();

		operator LegsPoseMsg() const override;
		LegsPose& operator=(const LegsPoseMsg& legs_pose_msg) override;
		LegsPose& operator=(const LegsPose& legs_pose);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

		robotlib::LegDataMap<Eigen::Vector3d> linear_velocity_;
		robotlib::LegDataMap<Eigen::Vector3d> angular_velocity_;

		Eigen::Vector3d base_velocity_{Eigen::Vector3d::Zero()};

		robotlib::LegDataMap<bool> stance_status_;
	};
} // namespace dls

#endif