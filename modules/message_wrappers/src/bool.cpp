
#include "dls2/msg_wrappers/bool.hpp"

Bool::Bool()
	: frame_id_(""),
	  sequence_id_(0),
	  timestamp_(0.0),
	  data_(false)
{
}

Bool::Bool(const Bool &bool_data)
	: frame_id_(bool_data.frame_id_),
	  sequence_id_(bool_data.sequence_id_),
	  timestamp_(bool_data.timestamp_),
	  data_(bool_data.data_)
{
}

Bool::~Bool() {}

Bool::operator BoolMsg() const
{
	BoolMsg bool_msg;

	bool_msg.frame_id(frame_id_);
	bool_msg.sequence_id(sequence_id_);
	bool_msg.timestamp(timestamp_);

	bool_msg.data() = data_;

	return bool_msg;
}

Bool &Bool::operator=(const BoolMsg &bool_msg)
{
	frame_id_ = bool_msg.frame_id();
	sequence_id_ = bool_msg.sequence_id();
	timestamp_ = bool_msg.timestamp();

	data_ = bool_msg.data();

	return *this;
}

Bool &Bool::operator=(const Bool &bool_data)
{
	frame_id_ = bool_data.frame_id_;
	sequence_id_ = bool_data.sequence_id_;
	timestamp_ = bool_data.timestamp_;

	data_ = bool_data.data_;

	return *this;
}

void Bool::setDataFromWrapperBase(WrapperBase *wrapper_base)
{
	// Assuming wrapper is of type Wrapper<MsgType>
	*this = static_cast<Bool &>(*wrapper_base);
}