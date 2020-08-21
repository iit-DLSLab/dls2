#ifndef HYQ_RAW_INTERFACE_H
#define HYQ_RAW_INTERFACE_H

#include <hardware_interface/internal/hardware_resource_manager.h>
#include <string>
#include <Eigen/Dense>
#include "imu_kvh_interface.h"
#include "imu_mgx_interface.h"

namespace hardware_interface
{

class HyqRawHandle
{
public:
	struct Data
	{
		Data():
			name(),
      		imu_mgx(nullptr),
      		imu_kvh(nullptr),
			abs_enc(nullptr),
			rel_enc(nullptr),
			torque_sensor_haa(nullptr),
			load_cell_hfe(nullptr),
			load_cell_kfe(nullptr)
		{}

		std::string name;
    	ImuMgxHandle::Data *imu_mgx;
    	ImuKvhHandle::Data *imu_kvh;
		uint32_t *abs_enc;
		uint32_t *rel_enc;
		uint16_t *torque_sensor_haa;
		uint16_t *load_cell_hfe;
		uint16_t *load_cell_kfe;
	};

	HyqRawHandle(const Data &data = Data()) :
		name_(data.name),
		imu_mgx_(data.imu_mgx),
		imu_kvh_(data.imu_kvh),
		abs_enc_(data.abs_enc),
		rel_enc_(data.rel_enc),
		torque_sensor_haa_(data.torque_sensor_haa),
		load_cell_hfe_(data.load_cell_hfe),
		load_cell_kfe_(data.load_cell_kfe)
	{}

	std::string getName() const { return name_; }
	const ImuMgxHandle::Data *getImuMgx() const { return imu_mgx_; }
	const ImuKvhHandle::Data *getImuKvh() const { return imu_kvh_; }
	const uint32_t *getAbsEnc() const { return abs_enc_; }
	const uint32_t *getRelEnc() const {return rel_enc_; }
	const uint16_t *getTorqueSensorHAA() const { return torque_sensor_haa_; }
	const uint16_t *getLoadCellHFE() const { return load_cell_hfe_; }
	const uint16_t *getLoadCellKFE() const { return load_cell_hfe_; }

private:
	std::string name_;
	ImuMgxHandle::Data *imu_mgx_;
	ImuKvhHandle::Data *imu_kvh_;
	uint32_t *abs_enc_;
	uint32_t *rel_enc_;
	uint16_t *torque_sensor_haa_;
	uint16_t *load_cell_hfe_;
	uint16_t *load_cell_kfe_;
};

class HyqRawInterface : public HardwareResourceManager<HyqRawHandle> {};

}

#endif // HYQ_RAW_INTERFACE_H
