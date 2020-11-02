#include "dls_hw_sim/dls_hw_sim.hpp"
#include <ros/ros.h>
#include <urdf/model.h>
#include <transmission_interface/transmission_parser.h>

#include <sstream>
#include <iostream>

namespace dls_hw_sim
{

DlsRobotHwSim::DlsRobotHwSim() :
	gazebo::ModelPlugin(),
	blind_state_pub_(dls::topics::low_level_estimation::blind_state)
{ }

void DlsRobotHwSim::Load
(
	gazebo::physics::ModelPtr model,
	sdf::ElementPtr element
)
{
	std::cout << "LOADING DLS_HW_SIM PLUGIN" << std::endl;
	if (!model)
	{
		ROS_ERROR_STREAM_NAMED("loadThread","parent model is NULL");
		std::cerr << "Parent model is NULL" << std::endl;;
		return;
	}

	if(!ros::isInitialized())
	{
		ROS_FATAL_STREAM_NAMED
		(
			"dls_hw_sim",
			" A ROS node for Gazebo has not been initialized, unable to load"
			" plugin."
		);
		std::cerr << "Ros node has not been initialized, unable to laod plugin"
			      << std::endl;
		return;
	}

	auto robot_namespace = model->GetName();
	auto robot_description = "robot_description";

	ros::Duration gazebo_period(
		model->GetWorld()->GetPhysicsEngine()->GetMaxStepSize());

	ros::NodeHandle node_handle(robot_namespace);

	/*const*/ std::string urdf_string;
	node_handle.getParam("/robot_description", urdf_string);

	urdf::Model urdf_model;
	const urdf::Model *const urdf_model_ptr =
		urdf_model.initString(urdf_string) ? &urdf_model : NULL;

	transmission_interface::TransmissionParser::parse(urdf_string, transmissions_);
	if
	(
		!this->initSim
		(
			robot_namespace,
			node_handle,
			model,
			urdf_model_ptr,
			transmissions_
		)
	)
	{
		ROS_FATAL_NAMED("gazebo_ros_control","Could not initialize robot simulation interface");
		std::cerr << "COULD NOT INITIALIZE ROBOGT SIMULATION INTERFACE"
			      << std::endl;
		return;
	}

	this->sim_model_ = model;
	this->update_connection = gazebo::event::Events::ConnectWorldUpdateBegin
	(
		 std::bind(&DlsRobotHwSim::onGazeboUpdate, this)
	);
}

void DlsRobotHwSim::onGazeboUpdate()
{
	ros::Time t = ros::Time::now();
	ros::Duration d; // unused in the functions

	readSim(t, d);
	writeSim(t, d);
}

bool DlsRobotHwSim::initSim(
		const std::string& robot_namespace,
		ros::NodeHandle model_nh,
		gazebo::physics::ModelPtr parent_model,
		const urdf::Model *const urdf_model,
		std::vector<transmission_interface::TransmissionInfo> transmissions)
{
	sim_model_ = parent_model;

	dls_gazebo_imu_kvh_.init(parent_model,model_nh);
	dls_gazebo_imu_mgx_.init(parent_model,model_nh);
	dls_gazebo_imu_sensor_.init(parent_model,model_nh);
	dls_gazebo_joint_state_.init(parent_model,transmissions,model_nh);
	dls_gazebo_blind_state_.init(parent_model,transmissions,model_nh);
	dls_gazebo_hyq_raw_.init(parent_model,transmissions,model_nh);
	dls_gazebo_odometry_.init(parent_model,model_nh);


	// Resize vectors to our DOF
	n_dof_ = transmissions.size();
	joint_name_.resize(n_dof_);
	this->blind_state_msg_.joint_state().position().resize(12);
	this->blind_state_msg_.joint_state().velocity().resize(12);
	this->blind_state_msg_.joint_state().effort().resize(12);
	// joint_types_.resize(n_dof_);
	// joint_lower_limits_.resize(n_dof_);
	// joint_upper_limits_.resize(n_dof_);
	// joint_effort_limits_.resize(n_dof_);


	// joint_effort_command_.resize(n_dof_);


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
		// joint_effort_command_[j] = 0.0;


		//IMPORTANT TODO FIX
		// IMPORTANT - Joint Order depends on the proper joint order in the urdf!
		if (transmissions[j].actuators_[0].hardware_interfaces_.size()>0) {
		}


		// joint_state_interface_.registerHandle(
		// 	hardware_interface::JointStateHandle(
		// 		dls_gazebo_joint_state_.name_[j],
		// 		&dls_gazebo_joint_state_.position_[j],
		// 		&dls_gazebo_joint_state_.velocity_[j],
		// 		&dls_gazebo_joint_state_.effort_[j]
		// 	)
		// );

		// joint_command_interface_.registerHandle(hardware_interface::JointHandle(joint_state_interface_.getHandle(joint_name_[j]), &joint_effort_command_[j]));

		gazebo::physics::JointPtr joint = parent_model->GetJoint(joint_name_[j]);
		if (!joint) {
			ROS_ERROR_STREAM("This robot has a joint named \"" << joint_name_[j] << "\" which is not in the gazebo model.");
			return false;
		} else {
			sim_joints_.push_back(joint);
			// joint_effort_limits_[j] = joint->GetEffortLimit(0);
			// joint->SetEffortLimit(0,joint_effort_limits_[j]); // TODO this set is useless:
		}

	}


	initial_pose_ = sim_model_->GetWorldPose();
	// robot_name_ = sim_model_->GetName();


	// imu_kvh_interface_.registerHandle(hardware_interface::ImuKvhHandle(dls_gazebo_imu_kvh_.data_));
	// imu_mgx_interface_.registerHandle(hardware_interface::ImuMgxHandle(dls_gazebo_imu_mgx_.data_));
	// imu_sensor_interface_.registerHandle(hardware_interface::ImuSensorHandle(dls_gazebo_imu_sensor_.data_));
	// blind_state_interface_.registerHandle(hardware_interface::BlindStateHandle(dls_gazebo_blind_state_.data_));
	// hyq_raw_interface_.registerHandle(hardware_interface::HyqRawHandle(dls_gazebo_hyq_raw_.data_));



	// // Register interfaces
	// registerInterface(&joint_command_interface_);
	// registerInterface(&joint_state_interface_);
	// registerInterface(&imu_sensor_interface_);
	// registerInterface(&blind_state_interface_);
	// registerInterface(&imu_kvh_interface_);
	// registerInterface(&imu_mgx_interface_);
	// registerInterface(&hyq_raw_interface_);

	freeze_cmd_=true;
	freeze_state_=false;
	freeze_base_srv_ = model_nh.advertiseService("freeze_base", &DlsRobotHwSim::freezeBase, this);


	return true;
}


void DlsRobotHwSim::readSim(ros::Time time, ros::Duration period)
{
	//ROS_ERROR_STREAM("READSIM");
	//// Fill ROS Control Interfaces
	//ros::Time t = ros::Time::now();

	////Fill ROS Control
	//dls_gazebo_joint_state_.fillJointStateInterface(t);
	//dls_gazebo_imu_kvh_.fillImuKvhInterface(t);
	//dls_gazebo_imu_mgx_.fillImuMgxInterface(t);
	//dls_gazebo_blind_state_.fillBlindStateInterface(t);
	////dls_gazebo_hyq_raw_.fillHyqRawInterface(t);

	//// Fill ROS Messages
	dls_gazebo_hyq_raw_.fillAndPublish();
	dls_gazebo_blind_state_.fillAndPublish();
	dls_gazebo_imu_mgx_.fillAndPublish();
	dls_gazebo_imu_kvh_.fillAndPublish();
	dls_gazebo_joint_state_.fillAndPublish();
	dls_gazebo_imu_sensor_.fillAndPublish();
	dls_gazebo_odometry_.fillAndPublish();

	publish_blind_state();
}

void DlsRobotHwSim::publish_blind_state()
{

	// this->blind_state_msg_.header().stamp = ros::Time::now();
	for(int i = 0; i < 12; i++)
	{
		this->blind_state_msg_.joint_state().position()[i] = sim_joints_[i]->GetAngle(0).Radian();
		this->blind_state_msg_.joint_state().velocity()[i] = sim_joints_[i]->GetVelocity(0);
		this->blind_state_msg_.joint_state().effort()[i]   = sim_joints_[i]->GetForce(0);
	}

	this->blind_state_msg_.base_pose_world().position()[0] = sim_model_->GetWorldPose().pos.x;
	this->blind_state_msg_.base_pose_world().position()[1] = sim_model_->GetWorldPose().pos.y;
	this->blind_state_msg_.base_pose_world().position()[2] = sim_model_->GetWorldPose().pos.z;

	// Filled in order x y z w to conform to Eigen's internal representation
	this->blind_state_msg_.base_pose_world().quaternion()[0] = sim_model_->GetWorldPose().rot.x;
	this->blind_state_msg_.base_pose_world().quaternion()[1] = sim_model_->GetWorldPose().rot.y;
	this->blind_state_msg_.base_pose_world().quaternion()[2] = sim_model_->GetWorldPose().rot.z;
	this->blind_state_msg_.base_pose_world().quaternion()[3] = sim_model_->GetWorldPose().rot.w;

	this->blind_state_msg_.base_velocity_world().linear()[0]  = sim_model_->GetWorldLinearVel().x;
	this->blind_state_msg_.base_velocity_world().linear()[1]  = sim_model_->GetWorldLinearVel().y;
	this->blind_state_msg_.base_velocity_world().linear()[2]  = sim_model_->GetWorldLinearVel().z;
	this->blind_state_msg_.base_velocity_world().angular()[0] = sim_model_->GetWorldAngularVel().x;
	this->blind_state_msg_.base_velocity_world().angular()[1] = sim_model_->GetWorldAngularVel().y;
	this->blind_state_msg_.base_velocity_world().angular()[2] = sim_model_->GetWorldAngularVel().z;

	this->blind_state_msg_.base_acceleration_world().linear()[0]  = sim_model_->GetWorldLinearAccel().x;
	this->blind_state_msg_.base_acceleration_world().linear()[1]  = sim_model_->GetWorldLinearAccel().y;
	this->blind_state_msg_.base_acceleration_world().linear()[2]  = sim_model_->GetWorldLinearAccel().z;
	this->blind_state_msg_.base_acceleration_world().angular()[0] = sim_model_->GetWorldLinearAccel().x;
	this->blind_state_msg_.base_acceleration_world().angular()[1] = sim_model_->GetWorldLinearAccel().y;
	this->blind_state_msg_.base_acceleration_world().angular()[2] = sim_model_->GetWorldLinearAccel().z;

	// TEMPORARY PRINT OUT THE BLIND STATE TO THE CONSOLE
	// {
		// std::stringstream ss;

		// ss << "===============================================================";

		// ss << "position: ";

		// std::copy(std::cbegin(blind_state_msg_.joint_state().position()),
		// 	      std::cend(blind_state_msg_.joint_state().position()),
		// 	      std::ostream_iterator<double>(ss, ", "));

		// ss << "\nVelocity: ";

		// std::copy(std::cbegin(blind_state_msg_.joint_state().velocity()),
		// 	      std::cend(blind_state_msg_.joint_state().velocity()),
		// 	      std::ostream_iterator<double>(ss, ", "));

		// ss << "\nEffort: ";

		// std::copy(std::cbegin(blind_state_msg_.joint_state().effort()),
		// 	      std::cend(blind_state_msg_.joint_state().effort()),
		// 	      std::ostream_iterator<double>(ss, ", "));

		// ss << "\nPosition: ";

		// std::copy(std::cbegin(blind_state_msg_.base_pose_world().position()),
		// 	      std::cend(blind_state_msg_.base_pose_world().position()),
		// 	      std::ostream_iterator<double>(ss, ", "));

		// ss << "\nOrientation: ";

		// std::copy(std::cbegin(blind_state_msg_.base_pose_world().quaternion()),
		// 	      std::cend(blind_state_msg_.base_pose_world().quaternion()),
		// 	      std::ostream_iterator<double>(ss, ", "));

		// ss << "\nVelocity: ";

		// std::copy(std::cbegin(blind_state_msg_.base_velocity_world().linear()),
		// 	      std::cend(blind_state_msg_.base_velocity_world().linear()),
		// 	      std::ostream_iterator<double>(ss, ", "));

		// std::copy(std::cbegin(blind_state_msg_.base_velocity_world().angular()),
		// 	      std::cend(blind_state_msg_.base_velocity_world().angular()),
		// 	      std::ostream_iterator<double>(ss, ", "));

		// ss << "\nAcceleration: ";

		// std::copy(
		// 	std::cbegin(blind_state_msg_.base_acceleration_world().linear()),
		// 	std::cend(blind_state_msg_.base_acceleration_world().linear()),
		// 	std::ostream_iterator<double>(ss, ", "));

		// std::copy(
		// 	std::cbegin(blind_state_msg_.base_acceleration_world().angular()),
		// 	std::cend(blind_state_msg_.base_acceleration_world().angular()),
		// 	std::ostream_iterator<double>(ss, ", "));

		// ss << "\n===============================================================";
		// std::cout << ss.str() << std::endl;
	// }


	this->blind_state_pub_.publish(this->blind_state_msg_);
}

bool DlsRobotHwSim::freezeBase(std_srvs::Empty::Request& req, std_srvs::Empty::Response& res)
{
	freeze_cmd_=!freeze_state_;
	return true;
}

void DlsRobotHwSim::writeSim(ros::Time time, ros::Duration period)
{
	for (unsigned int i=0; i < sim_joints_.size(); i++) {
		// sim_joints_[i]->SetForce(0, joint_effort_command_[i]);
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


}
