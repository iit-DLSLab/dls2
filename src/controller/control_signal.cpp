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
#include "dls2/controller/control_signal.hpp"

using namespace dls;
ControlSignal::ControlSignal() :
	torques(),
	signal_reconstruction_method(SignalReconstructionMethod::ZERO_ORDER_HOLD),
	time()
{ }

ControlSignal::ControlSignal(ControlSignalMsg msg) :
	// TODO this 12 should not be hardcoded
	torques(Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(msg.torques().data(), 12)),
	signal_reconstruction_method((SignalReconstructionMethod)msg.signal_reconstruction_method()),
	time(msg.header().time().seconds())
{ }

ControlSignal::operator ControlSignalMsg() const
{
	// TODO do not assign this here, do not resize this here
	ControlSignalMsg msg;

	// TODO this 12 should not be hardcoded
	msg.torques().resize(12);
	Eigen::VectorXd::Map(&msg.torques()[0], 12) = this->torques;

	msg.signal_reconstruction_method((uint64_t)this->signal_reconstruction_method);
	msg.header().time().seconds()=this->time;
	return msg;
}
