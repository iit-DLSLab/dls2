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
#include "dls2/controller/control_signal.hpp"

using namespace dls;
ControlSignal::ControlSignal(const std::shared_ptr<robotlib::RobotBase> &pRobot) 
    : torques(pRobot->makeJointState())
	, signal_reconstruction_method(SignalReconstructionMethod::ZERO_ORDER_HOLD)
	, time()
{ }

ControlSignal::operator ControlSignalMsg() const
{
	ControlSignalMsg msg;

	for(auto &leg_pair : this->torques)
	{
		for(auto &joint_pair : *leg_pair.data_)
		{
			msg.torques().push_back(*joint_pair.data_);
		}
	}
	
	msg.signal_reconstruction_method((uint64_t)this->signal_reconstruction_method);
	msg.header().time().seconds(this->time);
	return msg;
}


ControlSignal & ControlSignal::operator=(const ControlSignalMsg &msg){

    this->time = msg.header().time().seconds();
	this->signal_reconstruction_method = (dls::ControlSignal::SignalReconstructionMethod) msg.signal_reconstruction_method();
	this->torques = msg.torques();

    return *this;
}
