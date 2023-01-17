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
#ifndef STANCE_LEGS_HPP
#define STANCE_LEGS_HPP


#include "robotlib/robot_base.hpp"

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls2/msg_wrappers/pose.hpp"
#include "dls2/msg_wrappers/screw.hpp"
#include "dls_messages/dds/stance_legs.h"

namespace dls
{
	class StanceLegs : public Wrapper<StanceLegsMsg>
	{
	public:
		StanceLegs(const std::shared_ptr<robotlib::RobotBase>);
		StanceLegs(StanceLegs&);
		StanceLegs() = delete;
        ~StanceLegs();

		operator StanceLegsMsg() const override;
		StanceLegs& operator= (StanceLegsMsg&) override;

		std::string robot_name;

		robotlib::LegDataMap<bool> stance_legs;

		unsigned long long time;
	};
} // end namespace dls

#endif /* end of include guard: STANCE_LEGS_HPP */
