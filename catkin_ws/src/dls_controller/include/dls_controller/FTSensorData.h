/*
 * FTSensorData.h
 *
 *  Created on: Sept 23, 2015
 *      Author: Felipe Polido
 *
 */
#ifndef CONTROLLER_FTSENSORDATA_H_
#define CONTROLLER_FTSENSORDATA_H_

class FTSensorData{

	public:

	FTSensorData(int numberOfSensors);

	~FTSensorData() {};

	std::vector<Eigen::Vector3d> force;
	std::vector<Eigen::Vector3d> torque;
    std::vector<Eigen::Vector3d> normal;
};

inline FTSensorData::FTSensorData(int numberOfSensors)
{
	for(int i = 0; i != numberOfSensors; i++)
	{
		force.push_back(Eigen::Vector3d::Zero());	
		torque.push_back(Eigen::Vector3d::Zero());	
        normal.push_back(Eigen::Vector3d::Zero());
	}

}

#endif
