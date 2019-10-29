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
	enum class SignalReconstructionMethod
	{
		ZERO_ORDER_HOLD,
		// IMPULSE
	} signalReconstructionMethod;
};

#endif /* end of include guard: CONTROL_SIGNAL_HPP_QCFRROHM */
