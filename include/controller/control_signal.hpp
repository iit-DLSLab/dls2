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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef CONTROL_SIGNAL_HPP_QCFRROHM
#define CONTROL_SIGNAL_HPP_QCFRROHM

#include <Eigen/Dense>
#include "msg/control_signalPubSubTypes.h"
#include "todo.h"

/// A struct representing the control signal that is output by a Controller
namespace dls
{
TODO("Should this be made thread safe?")
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
	} signal_reconstruction_method;
	double time;
	
	// =============================== Fastrtps ================================
	TODO("Figure out if this should be const or reference or whatever")
	ControlSignal();
	ControlSignal(ControlSignalMsg);
	operator ControlSignalMsg() const;
};
} // end namespace dls
#endif /* end of include guard: CONTROL_SIGNAL_HPP_QCFRROHM */
