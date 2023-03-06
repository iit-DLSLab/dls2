#ifndef TRANSLATION_CPP
#define TRANSLATION_CPP

#include "dls2/msg_wrappers/translation.hpp"

using namespace dls;

SensorFusion::SensorFusion()
{ }

// SensorFusion::SensorFusion(const std::shared_ptr<robotlib::RobotBase>& pRobot)
// 	: position(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
// 	, linear_velocity(pRobot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
// 	, timestamp(0.0)
// { }

SensorFusion::SensorFusion(SensorFusion& from)
    : position(from.position)
    , linear_velocity(from.linear_velocity)
    , timestamp(from.timestamp)
{ }

SensorFusion::operator SensorFusionMsg() const
{
    SensorFusionMsg msg;

    for (long unsigned int i = 0; i < msg.position().size(); i++)
    {
        msg.position()[i] = this->position[i];
        msg.linear_velocity()[i] = this->linear_velocity[i];
    }

    msg.timestamp(this->timestamp);

    return msg;           
}

SensorFusion& SensorFusion::operator=(const SensorFusionMsg& msg){

    for (long unsigned int i = 0; i < msg.position().size(); i++)
    {
        this->position[i] = msg.position()[i];
        this->linear_velocity[i] = msg.linear_velocity()[i];
    }

    this->timestamp = msg.timestamp();

    return *this;
}

SensorFusion& SensorFusion::operator=(const SensorFusion& from)
{
    this->position = from.position;
    this->linear_velocity = from.linear_velocity;
    this->timestamp = from.timestamp;

    return *this;
}

#endif // #ifndef TRANSLATION_CPP