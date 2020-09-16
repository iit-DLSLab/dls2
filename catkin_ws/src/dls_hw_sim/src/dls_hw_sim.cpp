#include "dls_hw_sim/dls_hw_sim.hpp"

namespace dls_hw_sim
{

void DlsRobotHwSim::Load
(
	gazebo::physics::ModelPtr model,
	sdf::ElementPtr
)
{
	this->sim_model_ = model;
	this->update_connection = gazebo::event::Events::ConnectWorldUpdateBegin
	(
		std::bind(&DlsRobotHwSim::onGazeboUpdate, this)
	);
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
			const std::string &hardware_interface = transmissions[j].actuators_[0].hardware_interfaces_[0];
			ROS_ERROR_STREAM("Loading joint '" << joint_name_[j] << "' of type '" << hardware_interface << "'");
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
	ROS_ERROR_STREAM("READSIM");
	// Fill ROS Control Interfaces
	ros::Time t = ros::Time::now();

	//Fill ROS Control
	dls_gazebo_joint_state_.fillJointStateInterface(t);
	dls_gazebo_imu_kvh_.fillImuKvhInterface(t);
	dls_gazebo_imu_mgx_.fillImuMgxInterface(t);
	dls_gazebo_blind_state_.fillBlindStateInterface(t);
	//dls_gazebo_hyq_raw_.fillHyqRawInterface(t);

	// Fill ROS Messages
	//dls_gazebo_hyq_raw_.fillAndPublish();
	dls_gazebo_blind_state_.fillAndPublish();
	dls_gazebo_imu_mgx_.fillAndPublish();
	dls_gazebo_imu_kvh_.fillAndPublish();
	dls_gazebo_joint_state_.fillAndPublish();
	dls_gazebo_imu_sensor_.fillAndPublish();
	dls_gazebo_odometry_.fillAndPublish();

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
