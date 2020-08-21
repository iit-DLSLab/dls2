#ifndef BLIND_STATE_INTERFACE_H
#define BLIND_STATE_INTERFACE_H

#include <hardware_interface/internal/hardware_resource_manager.h>
#include <string>
#include <Eigen/Dense>

namespace hardware_interface
{

class BlindStateHandle
{
public:
	struct Data
	{
		Data():
			name(),
			joint_position(nullptr),
			joint_velocity(nullptr),
			joint_effort(nullptr),
			base_pose_world(nullptr),
			base_velocity_world(nullptr),
			base_acceleration_world(nullptr)
		{}

		std::string name;
		double *joint_position;
		double *joint_velocity;
		double *joint_effort;
		double *base_pose_world;
		double *base_velocity_world;
		double *base_acceleration_world;
	};

	BlindStateHandle(const Data &data = Data()) :
		name_(data.name),
		joint_position_(data.joint_position),
		joint_velocity_(data.joint_velocity),
		joint_effort_(data.joint_effort),
		base_pose_world_(data.base_pose_world),
		base_velocity_world_(data.base_velocity_world),
		base_acceleration_world_(data.base_acceleration_world)
	{}

	std::string getName() const { return name_; }
	const double *getJointPosition() const { return joint_position_; }
	const double *getJointVelocity() const { return joint_velocity_; }
	const double *getJointEffort() const {return joint_effort_; }
	const double *getBasePoseWorld() const { return base_pose_world_; }
	const double *getBaseVelocityWorld() const { return base_velocity_world_; }
	const double *getBaseAccelerationWorld() const { return base_acceleration_world_; }

private:
	std::string name_;
	double *joint_position_;
	double *joint_velocity_;
	double *joint_effort_;
	double *base_pose_world_;
	double *base_velocity_world_;
	double *base_acceleration_world_;
};

class BlindStateInterface : public HardwareResourceManager<BlindStateHandle> {};

}

#endif // BLIND_STATE_INTERFACE_H
