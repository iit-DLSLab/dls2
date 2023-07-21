
#ifndef BASE_STATE_HPP
#define BASE_STATE_HPP

#include "robotlib/robot_base.hpp"

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls2/msg_wrappers/pose.hpp"
#include "dls2/msg_wrappers/screw.hpp"
#include "dls_messages/dds/base_state.h"

namespace dls
{
	class BaseState : public Wrapper<BaseStateMsg>
	{
	public:
		BaseState();
		BaseState(BaseState&);
        ~BaseState();

		operator BaseStateMsg() const override;
		BaseState& operator= (const BaseStateMsg&) override;

		std::string robot_name;

		dls::Pose pose;
		dls::Screw velocity;
		dls::Screw acceleration;

		unsigned long long time;
	};
} // end namespace dls

#endif /* end of include guard: BASE_STATE_HPP */
