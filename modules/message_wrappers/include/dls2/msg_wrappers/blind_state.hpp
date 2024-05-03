
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
	    template <typename SignalType>
		friend class SignalWriter;
		friend class Plugin;

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

		robotlib::LegDataMap<bool> feet_contact_;
	
	private:
		virtual void setDataFromWrapperBase(WrapperBase* wrapper_base) override;
	};
} // namespace dls

#endif