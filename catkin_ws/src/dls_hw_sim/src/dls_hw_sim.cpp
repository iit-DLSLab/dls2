#include <cassert>
#include <gazebo/sensors/SensorManager.hh>
#include <gazebo/math/Vector3.hh>
#include <gazebo/common/common.hh>
#include <geometry_msgs/Vector3Stamped.h>

#include "dls_hw_sim/dls_hw_sim.h"

PLUGINLIB_EXPORT_CLASS(dls_hw_sim::DlsRobotHwSim, gazebo_ros_control::RobotHWSim)


// dls2
// #include "dls2/util/messaging/publisher_base.hpp"
// #include <memory>
// #include "dls2/msg/boolPubSubTypes.h"
// #include "dls2/topics/simulation_pause.hpp"
// std::shared_ptr<dls::PublisherBase<BoolMsgPubSubType>> pPause_pub;

void callback_pause(bool is_paused)
{
	// std::cout << "pause callback triggered" << std::endl;
	// BoolMsg msg;
	// msg.val(is_paused);
	// pPause_pub->publish(msg);
}
namespace dls_hw_sim
{

bool DlsRobotHwSim::initSim(
	const std::string& robot_namespace,
	ros::NodeHandle model_nh,
	gazebo::physics::ModelPtr parent_model,
	const urdf::Model *const urdf_model,
	std::vector<transmission_interface::TransmissionInfo> transmissions)
{
	sim_model_ = parent_model;

	joint_state_pub_ = model_nh.advertise<sensor_msgs::JointState>("joint_states", 1);
	odometry_pub_ = model_nh.advertise<nav_msgs::Odometry>("ground_truth",1);
	blind_state_pub_ = model_nh.advertise<dls2_msgs::BlindState>("blind_state",1);
	hyq_raw_pub_ = model_nh.advertise<dls2_msgs::HyqRaw>("hyq_raw",1);
	imu_kvh_pub_ = model_nh.advertise<dls2_msgs::ImuKvh>("imu_kvh",1);
	imu_mgx_pub_ = model_nh.advertise<dls2_msgs::ImuMgx>("imu_mgx",1);

	// Resize vectors to our DOF
	n_dof_ = transmissions.size();
	joint_name_.resize(n_dof_);
	joint_types_.resize(n_dof_);
	joint_lower_limits_.resize(n_dof_);
	joint_upper_limits_.resize(n_dof_);
	joint_effort_limits_.resize(n_dof_);
	joint_position_.resize(n_dof_);
	joint_position_prev_.resize(n_dof_);
	joint_velocity_.resize(n_dof_);
	joint_acceleration_.resize(n_dof_);
	joint_effort_.resize(n_dof_);
	joint_effort_command_.resize(n_dof_);
	joint_state_msg_.name.resize(n_dof_);
	joint_state_msg_.position.resize(n_dof_);
	joint_state_msg_.velocity.resize(n_dof_);
	joint_state_msg_.effort.resize(n_dof_);
	joint_state_msg_.header.frame_id = "joint";
	odometry_msg_.child_frame_id = "base";
	blind_state_msg_.header.frame_id = "base";

	// pPause_pub = std::make_shared<dls::PublisherBase<BoolMsgPubSubType>>(dls::topics::simulation_pause);
	// pause_connection =  gazebo::event::Events::ConnectPause(std::function<void(bool)>(callback_pause));
	//// gazebo::event::Events::ConnectPause();

	for (int i=0;i<36;i++)
	{
		odometry_msg_.pose.covariance[i]=0.0;
		odometry_msg_.twist.covariance[i]=0.0;
	}
	for (int i=1;i<6;i++)
	{
		odometry_msg_.pose.covariance[i*7]=1.0;
		odometry_msg_.pose.covariance[i*7]=1.0;
	}

	// Initialize values
	for (unsigned int j=0; j < n_dof_; j++) {
		// Check that this transmission has one joint
		if (transmissions[j].joints_.size() == 0) {
			ROS_WARN_STREAM_NAMED("dls_hw_sim","Transmission " << transmissions[j].name_ << " has no associated joints.");
			continue;
		} else if (transmissions[j].joints_.size() > 1) {
			ROS_WARN_STREAM_NAMED("dls_robot_hw_sim","Transmission " << transmissions[j].name_ << " has more than one joint. Currently the default robot hardware simulation interface only supports one.");
			continue;
		}

		// Check that this transmission has one actuator
		if (transmissions[j].actuators_.size() == 0) {
			ROS_WARN_STREAM_NAMED("dls_robot_hw_sim","Transmission " << transmissions[j].name_ << " has no associated actuators.");
			continue;
		} else if (transmissions[j].actuators_.size() > 1) {
			ROS_WARN_STREAM_NAMED("dls_robot_hw_sim","Transmission " << transmissions[j].name_ << " has more than one actuator. Currently the default robot hardware simulation interface only supports one.");
			continue;
		}

		// Add data from transmission
		joint_name_[j] = transmissions[j].joints_[0].name_;
		joint_position_[j] = 0.0;
		joint_velocity_[j] = 0.0;
		joint_acceleration_[j] = 0.0;
		joint_effort_[j] = 0.0;
		joint_effort_command_[j] = 0.0;

		joint_state_msg_.name[j] = joint_name_[j];  // IMPORTANT - Joint Order depends on the proper joint order in the urdf!

		if (transmissions[j].actuators_[0].hardware_interfaces_.size()>0) {
			const std::string &hardware_interface = transmissions[j].actuators_[0].hardware_interfaces_[0];
			ROS_ERROR_STREAM("Loading joint '" << joint_name_[j] << "' of type '" << hardware_interface << "'");
		}
		else
		{
			//ROS_ERROR_STREAM(joint_name_[j] << " does not have an associated hardware interface.");
		}

		// Debug
		//ROS_DEBUG_STREAM_NAMED("dls_robot_hw_sim","Loading joint '" << joint_name_[j] << "' of type '" << hardware_interface << "'");


		joint_state_interface_.registerHandle(hardware_interface::JointStateHandle(joint_name_[j], &joint_position_[j], &joint_velocity_[j], &joint_effort_[j]));
		joint_command_interface_.registerHandle(hardware_interface::JointHandle(joint_state_interface_.getHandle(joint_name_[j]), &joint_effort_command_[j]));

		gazebo::physics::JointPtr joint = parent_model->GetJoint(joint_name_[j]);
		if (!joint) {
			ROS_ERROR_STREAM("This robot has a joint named \"" << joint_name_[j] << "\" which is not in the gazebo model.");
			return false;
		} else {
			sim_joints_.push_back(joint);
			joint_effort_limits_[j] = joint->GetEffortLimit(0);
			joint->SetEffortLimit(0,joint_effort_limits_[j]); // TODO this set is useless:
		}

	}

	initial_pose_ = sim_model_->GetWorldPose();
	robot_name_ = sim_model_->GetName();


	// Hardware interfaces: Base IMU sensors
    imu_sensor_ = std::dynamic_pointer_cast<gazebo::sensors::ImuSensor> (gazebo::sensors::SensorManager::Instance()->GetSensor("trunk_imu"));
	if (!this->imu_sensor_) 	{
        ROS_WARN("Could not find base IMU sensor, using ground truth instead.");
	}

	imu_orientation_.resize(4);
	imu_orientation_covariance_.resize(9); // row major 3x3
	imu_angular_velocity_.resize(3);
	imu_angular_velocity_covariance_.resize(9); // row major 3x3
	imu_linear_acceleration_.resize(3);
	imu_linear_acceleration_covariance_.resize(9); // row major 3x3

	imu_kvh_angular_velocity_.resize(3);
	imu_kvh_specific_force_.resize(3);
	imu_mgx_angular_velocity_.resize(3);
	imu_mgx_specific_force_.resize(3);
	imu_mgx_quaternion_.resize(3);
	hyq_abs_enc_.resize(12);
	hyq_rel_enc_.resize(12);
	hyq_torque_sensor_haa_.resize(4);
	hyq_load_cell_hfe_.resize(4);
	hyq_load_cell_kfe_.resize(4);

	imuData.name = "trunk_imu"; // TODO: Fetch from elsewhere?
	imuData.frame_id = "trunk_imu"; // TODO: Fetch from URDF?
	imuData.orientation = &imu_orientation_[0];
	imuData.orientation_covariance = &imu_orientation_covariance_[0];
	imuData.angular_velocity = &imu_angular_velocity_[0];
	imuData.angular_velocity_covariance = &imu_angular_velocity_covariance_[0];
	imuData.linear_acceleration = &imu_linear_acceleration_[0];
	imuData.linear_acceleration_covariance = &imu_linear_acceleration_covariance_[0];
	imu_sensor_interface_.registerHandle(hardware_interface::ImuSensorHandle(imuData));

	blind_state_base_pose_world_.resize(7);
	blind_state_base_velocity_world_.resize(6);
	blind_state_base_acceleration_world_.resize(6);

	blindStateData.name = "blind_state"; // TODO
	blindStateData.joint_position = &joint_position_[0];
	blindStateData.joint_velocity = &joint_velocity_[0];
	blindStateData.joint_acceleration = &joint_acceleration_[0];
	blindStateData.joint_effort = &joint_effort_[0];
	blindStateData.base_pose_world = &blind_state_base_pose_world_[0];
	blindStateData.base_velocity_world = &blind_state_base_velocity_world_[0];
	blindStateData.base_acceleration_world = &blind_state_base_acceleration_world_[0];
	blind_state_interface_.registerHandle(hardware_interface::BlindStateHandle(blindStateData));

	imuKvhData.name = "imu_kvh";
	imuKvhData.angular_velocity = &imu_kvh_angular_velocity_[0];
	imuKvhData.specific_force = &imu_kvh_specific_force_[0];
	imuKvhData.time_stamp = &imu_kvh_time_stamp_;
	imuKvhData.seq = &imu_kvh_seq_;
	imuKvhData.status = &imu_kvh_status_;
	imuKvhData.time_sync = &imu_kvh_time_sync_;
	imu_kvh_interface_.registerHandle(hardware_interface::ImuKvhHandle(imuKvhData));

	imuMgxData.name = "imu_mgx";
	imuMgxData.angular_velocity = &imu_mgx_angular_velocity_[0];
	imuMgxData.specific_force = &imu_mgx_specific_force_[0];
	imuMgxData.quaternion = &imu_mgx_quaternion_[0];
	imuMgxData.time_stamp = &imu_mgx_time_stamp_;
	imuMgxData.temperature = &imu_mgx_temperature_;
	imu_mgx_interface_.registerHandle(hardware_interface::ImuMgxHandle(imuMgxData));

	hyqRawData.name = "hyq_raw";
	hyqRawData.imu_mgx = &imuMgxData;
	hyqRawData.imu_kvh = &imuKvhData;
	hyqRawData.abs_enc = &hyq_abs_enc_[0];
	hyqRawData.rel_enc = &hyq_rel_enc_[0];
	hyqRawData.torque_sensor_haa = &hyq_torque_sensor_haa_[0];
	hyqRawData.load_cell_hfe = &hyq_load_cell_hfe_[0];
	hyqRawData.load_cell_kfe = &hyq_load_cell_kfe_[0];
	hyq_raw_interface_.registerHandle(hardware_interface::HyqRawHandle(hyqRawData));

	// Register interfaces
	registerInterface(&joint_command_interface_);
	registerInterface(&joint_state_interface_);
	registerInterface(&imu_sensor_interface_);
	registerInterface(&blind_state_interface_);
	registerInterface(&imu_kvh_interface_);
	registerInterface(&imu_mgx_interface_);
	registerInterface(&hyq_raw_interface_);

	freeze_cmd_=true;
	freeze_state_=false;
    freeze_base_srv_ = model_nh.advertiseService("freeze_base", &DlsRobotHwSim::freezeBase, this);

	return true;
}


void DlsRobotHwSim::readSim(ros::Time time, ros::Duration period)
{
	// Fill ROS Control Interfaces
	ros::Time t = ros::Time::now();

	//Fill ROS Control
	fillJointStateInterface(t);
	fillImuSensorInterface(t);
	fillBlindStateInterface(t); //Depends on Joint State Interface
	fillImuKvhInterface(t);
	fillImuMgxInterface(t);
	fillHyqRawInterface(t); //Depends on Joint State, IMU KVH, and MGX Interfaces

	// Fill ROS Messages
	fillJointStateMsgAndPublish(t);
	fillOdometryMsgAndPublish(t);
	fillBlindStateMsgAndPublish(t);
	fillImuKvhMsgAndPublish(t);
	fillImuMgxMsgAndPublish(t);
	fillHyqRawMsgAndPublish(t); //Depends onIMU KVH and MXG Msgs

	for (int i=0;i<12;i++)
		joint_position_prev_[i]=joint_position_[i];

}

bool DlsRobotHwSim::freezeBase(std_srvs::Empty::Request& req, std_srvs::Empty::Response& res)
{
    freeze_cmd_=!freeze_state_;
    return true;
}

void DlsRobotHwSim::writeSim(ros::Time time, ros::Duration period)
{
 	for (unsigned int i=0; i < sim_joints_.size(); i++) {
 	        sim_joints_[i]->SetForce(0, joint_effort_command_[i]);
 	}
	if (freeze_cmd_!=freeze_state_)
	{
		freeze_state_ = freeze_cmd_;
		if (freeze_state_)
		{
			std::cout << "Freeze Base on!" << std::endl;
			sim_model_->SetWorldPose(initial_pose_);
		}
		else
		{
			std::cout << "Freeze Base off!" << std::endl;
		}
		sim_model_->SetGravityMode(!freeze_state_);
	}
	if (freeze_state_)
	{
		sim_model_->SetWorldPose(initial_pose_);
		sim_model_->SetGravityMode(false);
		gazebo::physics::LinkPtr base_link = sim_model_->GetLink("base_link");
		if (base_link)
		{
			base_link->SetLinearVel(gazebo::math::Vector3::Zero);
			base_link->SetLinearAccel(gazebo::math::Vector3::Zero);
			base_link->SetAngularVel(gazebo::math::Vector3::Zero);
			base_link->SetAngularAccel(gazebo::math::Vector3::Zero);

		}
	}
	// std::cout << "world is paused? " << std::endl;sim_model_->GetWorld()->IsPaused() <<std::endl;;
}

bool DlsRobotHwSim::checkForConflict(const std::list<hardware_interface::ControllerInfo>& info) const
{
	bool conflict[12];
	for (int i=0;i<12;i++) conflict[i]=false;
	for (auto it=info.begin();it!=info.end();++it)
	{
		for (auto it2=it->claimed_resources.begin();it2!=it->claimed_resources.end();++it2)
		{
			for (auto it3=it2->resources.begin();it3!=it2->resources.end();++it3) {
				if (it3->compare("lf_haa_joint")==0) { if (conflict[0])  return true; conflict[0]=true; }
				if (it3->compare("lf_hfe_joint")==0) { if (conflict[1])  return true; conflict[1]=true; }
				if (it3->compare("lf_kfe_joint")==0) { if (conflict[2])  return true; conflict[2]=true; }
				if (it3->compare("rf_haa_joint")==0) { if (conflict[3])  return true; conflict[3]=true; }
				if (it3->compare("rf_hfe_joint")==0) { if (conflict[4])  return true; conflict[4]=true; }
				if (it3->compare("rf_kfe_joint")==0) { if (conflict[5])  return true; conflict[5]=true; }
				if (it3->compare("lh_haa_joint")==0) { if (conflict[6])  return true; conflict[6]=true; }
				if (it3->compare("lh_hfe_joint")==0) { if (conflict[7])  return true; conflict[7]=true; }
				if (it3->compare("lh_kfe_joint")==0) { if (conflict[8])  return true; conflict[8]=true; }
				if (it3->compare("rh_haa_joint")==0) { if (conflict[9])  return true; conflict[9]=true; }
				if (it3->compare("rh_hfe_joint")==0) { if (conflict[10]) return true; conflict[10]=true; }
				if (it3->compare("rh_kfe_joint")==0) { if (conflict[11]) return true; conflict[11]=true; }
			}
		}
	}

	return false;
}

void DlsRobotHwSim::fillJointStateInterface(ros::Time time)
{
	for (unsigned int j=0; j < sim_joints_.size(); j++) {
		// Gazebo has an interesting API...
		if (joint_types_[j] == urdf::Joint::PRISMATIC) {
			joint_position_[j] = sim_joints_[j]->GetAngle(0).Radian();
		} else {
			joint_position_[j] += angles::shortest_angular_distance(joint_position_[j],sim_joints_[j]->GetAngle(0).Radian());
		}
		joint_velocity_[j] = sim_joints_[j]->GetVelocity(0);
		joint_acceleration_[j] = 0.0; // TODO
		joint_effort_[j] = sim_joints_[j]->GetForce((unsigned int)(0));
	}
}

void DlsRobotHwSim::fillImuSensorInterface(ros::Time time)
{
	// TODO
}

void DlsRobotHwSim::fillBlindStateInterface(ros::Time time)
{
	//blind_state_joint_state_; Filled by Joint State Interface
	blind_state_base_pose_world_[0] = sim_model_->GetWorldPose().pos.x;
	blind_state_base_pose_world_[1] = sim_model_->GetWorldPose().pos.y;
	blind_state_base_pose_world_[2] = sim_model_->GetWorldPose().pos.z;
	blind_state_base_pose_world_[3] = sim_model_->GetWorldPose().rot.x;
	blind_state_base_pose_world_[4] = sim_model_->GetWorldPose().rot.y;
	blind_state_base_pose_world_[5] = sim_model_->GetWorldPose().rot.z;
	blind_state_base_pose_world_[6] = sim_model_->GetWorldPose().rot.w;

	blind_state_base_velocity_world_[0] = sim_model_->GetWorldLinearVel().x;
	blind_state_base_velocity_world_[1] = sim_model_->GetWorldLinearVel().y;
	blind_state_base_velocity_world_[2] = sim_model_->GetWorldLinearVel().z;
	blind_state_base_velocity_world_[3] = sim_model_->GetWorldAngularVel().x;
	blind_state_base_velocity_world_[4] = sim_model_->GetWorldAngularVel().y;
	blind_state_base_velocity_world_[5] = sim_model_->GetWorldAngularVel().z;

	blind_state_base_acceleration_world_[0] = sim_model_->GetWorldLinearAccel().x;
	blind_state_base_acceleration_world_[1] = sim_model_->GetWorldLinearAccel().y;
	blind_state_base_acceleration_world_[2] = sim_model_->GetWorldLinearAccel().z;
	blind_state_base_acceleration_world_[3] = sim_model_->GetWorldAngularAccel().x;
	blind_state_base_acceleration_world_[4] = sim_model_->GetWorldAngularAccel().y;
	blind_state_base_acceleration_world_[5] = sim_model_->GetWorldAngularAccel().z;

}

void DlsRobotHwSim::fillImuKvhInterface(ros::Time time)
{
	imu_kvh_angular_velocity_[0] = sim_model_->GetWorldAngularVel().x;
	imu_kvh_angular_velocity_[1] = sim_model_->GetWorldAngularVel().y;
	imu_kvh_angular_velocity_[2] = sim_model_->GetWorldAngularVel().z;
	imu_kvh_specific_force_[0] = sim_model_->GetWorldLinearAccel().x;
	imu_kvh_specific_force_[2] = sim_model_->GetWorldLinearAccel().y;
	imu_kvh_specific_force_[1] = sim_model_->GetWorldLinearAccel().z+9.81;
	imu_kvh_seq_++;
	imu_kvh_status_=119;
	imu_kvh_time_sync_=0;
}

void DlsRobotHwSim::fillImuMgxInterface(ros::Time time)
{
	imu_mgx_angular_velocity_[0] = sim_model_->GetWorldAngularVel().x;
	imu_mgx_angular_velocity_[1] = sim_model_->GetWorldAngularVel().y;
	imu_mgx_angular_velocity_[2] = sim_model_->GetWorldAngularVel().z;
	imu_mgx_specific_force_[0] = sim_model_->GetWorldLinearAccel().x;
	imu_mgx_specific_force_[2] = sim_model_->GetWorldLinearAccel().y;
	imu_mgx_specific_force_[1] = sim_model_->GetWorldLinearAccel().z+9.81;
	imu_mgx_quaternion_[0] = sim_model_->GetWorldPose().rot.x;
	imu_mgx_quaternion_[1] = sim_model_->GetWorldPose().rot.y;
	imu_mgx_quaternion_[2] = sim_model_->GetWorldPose().rot.z;
	imu_mgx_quaternion_[3] = sim_model_->GetWorldPose().rot.w;
	imu_mgx_time_stamp_ = 0;
	imu_mgx_temperature_ = 0;
}

void DlsRobotHwSim::fillHyqRawInterface(ros::Time time)
{
	//hyq_raw_imu_kvh; // filled by IMU KVH Interface
	//hyq_raw_imu_mgx; // filled by IMU MGX Interface
	for (int i=0;i<12;i++) {
		// TODO Magic getNumber
		//651 = 4096 bits / (2*Pi)
		hyq_raw_msg_.abs_enc[i] = (uint32_t)round(joint_position_[i]*651.89919754);
		//12732 = 80000 bits / (2*Pi)
		hyq_raw_msg_.rel_enc[i] = (uint32_t)round((joint_position_[i]-joint_position_prev_[i])*12732.406202);
	}
	for (int i=0;i<4;i++) {
		// TODO torque per volt
		hyq_raw_msg_.torque_sensor_haa[i] = (uint16_t)joint_effort_[i];
		// TODO newton per volt AND Inverse Lever Arm calculation!
		hyq_raw_msg_.load_cell_hfe[i+4] = (uint16_t)joint_effort_[i+4];
		hyq_raw_msg_.load_cell_kfe[i+8] = (uint16_t)joint_effort_[i+8];
	}
}


void DlsRobotHwSim::fillJointStateMsg(ros::Time time)
{
	for (int i=0;i<n_dof_;i++)
	{
		joint_state_msg_.position[i] = joint_position_[i];
		joint_state_msg_.velocity[i] = joint_velocity_[i];
		joint_state_msg_.effort[i] = joint_effort_[i];
	}
	joint_state_msg_.header.stamp = time;
}

void DlsRobotHwSim::fillOdometryMsg(ros::Time time)
{
	odometry_msg_.header.stamp = time;
	odometry_msg_.pose.pose.position.x = sim_model_->GetWorldPose().pos.x;
	odometry_msg_.pose.pose.position.y = sim_model_->GetWorldPose().pos.y;
	odometry_msg_.pose.pose.position.z = sim_model_->GetWorldPose().pos.z;
	odometry_msg_.pose.pose.orientation.w = sim_model_->GetWorldPose().rot.w;
	odometry_msg_.pose.pose.orientation.x = sim_model_->GetWorldPose().rot.x;
	odometry_msg_.pose.pose.orientation.y = sim_model_->GetWorldPose().rot.y;
	odometry_msg_.pose.pose.orientation.z = sim_model_->GetWorldPose().rot.z;
	//odometry_msg_.pose.covariance left at I
	odometry_msg_.twist.twist.linear.x = sim_model_->GetWorldLinearVel().x;
	odometry_msg_.twist.twist.linear.y = sim_model_->GetWorldLinearVel().y;
	odometry_msg_.twist.twist.linear.z = sim_model_->GetWorldLinearVel().z;
	odometry_msg_.twist.twist.angular.x = sim_model_->GetWorldAngularVel().x;
	odometry_msg_.twist.twist.angular.y = sim_model_->GetWorldAngularVel().y;
	odometry_msg_.twist.twist.angular.z = sim_model_->GetWorldAngularVel().z;
	//odometry_msg_.twist.covariance left at I}
}

void DlsRobotHwSim::fillBlindStateMsg(ros::Time time)
{
	blind_state_msg_.header.stamp = time;
	for (int i=0;i<n_dof_;i++)
	{
		blind_state_msg_.joint_state.position[i] = joint_position_[i];
		blind_state_msg_.joint_state.velocity[i] = joint_velocity_[i];
		blind_state_msg_.joint_state.acceleration[i] = 0; // TODO
		blind_state_msg_.joint_state.effort[i] = joint_effort_[i];
	}

	blind_state_msg_.base_pose_world.position[0] = sim_model_->GetWorldPose().pos.x;
	blind_state_msg_.base_pose_world.position[1] = sim_model_->GetWorldPose().pos.y;
	blind_state_msg_.base_pose_world.position[2] = sim_model_->GetWorldPose().pos.z;

	// Filled in order x y z w to conform to Eigen's internal representation
	blind_state_msg_.base_pose_world.quaternion[0] = sim_model_->GetWorldPose().rot.x;
	blind_state_msg_.base_pose_world.quaternion[1] = sim_model_->GetWorldPose().rot.y;
	blind_state_msg_.base_pose_world.quaternion[2] = sim_model_->GetWorldPose().rot.z;
	blind_state_msg_.base_pose_world.quaternion[3] = sim_model_->GetWorldPose().rot.w;

	blind_state_msg_.base_velocity_world.linear[0] = sim_model_->GetWorldLinearVel().x;
	blind_state_msg_.base_velocity_world.linear[1] = sim_model_->GetWorldLinearVel().y;
	blind_state_msg_.base_velocity_world.linear[2] = sim_model_->GetWorldLinearVel().z;
	blind_state_msg_.base_velocity_world.angular[0] = sim_model_->GetWorldAngularVel().x;
	blind_state_msg_.base_velocity_world.angular[1] = sim_model_->GetWorldAngularVel().y;
	blind_state_msg_.base_velocity_world.angular[2] = sim_model_->GetWorldAngularVel().z;

	blind_state_msg_.base_acceleration_world.linear[0] = sim_model_->GetWorldLinearAccel().x;
	blind_state_msg_.base_acceleration_world.linear[1] = sim_model_->GetWorldLinearAccel().y;
	blind_state_msg_.base_acceleration_world.linear[2] = sim_model_->GetWorldLinearAccel().z;
	blind_state_msg_.base_acceleration_world.angular[0] = sim_model_->GetWorldLinearAccel().x;
	blind_state_msg_.base_acceleration_world.angular[1] = sim_model_->GetWorldLinearAccel().y;
	blind_state_msg_.base_acceleration_world.angular[2] = sim_model_->GetWorldLinearAccel().z;
}

void DlsRobotHwSim::fillImuKvhMsg(ros::Time time) //TODO Copy data from interface
{
	imu_kvh_msg_.angular_velocity[0] = sim_model_->GetWorldAngularVel().x;
	imu_kvh_msg_.angular_velocity[1] = sim_model_->GetWorldAngularVel().y;
	imu_kvh_msg_.angular_velocity[2] = sim_model_->GetWorldAngularVel().z;
	imu_kvh_msg_.specific_force[0] = sim_model_->GetWorldLinearAccel().x;
	imu_kvh_msg_.specific_force[2] = sim_model_->GetWorldLinearAccel().y;
	imu_kvh_msg_.specific_force[1] = sim_model_->GetWorldLinearAccel().z+9.81;
	imu_kvh_msg_.seq++;
	imu_kvh_msg_.status=119;
	imu_kvh_msg_.time_sync=0;
}

void DlsRobotHwSim::fillImuMgxMsg(ros::Time time) //TODO Copy data from interface
{
	imu_mgx_msg_.angular_velocity[0] = sim_model_->GetWorldAngularVel().x;
	imu_mgx_msg_.angular_velocity[1] = sim_model_->GetWorldAngularVel().y;
	imu_mgx_msg_.angular_velocity[2] = sim_model_->GetWorldAngularVel().z;
	imu_mgx_msg_.specific_force[0] = sim_model_->GetWorldLinearAccel().x;
	imu_mgx_msg_.specific_force[2] = sim_model_->GetWorldLinearAccel().y;
	imu_mgx_msg_.specific_force[1] = sim_model_->GetWorldLinearAccel().z+9.81;
	imu_mgx_msg_.quaternion[0] = sim_model_->GetWorldPose().rot.x;
	imu_mgx_msg_.quaternion[1] = sim_model_->GetWorldPose().rot.y;
	imu_mgx_msg_.quaternion[2] = sim_model_->GetWorldPose().rot.z;
	imu_mgx_msg_.quaternion[3] = sim_model_->GetWorldPose().rot.w;
	imu_mgx_msg_.time_stamp = 0;
	imu_mgx_msg_.temperature = 0;

}

void DlsRobotHwSim::fillHyqRawMsg(ros::Time time) //TODO Copy data from interface
{
	hyq_raw_msg_.imu_kvh = imu_kvh_msg_;
	hyq_raw_msg_.imu_mgx = imu_mgx_msg_;
	for (int i=0;i<12;i++) {
		hyq_raw_msg_.abs_enc[i] = 0;
		hyq_raw_msg_.rel_enc[i] = 0;
	}
	for (int i=0;i<4;i++) {
		hyq_raw_msg_.torque_sensor_haa[i] = 0;
		hyq_raw_msg_.load_cell_hfe[i] = 0;
		hyq_raw_msg_.load_cell_kfe[i] = 0;
	}
}

void DlsRobotHwSim::fillImuKvhMsgAndPublish(ros::Time time){
	if (imu_kvh_pub_.getNumSubscribers()>0)
	{
		fillImuKvhMsg(time);
		imu_kvh_pub_.publish(imu_kvh_msg_);
	}
}

void DlsRobotHwSim::fillImuMgxMsgAndPublish(ros::Time time){
	if (imu_mgx_pub_.getNumSubscribers()>0)
	{
		fillImuMgxMsg(time);
		imu_mgx_pub_.publish(imu_mgx_msg_);
	}
}

void DlsRobotHwSim::fillHyqRawMsgAndPublish(ros::Time time){
	if (hyq_raw_pub_.getNumSubscribers()>0)
	{
		fillHyqRawMsg(time);
		hyq_raw_pub_.publish(hyq_raw_msg_);
	}
}

void DlsRobotHwSim::fillJointStateMsgAndPublish(ros::Time time)
{
	if (joint_state_pub_.getNumSubscribers()>0)
	{
		fillJointStateMsg(time);
		joint_state_pub_.publish(joint_state_msg_);
	}
}

void DlsRobotHwSim::fillOdometryMsgAndPublish(ros::Time time)
{
	if (odometry_pub_.getNumSubscribers()>0)
	{
		fillOdometryMsg(time);
		odometry_pub_.publish(odometry_msg_);
	}
}

void DlsRobotHwSim::fillBlindStateMsgAndPublish(ros::Time time)
{
	if (blind_state_pub_.getNumSubscribers()>0)
	{
		fillBlindStateMsg(time);
		blind_state_pub_.publish(blind_state_msg_);
	}
}



}
