
#ifndef SLIP_FLAG_HPP
#define SLIP_FLAG_HPP


#include "robotlib/robot_base.hpp"

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/slip_flag.h"
#include "dls_messages/dds/slip_flagPubSubTypes.h"

namespace dls
{
	class SlipFlag : public Wrapper<SlipFlagMsg>
	{
	public:
		SlipFlag(const std::shared_ptr<robotlib::RobotBase>);
		SlipFlag(SlipFlag&);
		SlipFlag() = delete;
        ~SlipFlag();

		operator SlipFlagMsg() const override;
		SlipFlag& operator= (const SlipFlagMsg&) override;

		std::string robot_name;

		robotlib::LegDataMap<bool> slip_flag;

		double time;
	};
} // end namespace dls

#endif /* end of include guard: SLIP_FLAG_HPP */
