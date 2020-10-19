#ifndef GAZEBO_ROS_CONTROL___DLS_GAZEBO_IMU_KVH_H
#define GAZEBO_ROS_CONTROL___DLS_GAZEBO_IMU_KVH_H

#include <dls2_msgs/ImuKvh.h>
#include <dls2_hardware_interface/imu_kvh_interface.h>


namespace dls_hw_sim
{

	class ImuKvhRosInterface : public RosInterface<dls2_msgs::ImuKvh> {
	public:

		virtual void initMsg() override { msg_.seq = 0;}

		virtual void fill() override {
			msg_.angular_velocity[0] = sim_model_->GetWorldAngularVel().x;
			msg_.angular_velocity[1] = sim_model_->GetWorldAngularVel().y;
			msg_.angular_velocity[2] = sim_model_->GetWorldAngularVel().z;
			msg_.specific_force[0] = sim_model_->GetWorldLinearAccel().x;
			msg_.specific_force[2] = sim_model_->GetWorldLinearAccel().y;
			msg_.specific_force[1] = sim_model_->GetWorldLinearAccel().z+9.81;
			msg_.seq++;
			msg_.status=119;
			msg_.time_sync=0;
		}

	};

	class DlsGazeboImuKvh : public GazeboHardwareInterface {
	public:
		DlsGazeboImuKvh() : ros(nullptr) {
			data_.name = "imu_kvh";
			data_.angular_velocity = &angular_velocity_[0];
			data_.specific_force = &specific_force_[0];
			data_.time_stamp = &time_stamp_;
			data_.seq = &seq_;
			data_.status = &status_;
			data_.time_sync = &time_sync_;
		}

		~DlsGazeboImuKvh() { if (ros) delete(ros); }

		void init(gazebo::physics::ModelPtr sim_model, ros::NodeHandle &model_nh) {
			GazeboHardwareInterface::init(sim_model);
			ros = new ImuKvhRosInterface();
			ros->init(sim_model_,model_nh,"imu_kvh",1);
			add_interface(ros);
		}

		

		void fillImuKvhInterface(ros::Time time)
		{
			angular_velocity_[0] = sim_model_->GetWorldAngularVel().x;
			angular_velocity_[1] = sim_model_->GetWorldAngularVel().y;
			angular_velocity_[2] = sim_model_->GetWorldAngularVel().z;
			specific_force_[0] = sim_model_->GetWorldLinearAccel().x;
			specific_force_[2] = sim_model_->GetWorldLinearAccel().y;
			specific_force_[1] = sim_model_->GetWorldLinearAccel().z+9.81;
			seq_++;
			status_=119;
			time_sync_=0;
		}



	public:
		hardware_interface::ImuKvhHandle::Data data_;
	private:
		ImuKvhRosInterface *ros;		
		std::string				name_;
		std::array<float,3> 	angular_velocity_;
		std::array<float,3> 	specific_force_;
		uint32_t 				time_stamp_;
		uint8_t 				seq_;
		uint8_t 				status_;
		uint16_t 				time_sync_;


	};

}

#endif