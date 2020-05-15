#ifndef IMU_KVH_INTERFACE_H
#define IMU_KVH_INTERFACE_H

#include <hardware_interface/internal/hardware_resource_manager.h>
#include <string>
#include <Eigen/Dense>

namespace hardware_interface
{

class ImuKvhHandle
{
public:
	struct Data
	{
		Data():
			name(),
			angular_velocity(nullptr),
			specific_force(nullptr),
			time_stamp(nullptr),
			seq(nullptr),
			status(nullptr),
			time_sync(nullptr)
		{}

		std::string name;
		float *angular_velocity;
		float *specific_force;
		uint32_t *time_stamp;
		uint8_t *seq;
		uint8_t *status;
		uint16_t *time_sync;
	};

	ImuKvhHandle(const Data &data = Data()) :
		name_(data.name),
		angular_velocity_(data.angular_velocity),
		specific_force_(data.specific_force),
		time_stamp_(data.time_stamp),
		seq_(data.seq),
		status_(data.status),
		time_sync_(data.time_sync)
	{}

	std::string getName() const { return name_; }
	const float *getAngularVelocity() const { return angular_velocity_; }
	const float *getSpecificForce() const { return specific_force_; }
	const uint32_t *getTimeStamp() const { return time_stamp_; }
	const uint8_t *getSeq() const {return seq_; }
	const uint8_t *getStatus() const { return status_; }
	const uint16_t *getTimeSync() const { return time_sync_; }

private:
	std::string name_;
  float *angular_velocity_;
  float *specific_force_;
  uint32_t *time_stamp_;
  uint8_t *seq_;
  uint8_t *status_;
  uint16_t *time_sync_;

};

class ImuKvhInterface : public HardwareResourceManager<ImuKvhHandle> {};

}

#endif // IMU_KVH_INTERFACE_H
