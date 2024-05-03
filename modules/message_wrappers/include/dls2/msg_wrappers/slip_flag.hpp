
#ifndef SLIP_FLAG_HPP
#define SLIP_FLAG_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/slip_flag.h"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class SlipFlag : public Wrapper<SlipFlagMsg>
	{
	public:
		template <typename SignalType>
		friend class SignalWriter;
		friend class PluginBase;

		SlipFlag(const std::shared_ptr<robotlib::RobotBase> robot);
		SlipFlag(SlipFlag& slip_flag);
		SlipFlag() = delete;
        virtual ~SlipFlag();

		operator SlipFlagMsg() const override;
		SlipFlag& operator=(const SlipFlagMsg& slip_flag_msg) override;
		SlipFlag& operator=(const SlipFlag& slip_flag);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

		std::string robot_name_{};
		robotlib::LegDataMap<bool> slip_flag_;
    private:
        virtual void setDataFromWrapperBase(WrapperBase *wrapper_base) override;
	};
} // namespace dls

#endif