#include "fastrtps_wrappers/blind_state_signal.hpp"

// =============================================================================
// Constructors
// =============================================================================
BlindStateSignal::BlindStateSignal() :
	joint_position(),
	joint_velocity(),
	joint_effort()
{
	joint_position.resize(12);
	joint_velocity.resize(12);
	joint_effort.resize(12);
}

// =============================================================================
// RTPS Util
// =============================================================================
// -----------------------------------------------------------------------------
// Converting Constructor
// -----------------------------------------------------------------------------

BlindStateSignal::BlindStateSignal(BlindStateMsg msg) :
	joint_position
	(
		Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>
		(
			msg.joint_state().position().data(), msg.joint_state().position().size()
		)
	),
	joint_velocity
	(
			Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>
			(
					msg.joint_state().velocity().data(), msg.joint_state().velocity().size()
			)
	),
	joint_effort
	(
			Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>
			(
					msg.joint_state().effort().data(), msg.joint_state().effort().size()
			)
	)
{
	(void)msg;
}

// -----------------------------------------------------------------------------
// Type Casting
// -----------------------------------------------------------------------------
BlindStateSignal::operator BlindStateMsg() const
{
    BlindStateMsg msg;

    return msg;
}
