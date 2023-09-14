
#include "dls2/msg_wrappers/control_signal.hpp"

ControlSignal::ControlSignal(const std::shared_ptr<robotlib::RobotBase> robot) 
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, torques_(robot->makeJointState(0.0))
	, signal_reconstruction_method_(SignalReconstructionMethod::ZERO_ORDER_HOLD)
{}

ControlSignal::ControlSignal(const ControlSignal& control_signal) 
    : frame_id_(control_signal.frame_id_)
	, sequence_id_(control_signal.sequence_id_)
	, timestamp_(control_signal.timestamp_)
    , torques_(control_signal.torques_)
	, signal_reconstruction_method_(control_signal.signal_reconstruction_method_)
{}

ControlSignal::~ControlSignal() {}

ControlSignal::operator ControlSignalMsg() const
{
	ControlSignalMsg control_signal_msg;

	control_signal_msg.frame_id(frame_id_);
	control_signal_msg.sequence_id(sequence_id_);
	control_signal_msg.timestamp(timestamp_);

	int i{0};
	for(auto &leg_pair : torques_)
	{
		for(auto &joint_pair : *leg_pair.data_)
		{
			control_signal_msg.torques()[i++] = *joint_pair.data_;
		}
	}
	
	control_signal_msg.signal_reconstruction_method((uint64_t)signal_reconstruction_method_);

	return control_signal_msg;
}

ControlSignal& ControlSignal::operator=(const ControlSignalMsg& control_signal_msg)
{
	frame_id_ = control_signal_msg.frame_id();
	sequence_id_ = control_signal_msg.sequence_id();
	timestamp_ = control_signal_msg.timestamp();

	signal_reconstruction_method_ = (dls::ControlSignal::SignalReconstructionMethod) control_signal_msg.signal_reconstruction_method();
	
	int i{0};
	for(auto &leg_pair : torques_)
	{
		for(auto &joint_pair : *leg_pair.data_)
		{
			*joint_pair.data_ = control_signal_msg.torques()[i++];
		}
	}

    return *this;
}

ControlSignal& ControlSignal::operator=(const ControlSignal& control_signal)
{ 
    frame_id_ = control_signal.frame_id_;
	sequence_id_ = control_signal.sequence_id_;
	timestamp_ = control_signal.timestamp_;

	torques_ = control_signal.torques_;
	signal_reconstruction_method_ = control_signal.signal_reconstruction_method_;
	
	return *this;
}