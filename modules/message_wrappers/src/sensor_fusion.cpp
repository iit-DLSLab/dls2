#include "dls2/msg_wrappers/sensor_fusion.hpp"

SensorFusion::SensorFusion()
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
    , position_(Eigen::Vector3d::Zero())
    , linear_velocity_(Eigen::Vector3d::Zero())
{}

SensorFusion::SensorFusion(SensorFusion& sensor_fusion)
    : frame_id_(sensor_fusion.frame_id_)
	, sequence_id_(sensor_fusion.sequence_id_)
	, timestamp_(sensor_fusion.timestamp_)   
    , position_(sensor_fusion.position_)
    , linear_velocity_(sensor_fusion.linear_velocity_)
{}

SensorFusion::~SensorFusion(){}

SensorFusion::operator SensorFusionMsg() const
{
    SensorFusionMsg sensor_fusion_msg;

	sensor_fusion_msg.frame_id(frame_id_);
	sensor_fusion_msg.sequence_id(sequence_id_);
	sensor_fusion_msg.timestamp(timestamp_);

    for (unsigned int i{0}; i<3; i++)
    {
        sensor_fusion_msg.position()[i] = position_[i];
        sensor_fusion_msg.linear_velocity()[i] = linear_velocity_[i];
    }

    return sensor_fusion_msg;           
}

SensorFusion& SensorFusion::operator=(const SensorFusionMsg& sensor_fusion_msg)
{
	frame_id_ = sensor_fusion_msg.frame_id();
	sequence_id_ = sensor_fusion_msg.sequence_id();
	timestamp_ = sensor_fusion_msg.timestamp();

    for (unsigned int i{0}; i<3; i++)
    {
        position_[i] = sensor_fusion_msg.position()[i];
        linear_velocity_[i] = sensor_fusion_msg.linear_velocity()[i];
    }

    return *this;
}

SensorFusion& SensorFusion::operator=(const SensorFusion& sensor_fusion)
{
    frame_id_ = sensor_fusion.frame_id_;
	sequence_id_ = sensor_fusion.sequence_id_;
	timestamp_ = sensor_fusion.timestamp_;

    position_ = sensor_fusion.position_;
    linear_velocity_ = sensor_fusion.linear_velocity_;

    return *this;
}

void SensorFusion::setDataFromWrapperBase(WrapperBase *wrapper_base)
{
	// Assuming wrapper is of type Wrapper<MsgType>
	*this = static_cast<SensorFusion &>(*wrapper_base);
}