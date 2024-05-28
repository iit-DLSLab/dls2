#include "dls2/msg_wrappers/joint_state.hpp"

JointState::JointState(const std::shared_ptr<robotlib::RobotBase> robot)
	: frame_id_(""),
	  sequence_id_(0),
	  timestamp_(0.0),
	  q_(robot->makeJointState(0.0)),
	  qd_(robot->makeJointState(0.0)),
	  qdd_(robot->makeJointState(0.0))
{
}

JointState::JointState(const JointState &joint_state)
	: frame_id_(joint_state.frame_id_),
	  sequence_id_(joint_state.sequence_id_),
	  timestamp_(joint_state.timestamp_),
	  q_(joint_state.q_),
	  qd_(joint_state.qd_),
	  qdd_(joint_state.qdd_)
{
}

JointState::~JointState() {}

JointState::operator JointStateMsg() const
{
	JointStateMsg joint_state_msg;

	joint_state_msg.frame_id(frame_id_);
	joint_state_msg.sequence_id(sequence_id_);
	joint_state_msg.timestamp(timestamp_);

	int i{0};
	for(auto &leg : q_)
	{
		for(auto &joint : *leg.data_)
		{
			joint_state_msg.q()[i] = q_[joint.key_]; 
			joint_state_msg.qd()[i] = qd_[joint.key_]; 
			joint_state_msg.qdd()[i] = qdd_[joint.key_]; 
			i++;
		}
	}

	return joint_state_msg;
}

JointState &JointState::operator=(const JointStateMsg &joint_state_msg)
{
	frame_id_ = joint_state_msg.frame_id();
	sequence_id_ = joint_state_msg.sequence_id();
	timestamp_ = joint_state_msg.timestamp();

	int i{0};
	for(auto &leg : q_)
	{
		for(auto &joint : *leg.data_)
		{
			q_[joint.key_] = joint_state_msg.q()[i];
			qd_[joint.key_] = joint_state_msg.qd()[i];
			qdd_[joint.key_] = joint_state_msg.qdd()[i];
			i++;
		}
	}

	return *this;
}

JointState &JointState::operator=(const JointState &joint_state)
{
	frame_id_ = joint_state.frame_id_;
	sequence_id_ = joint_state.sequence_id_;
	timestamp_ = joint_state.timestamp_;

	q_ = joint_state.q_;
	qd_ = joint_state.qd_;
	qdd_ = joint_state.qdd_;

	return *this;
}

void JointState::setDataFromWrapperBase(WrapperBase *wrapper_base)
{
	// Assuming wrapper is of type Wrapper<MsgType>
	*this = static_cast<JointState &>(*wrapper_base);
}