#ifndef IMU_MGX_INTERFACE_H
#define IMU_MGX_INTERFACE_H

#include <hardware_interface/internal/hardware_resource_manager.h>
#include <string>
#include <Eigen/Dense>

namespace hardware_interface
{

class ImuMgxHandle
{
public:
	struct Data
	{
		Data():
			name(),
			angular_velocity(nullptr),
			specific_force(nullptr),
			quaternion(nullptr),
			time_stamp(nullptr),
			temperature(nullptr)
		{}

		std::string name;
		float *angular_velocity;
		float *specific_force;
		float *quaternion;
		uint32_t *time_stamp;
		uint16_t *temperature;
	};

	ImuMgxHandle(const Data &data = Data()) :
		name_(data.name),
		angular_velocity_(data.angular_velocity),
		specific_force_(data.specific_force),
		quaternion_(data.quaternion),
		time_stamp_(data.time_stamp),
		temperature_(data.temperature)
	{}

	std::string getName() const { return name_; }
	const float *getAngularVelocity() const { return angular_velocity_; }
	const float *getSpecificForce() const { return specific_force_; }
	const float *getQuaternion() const { return quaternion_; }
	const uint32_t *getTimeStamp() const {return time_stamp_; }
	const uint16_t *getTemperature() const { return temperature_; }

private:
	std::string name_;
  float *angular_velocity_;
  float *specific_force_;
  float *quaternion_;
  uint32_t *time_stamp_;
  uint16_t *temperature_;

};

class ImuMgxInterface : public HardwareResourceManager<ImuMgxHandle> {};

}

#endif // IMU_MGX_INTERFACE_H
