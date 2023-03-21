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
#ifndef BLIND_STATE_SIGNAL_HPP
#define BLIND_STATE_SIGNAL_HPP


#include "robotlib/robot_base.hpp"

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls2/msg_wrappers/pose.hpp"
#include "dls2/msg_wrappers/screw.hpp"
#include "dls_messages/dds/blind_state.h"

namespace dls
{
	class BlindState : public Wrapper<BlindStateMsg>
	{
	public:
		BlindState(const std::shared_ptr<robotlib::RobotBase>);
		BlindState(BlindState&);
		BlindState() = delete;
        ~BlindState();

		operator BlindStateMsg() const override;
		BlindState& operator= (const BlindStateMsg&) override;

		std::string robot_name;
		robotlib::JointDataMap<std::string> joint_name;
		robotlib::JointState joint_position;
		robotlib::JointState joint_velocity;
		robotlib::JointState joint_acceleration;
		robotlib::JointState joint_effort;
		robotlib::JointState joint_temperature;

		// FEET
		robotlib::LegDataMap<Eigen::Vector3d> foot_position;
		robotlib::LegDataMap<Eigen::Vector3d> foot_velocity;
		robotlib::LegDataMap<Eigen::Vector3d> foot_acceleration;

		dls::Pose base_pose_world;
		dls::Screw base_vel_world;
		dls::Screw base_acc_world;

		robotlib::LegDataMap<bool> stance_legs;

		unsigned long long time;
	};
} // end namespace dls

#endif /* end of include guard: BLIND_STATE_SIGNAL_HPP */
