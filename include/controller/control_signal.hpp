#ifndef CONTROL_SIGNAL_HPP_QCFRROHM
#define CONTROL_SIGNAL_HPP_QCFRROHM

#include <Eigen/Dense>

/// A struct representing the control signal that is output by a Controller
struct ControlSignal
{
	TODO("Zero this")
	Eigen::VectorXd torques; ///< The torque vector to the joints

	/// How the control layer should interpret the torques from the controller
	/// when summing the torques between multiple controllers
	// if the inherited type is changed, change it also in the idl file
	enum class SignalReconstructionMethod : uint64_t
	{
		ZERO_ORDER_HOLD,
		// IMPULSE
	} signal_reconstruction_method = SignalReconstructionMethod::ZERO_ORDER_HOLD;
};

#endif /* end of include guard: CONTROL_SIGNAL_HPP_QCFRROHM */
