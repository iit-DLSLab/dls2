
#ifndef BASE_STATE_HPP
#define BASE_STATE_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/base_state.h"
#include "dls2/msg_wrappers/pose.hpp"
#include "dls2/msg_wrappers/screw.hpp"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class BaseState : public Wrapper<BaseStateMsg>
	{
	public:
	    template <typename SignalType>
		friend class SignalWriter;
		friend class PluginBase;

		BaseState(const std::shared_ptr<robotlib::RobotBase> robot);
		BaseState(BaseState& base_state);
		BaseState() = delete;
        virtual ~BaseState();

		operator BaseStateMsg() const override;
		BaseState& operator=(const BaseStateMsg& base_state_msg) override;
        BaseState& operator=(const BaseState& base_state);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

		std::string robot_name_{};

		dls::Pose pose_{};
		dls::Screw velocity_{};
		dls::Screw acceleration_{};

		// Stance status
		robotlib::LegDataMap<bool> stance_status_;

	private:
		virtual void setDataFromWrapperBase(WrapperBase* wrapper_base) override;
	};
} // namespace dls

#endif