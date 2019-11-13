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
#include "controller/control_signal.hpp"
ControlSignal::ControlSignal() :
	torques(),
	signal_reconstruction_method(SignalReconstructionMethod::ZERO_ORDER_HOLD)
{ }

ControlSignal::ControlSignal(ControlSignalMsg msg) :
	torques(Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(msg.torques().data(), msg.torques().size())),
	signal_reconstruction_method((SignalReconstructionMethod)msg.signal_reconstruction_method())
{ }

ControlSignal::operator ControlSignalMsg() const
{
	ControlSignalMsg msg;

	msg.torques().resize(this->torques.size());
	Eigen::VectorXd::Map(&msg.torques()[0], this->torques.size()) = this->torques;

	msg.signal_reconstruction_method((uint64_t)this->signal_reconstruction_method);

	return msg;
}
