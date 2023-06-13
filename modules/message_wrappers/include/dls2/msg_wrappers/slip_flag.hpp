/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef SLIP_FLAG_HPP
#define SLIP_FLAG_HPP


#include "robotlib/robot_base.hpp"

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls2/msg_wrappers/pose.hpp"
#include "dls2/msg_wrappers/screw.hpp"
#include "dls_messages/dds/slip_flag.h"

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

		unsigned long long time;
	};
} // end namespace dls

#endif /* end of include guard: SLIP_FLAG_HPP */
