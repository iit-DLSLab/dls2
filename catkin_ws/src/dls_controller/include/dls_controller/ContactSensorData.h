/*
 * FTSensorData.h
 *
 *  Created on: November 7, 2016
 *      Author: Carlos Mastalli
 *
 */
#ifndef CONTROLLER_CONTACTSENSORDATA_H_
#define CONTROLLER_CONTACTSENSORDATA_H_

class ContactSensorData
{
public:
    ContactSensorData(int numberOfSensors);
    ~ContactSensorData() {};

    std::vector<bool> contact_;
};

inline ContactSensorData::ContactSensorData(int numberOfSensors)
{
    contact_.resize(numberOfSensors);
    for(int i = 0; i != numberOfSensors; i++)
        contact_[i] = false;
}

#endif
