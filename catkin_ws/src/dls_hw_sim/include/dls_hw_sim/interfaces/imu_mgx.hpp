#ifndef GAZEBO_ROS_CONTROL___DLS_GAZEBO_IMU_MGX_H
#define GAZEBO_ROS_CONTROL___DLS_GAZEBO_IMU_MGX_H

#include <dls2_msgs/ImuMgx.h>
#include <dls2_hardware_interface/imu_mgx_interface.h>


namespace dls_hw_sim
{

	class ImuMgxRosInterface : public RosInterface<dls2_msgs::ImuMgx> {
	public:
		virtual void initMsg() override {}
		virtual void fill() override {
			msg_.angular_velocity[0] = sim_model_->GetWorldAngularVel().x;
			msg_.angular_velocity[1] = sim_model_->GetWorldAngularVel().y;
			msg_.angular_velocity[2] = sim_model_->GetWorldAngularVel().z;
			msg_.specific_force[0] = sim_model_->GetWorldLinearAccel().x;
			msg_.specific_force[2] = sim_model_->GetWorldLinearAccel().y;
			msg_.specific_force[1] = sim_model_->GetWorldLinearAccel().z+9.81;
			msg_.quaternion[0] = sim_model_->GetWorldPose().rot.x;
			msg_.quaternion[1] = sim_model_->GetWorldPose().rot.y;
			msg_.quaternion[2] = sim_model_->GetWorldPose().rot.z;
			msg_.quaternion[3] = sim_model_->GetWorldPose().rot.w;
			msg_.time_stamp = 0;
			msg_.temperature = 0;
		}
	};

	class DlsGazeboImuMgx : public GazeboHardwareInterface {
	public:
		DlsGazeboImuMgx() : ros(nullptr) {
			data_.name = "imu_mgx";
			data_.angular_velocity = &angular_velocity_[0];
			data_.specific_force = &specific_force_[0];
			data_.quaternion = &quaternion_[0];
			data_.time_stamp = &time_stamp_;
			data_.temperature = &temperature_;
		}

		~DlsGazeboImuMgx() { if (ros) delete(ros); }

		void init(gazebo::physics::ModelPtr sim_model, ros::NodeHandle &model_nh) {
			GazeboHardwareInterface::init(sim_model);
			ros = new ImuMgxRosInterface();
			ros->init(sim_model_,model_nh,"imu_mgx",1);
			add_interface(ros);
		}

		
		void fillImuMgxInterface(ros::Time time)
		{
			angular_velocity_[0] = sim_model_->GetWorldAngularVel().x;
			angular_velocity_[1] = sim_model_->GetWorldAngularVel().y;
			angular_velocity_[2] = sim_model_->GetWorldAngularVel().z;
			specific_force_[0] = sim_model_->GetWorldLinearAccel().x;
			specific_force_[2] = sim_model_->GetWorldLinearAccel().y;
			specific_force_[1] = sim_model_->GetWorldLinearAccel().z+9.81;
			quaternion_[0] = sim_model_->GetWorldPose().rot.x;
			quaternion_[1] = sim_model_->GetWorldPose().rot.y;
			quaternion_[2] = sim_model_->GetWorldPose().rot.z;
			quaternion_[3] = sim_model_->GetWorldPose().rot.w;
			time_stamp_ = 0;
			temperature_ = 0;
		}


	public:
		hardware_interface::ImuMgxHandle::Data data_;
	private:
		ImuMgxRosInterface *ros;		
		std::string				name_;
		std::array<float,3> 	angular_velocity_;
		std::array<float,3> 	specific_force_;
		std::array<float,4>		quaternion_;
		uint32_t 				time_stamp_;
		uint16_t 				temperature_;

	};

}

#endif