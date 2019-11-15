#ifndef ROSCONTROL_TO_DLS2_HPP_X8TSOKXU
#define ROSCONTROL_TO_DLS2_HPP_X8TSOKXU

#include "util/messaging/publisher_base.hpp"
#include "msg/blind_state.h"

namespace roscontrol_to_dls2
{
	class RoscontrolToDls2
	{
	public:
		RoscontrolToDls2();

	private:
		PublisherBase<BlindStateMsgPubSubType> state_pub;
	};
}

#endif /* end of include guard: ROSCONTROL_TO_DLS2_HPP_X8TSOKXU */
