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
#ifndef LEGS_POSE_SIGNAL_HPP
#define LEGS_POSE_SIGNAL_HPP


#include "robotlib/robot_base.hpp"

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/legs_pose.h"

namespace dls
{
	class LegsPose : public Wrapper<LegsPoseMsg>
	{
	public:
		LegsPose(const std::shared_ptr<robotlib::RobotBase>&);
		LegsPose(LegsPose&);
		LegsPose() = delete;
        ~LegsPose();

		operator LegsPoseMsg() const override;
		LegsPose& operator= (LegsPoseMsg&) override;

		robotlib::LegDataMap<Eigen::Vector3d> lin_velocity;
		robotlib::LegDataMap<Eigen::Vector3d> ang_velocity;

		double time;
	};
} // end namespace dls

#endif /* end of include guard: LEGS_POSE_SIGNAL_HPP */
