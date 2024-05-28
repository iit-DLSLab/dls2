
#include "dls2/msg_wrappers/vector3d.hpp"

Vector3d::Vector3d()
	: frame_id_(""),
	  sequence_id_(0),
	  timestamp_(0.0),
	  data_(Eigen::Vector3d::Zero())
{
}

Vector3d::Vector3d(const Vector3d &vector3d)
	: frame_id_(vector3d.frame_id_),
	  sequence_id_(vector3d.sequence_id_),
	  timestamp_(vector3d.timestamp_),
	  data_(vector3d.data_)
{
}

Vector3d::~Vector3d() {}

Vector3d::operator Vector3dMsg() const
{
	Vector3dMsg vector3d_msg;

	vector3d_msg.frame_id(frame_id_);
	vector3d_msg.sequence_id(sequence_id_);
	vector3d_msg.timestamp(timestamp_);

	vector3d_msg.data()[0] = data_(0);
	vector3d_msg.data()[1] = data_(1);
	vector3d_msg.data()[2] = data_(2);

	return vector3d_msg;
}

Vector3d &Vector3d::operator=(const Vector3dMsg &vector3d_msg)
{
	frame_id_ = vector3d_msg.frame_id();
	sequence_id_ = vector3d_msg.sequence_id();
	timestamp_ = vector3d_msg.timestamp();

	data_(0) = vector3d_msg.data()[0];
	data_(1) = vector3d_msg.data()[1];
	data_(2) = vector3d_msg.data()[2];

	return *this;
}

Vector3d &Vector3d::operator=(const Vector3d &vector3d)
{
	frame_id_ = vector3d.frame_id_;
	sequence_id_ = vector3d.sequence_id_;
	timestamp_ = vector3d.timestamp_;

	data_ = vector3d.data_;

	return *this;
}

void Vector3d::setDataFromWrapperBase(WrapperBase *wrapper_base)
{
	// Assuming wrapper is of type Wrapper<MsgType>
	*this = static_cast<Vector3d &>(*wrapper_base);
}