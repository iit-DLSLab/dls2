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
		BlindState(const std::shared_ptr<robotlib::RobotBase>&);
		BlindState(BlindState&);
		BlindState() = delete;
        ~BlindState();

		operator BlindStateMsg() const override;
		BlindState& operator= (BlindStateMsg&) override;

		robotlib::JointState joint_position;
		robotlib::JointState joint_velocity;
		robotlib::JointState joint_acceleration;
		robotlib::JointState joint_effort;

		// /// Angular velocity
		// ///
		// Eigen::Vector3d angular_velocity;

		// /// Angular acceleration
		// ///
		// Eigen::Vector3d angular_acceleration;

		// /// Orientation
		// ///
		// Eigen::Quaternion<double> orientation;

		// Pose base_pose_world;
		// Screw base_velocity_world;
		// Screw base_acceleration_world;

		double time;
	};
} // end namespace dls

#endif /* end of include guard: BLIND_STATE_SIGNAL_HPP */
