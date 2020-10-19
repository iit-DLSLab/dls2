#ifndef GAZEBO_ROS_CONTROL___DLS_GAZEBO_IMU_SENSOR_H
#define GAZEBO_ROS_CONTROL___DLS_GAZEBO_IMU_SENSOR_H

#include <hardware_interface/imu_sensor_interface.h>
#include <sensor_msgs/Imu.h>


namespace dls_hw_sim
{

	class ImuSensorRosInterface : public RosInterface<sensor_msgs::Imu> {
	public:
		ImuSensorRosInterface() {} // IMU ptr?

		virtual void initMsg() override {}
		virtual void fill() override {}
	private:
		//IMU ptr
	};

	class DlsGazeboImuSensor : public GazeboHardwareInterface {
	public:
		DlsGazeboImuSensor() : ros(nullptr) {
			data_.angular_velocity = &angular_velocity_[0];
			data_.angular_velocity_covariance =  &angular_velocity_covariance_[0];
			data_.linear_acceleration = &linear_acceleration_[0];
			data_.linear_acceleration_covariance = &linear_acceleration_covariance_[0];
			data_.orientation = &orientation_[0];
			data_.orientation_covariance = &orientation_covariance_[0];
			
			data_.name = "trunk_imu"; // TODO: Fetch from elsewhere?
			data_.frame_id = "trunk_imu"; // TODO: Fetch from URDF?

		}

		~DlsGazeboImuSensor() { if (ros) delete(ros); }

		void init(gazebo::physics::ModelPtr sim_model, ros::NodeHandle &model_nh) {
			sim_model_=sim_model;
			ros = new ImuSensorRosInterface();//imu ptr
			ros->init(sim_model_,model_nh,"imu_sensor",1);
		}


	public:
		hardware_interface::ImuSensorHandle::Data data_;
	private:
		//imu ptr
		ImuSensorRosInterface *ros;
		
		std::string imu_name_;
		std::array<double,3> orientation_;
		std::array<double,9> orientation_covariance_;
		std::array<double,3> angular_velocity_;
		std::array<double,9> angular_velocity_covariance_;
		std::array<double,3> linear_acceleration_;
		std::array<double,9> linear_acceleration_covariance_;


	};

}

#endif