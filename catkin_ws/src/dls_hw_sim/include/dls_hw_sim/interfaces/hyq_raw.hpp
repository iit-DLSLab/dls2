#ifndef GAZEBO_ROS_CONTROL___DLS_GAZEBO_HYQ_RAW_H
#define GAZEBO_ROS_CONTROL___DLS_GAZEBO_HYQ_RAW_H

#include <dls2_msgs/HyqRaw.h>
#include <dls2_hardware_interface/hyq_raw_interface.h>
#include <dls2_hardware_interface/imu_kvh_interface.h>
#include <dls2_hardware_interface/imu_mgx_interface.h>


namespace dls_hw_sim
{
	class HyqRawRosInterface : public RosInterface<dls2_msgs::HyqRaw> {
	public:
		HyqRawRosInterface(std::vector<gazebo::physics::JointPtr> joint) : joint_(joint) { }
		virtual void initMsg() override { }
		virtual void fill() override {
			//todo broken
			//msg_.imu_kvh = dls_gazebo_imu_kvh_->msg_;
			/*for (int i=0;i<3;i++) { 
				msg_.imu_mgx.angular_velocity[i] = i;//dls_gazebo_imu_mgx_->msg_.angular_velocity[i];
			}
			for (int i=0;i<12;i++) {
				msg_.abs_enc[i] = 0;//abs_enc_[i];
				msg_.rel_enc[i] = 0;//rel_enc_[i];
			}
			for (int i=0;i<4;i++) {
				msg_.torque_sensor_haa[i] = 0;//torque_sensor_haa_[i];
				msg_.load_cell_hfe[i] = 0;//load_cell_hfe_[i];
				msg_.load_cell_kfe[i] = 0;//load_cell_kfe_[i];
			}*/
		}
	private:
		std::vector<gazebo::physics::JointPtr>	joint_;
	};

	class DlsGazeboHyqRaw : public GazeboHardwareInterface {
	public:

		DlsGazeboHyqRaw() : ros(nullptr) {
			data_.name = "hyq_raw";
			data_.imu_mgx = &imu_mgx_;
			data_.imu_kvh = &imu_kvh_;
			data_.abs_enc = &abs_enc_[0];
			data_.rel_enc = &rel_enc_[0];
			data_.torque_sensor_haa = &torque_sensor_haa_[0];
			data_.load_cell_hfe = &load_cell_hfe_[0];
			data_.load_cell_kfe = &load_cell_kfe_[0];


			imu_kvh_.name = "imu_kvh";
			imu_kvh_.angular_velocity = &kvh_angular_velocity_[0];
			imu_kvh_.specific_force = &kvh_specific_force_[0];
			imu_kvh_.time_stamp = &kvh_time_stamp_;
			imu_kvh_.seq = &kvh_seq_;
			imu_kvh_.status = &kvh_status_;
			imu_kvh_.time_sync = &kvh_time_sync_;

			imu_mgx_.name = "imu_mgx";
			imu_mgx_.angular_velocity = &mgx_angular_velocity_[0];
			imu_mgx_.specific_force = &mgx_specific_force_[0];
			imu_mgx_.quaternion = &mgx_quaternion_[0];
			imu_mgx_.time_stamp = &mgx_time_stamp_;
			imu_mgx_.temperature = &mgx_temperature_;

		}

		~DlsGazeboHyqRaw() { if (ros) delete(ros); }

		void init(gazebo::physics::ModelPtr sim_model, std::vector<transmission_interface::TransmissionInfo> transmissions,ros::NodeHandle &model_nh) {
			GazeboHardwareInterface::init(sim_model);


			/*for (auto const& transmission: transmissions) {
				if (transmission.joints_.size() == 1) {
					gazebo::physics::JointPtr joint = sim_model->GetJoint(transmission.joints_[0].name_);
					if (joint!=nullptr) {
						joint_.push_back(joint);
						name_.push_back(transmission.joints_[0].name_);
					}
				}
			}

			joint_position_.resize(joint_.size());
			joint_position_prev_.resize(joint_.size());
			joint_velocity_.resize(joint_.size());
			joint_effort_.resize(joint_.size());

			ros = new HyqRawRosInterface(joint_);
			ros->init(sim_model_,model_nh,"hyq_raw",1);*/
			//add_interface(ros);

		}

		void fillHyqRawInterface(ros::Time time)
		{



			/*for (unsigned int j=0; j < joint_.size(); j++) {
				joint_position_prev_[j]=joint_position_[j];
				joint_position_[j] += angles::shortest_angular_distance(joint_position_[j],joint_[j]->GetAngle(0).Radian());
				joint_velocity_[j] = joint_[j]->GetVelocity(0);
				joint_effort_[j] = joint_[j]->GetForce((unsigned int)(0));
			}


			double dtmp,dtmp2;
			for (int i=0;i<12;i++) {
				// TODO Magic Numbers
				dtmp = joint_position_[i];
				if (dtmp < 0) dtmp += 6.28318; // 2*Pi
				dtmp *=651.89919754; //651 = 4096 bits / (2*Pi)
				abs_enc_[i] = (uint32_t)round(dtmp);
				//12732 = 80000 bits / (2*Pi)

				dtmp = joint_position_[i];
				dtmp2 = joint_position_prev_[i];
				if (dtmp<0) dtmp += 6.28318;
				if (dtmp2<0) dtmp2 += 6.28318;
				dtmp -= dtmp2;
				dtmp *=12732.406201955;

				rel_enc_[i] = (uint32_t)round(dtmp);
			}
			for (int i=0;i<4;i++) {
				// TODO torque per volt
				torque_sensor_haa_[i] = (uint16_t)joint_effort_[i*3];
				// TODO newton per volt AND Inverse Lever Arm calculation!
				load_cell_hfe_[i] = (uint16_t)joint_effort_[i*3+1];
				load_cell_kfe_[i] = (uint16_t)joint_effort_[i*3+2];
			}*/

		}




		


	public:
		hardware_interface::HyqRawHandle::Data data_;
		
	private:
		std::vector<std::string>				name_;
		std::vector<gazebo::physics::JointPtr>	joint_;
		HyqRawRosInterface *ros;	
		//std::string					name_;
		std::vector<double> joint_position_;
		std::vector<double> joint_position_prev_;
		std::vector<double> joint_velocity_;
		std::vector<double> joint_effort_;	
		std::array<uint32_t,12> abs_enc_;
		std::array<uint32_t,12> rel_enc_;
		std::array<uint16_t,4> torque_sensor_haa_;
		std::array<uint16_t,4> load_cell_hfe_;
		std::array<uint16_t,4> load_cell_kfe_;
	

    	hardware_interface::ImuMgxHandle::Data imu_mgx_;
    	std::array<float,3> 	mgx_angular_velocity_;
		std::array<float,3> 	mgx_specific_force_;
		std::array<float,4>		mgx_quaternion_;
		uint32_t 				mgx_time_stamp_;
		uint16_t 				mgx_temperature_;

    	hardware_interface::ImuKvhHandle::Data imu_kvh_;	
		std::array<float,3> 	kvh_angular_velocity_;
		std::array<float,3> 	kvh_specific_force_;
		uint32_t 				kvh_time_stamp_;
		uint8_t 				kvh_seq_;
		uint8_t 				kvh_status_;
		uint16_t 				kvh_time_sync_;


	};

}

#endif