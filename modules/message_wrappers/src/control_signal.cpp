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
#include "dls2/msg_wrappers/control_signal.hpp"

using namespace dls;
ControlSignal::ControlSignal(const std::shared_ptr<robotlib::RobotBase> &pRobot) 
    : torques(pRobot->makeJointState())
	, signal_reconstruction_method(SignalReconstructionMethod::ZERO_ORDER_HOLD)
	, time()
{ }

ControlSignal::operator ControlSignalMsg() const
{
	ControlSignalMsg msg;

	int i = 0;
	for(auto &leg_pair : this->torques)
	{
		for(auto &joint_pair : *leg_pair.data_)
		{
			msg.torques()[i++] = *joint_pair.data_;
		}
	}
	
	msg.signal_reconstruction_method((uint64_t)this->signal_reconstruction_method);
	return msg;
}


ControlSignal & ControlSignal::operator=(const ControlSignalMsg &msg){

	this->signal_reconstruction_method = (dls::ControlSignal::SignalReconstructionMethod) msg.signal_reconstruction_method();
	
	int i = 0;
	for(auto &leg_pair : this->torques)
	{
		for(auto &joint_pair : *leg_pair.data_)
		{
			*joint_pair.data_ = msg.torques()[i++];
		}
	}

    return *this;
}
