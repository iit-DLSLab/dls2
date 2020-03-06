/*
 * ShinSensorData.h
 *
 *  Created on: 28 April 2017
 *      Author: Michele Focchi
 *
 */
#ifndef CONTROLLER_SHINSENSORDATA_H_
#define CONTROLLER_SHINSENSORDATA_H_

class ShinSensorData
{
public:
    ShinSensorData(int numberOfSensors);
    ~ShinSensorData() {};

    std::vector<double> contact_position_;
};

inline ShinSensorData::ShinSensorData(int numberOfSensors)
{
    contact_position_.resize(numberOfSensors);
    for(int i = 0; i != numberOfSensors; i++)
        contact_position_[i] = 0.0;
}

#endif
