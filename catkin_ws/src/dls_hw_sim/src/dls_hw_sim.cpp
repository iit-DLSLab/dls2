#include <cassert>
#include <gazebo/sensors/SensorManager.hh>
#include <gazebo/math/Vector3.hh>
#include <geometry_msgs/Vector3Stamped.h>

#include "dls_hw_sim/dls_hw_sim.h"

PLUGINLIB_EXPORT_CLASS(dls_hw_sim::DlsRobotHwSim, gazebo_ros_control::RobotHWSim)

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

	// Resize vectors to our DOF
	n_dof_ = transmissions.size();
	joint_name_.resize(n_dof_);
	joint_types_.resize(n_dof_);
	joint_lower_limits_.resize(n_dof_);
	joint_upper_limits_.resize(n_dof_);
	joint_effort_limits_.resize(n_dof_);
	joint_position_.resize(n_dof_);
	joint_velocity_.resize(n_dof_);
	joint_effort_.resize(n_dof_);
	joint_effort_command_.resize(n_dof_);
	joint_state_msg_.name.resize(n_dof_);
	joint_state_msg_.position.resize(n_dof_);
	joint_state_msg_.velocity.resize(n_dof_);
	joint_state_msg_.effort.resize(n_dof_);
	joint_state_msg_.header.frame_id = "joint";


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
		joint_effort_[j] = 0.0;
		joint_effort_command_[j] = 0.0;

		joint_state_msg_.name[j] = joint_name_[j];  // IMPORTANT - Joint Order depends on the proper joint order in the urdf!
		
		if (transmissions[j].actuators_[0].hardware_interfaces_.size()>0) {
			const std::string &hardware_interface = transmissions[j].actuators_[0].hardware_interfaces_[0];
			ROS_ERROR_STREAM("Loading joint '" << joint_name_[j] << "' of type '" << hardware_interface << "'");
		}
		else
		{
			ROS_ERROR_STREAM(joint_name_[j] << " does not have an associated hardware interface.");	
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
	imu_ang_vel_.resize(3);
	imu_lin_acc_.resize(3);

	base_orientation_.resize(4);
	base_ang_vel_.resize(3);
	base_ang_acc_.resize(3);
	base_lin_acc_.resize(3);
	base_lin_pos_.resize(3);
	base_lin_vel_.resize(3);

	imuData.name = "trunk_imu"; // TODO: Fetch from elsewhere?
	imuData.frame_id = "trunk_imu"; // TODO: Fetch from URDF?
	imuData.orientation = &imu_orientation_[0];
	imuData.angular_velocity = &imu_ang_vel_[0];
	imuData.linear_acceleration = &imu_lin_acc_[0];
	imu_sensor_interface_.registerHandle(hardware_interface::ImuSensorHandle(imuData));


	std::string frame("base_link"); //Some quick hack to pass the data over;


	// Register interfaces
	registerInterface(&joint_command_interface_);
	registerInterface(&joint_state_interface_);
	registerInterface(&imu_sensor_interface_);
	freeze_cmd_=true;
	freeze_state_=false;
    freeze_base_srv_ = model_nh.advertiseService("freeze_base", &DlsRobotHwSim::freezeBase, this);

	return true;
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
}

void DlsRobotHwSim::fillJointStateMsgAndPublish(ros::Time time)
{
	fillJointStateMsg(time);
	joint_state_pub_.publish(joint_state_msg_);
}

void DlsRobotHwSim::fillOdometryMsgAndPublish(ros::Time time)
{
	fillOdometryMsg(time);
	odometry_pub_.publish(odometry_msg_);
}

void DlsRobotHwSim::readSim(ros::Time time, ros::Duration period)
{
	ros::Time t = ros::Time::now(); 
	for (unsigned int j=0; j < sim_joints_.size(); j++) {
		// Gazebo has an interesting API...
		if (joint_types_[j] == urdf::Joint::PRISMATIC) {
			joint_position_[j] = sim_joints_[j]->GetAngle(0).Radian();
		} else {
			joint_position_[j] += angles::shortest_angular_distance(joint_position_[j],
                              	  sim_joints_[j]->GetAngle(0).Radian());
		}
		joint_velocity_[j] = sim_joints_[j]->GetVelocity(0);
		joint_effort_[j] = sim_joints_[j]->GetForce((unsigned int)(0));
	}
	fillJointStateMsgAndPublish(t);
	fillOdometryMsgAndPublish(t);

/*
	//Ground truth:

	gazebo::math::Vector3  gzLinearVel = sim_model_->GetWorldLinearVel();
	base_lin_vel_[0] = gzLinearVel.x;
	base_lin_vel_[1] = gzLinearVel.y;
	base_lin_vel_[2] = gzLinearVel.z;

	//gazebo::math::Vector3  gzLinearAcc = sim_model_->GetLink("base_link")->GetLinearAccel(); //not working
  base_lin_acc_[0] = (base_lin_vel_[0] - base_lin_vel_old_[0])/0.001;
  base_lin_acc_[1] = (base_lin_vel_[1] - base_lin_vel_old_[1])/0.001;
  base_lin_acc_[2] = (base_lin_vel_[2] - base_lin_vel_old_[2])/0.001;
  base_lin_vel_old_[0] = base_lin_vel_[0];
  base_lin_vel_old_[1] = base_lin_vel_[1];
  base_lin_vel_old_[2] = base_lin_vel_[2];

	gazebo::math::Vector3  gzAngularVel = sim_model_->GetWorldAngularVel();
	base_ang_vel_[0] = gzAngularVel.x;
	base_ang_vel_[1] = gzAngularVel.y;
	base_ang_vel_[2] = gzAngularVel.z;

	//gazebo::math::Vector3  gzAngularAcc = sim_model_->GetWorldAngularAccel();//not working
    base_ang_acc_[0] = (base_ang_vel_[0] - base_ang_vel_old_[0])/0.001;
    base_ang_acc_[1] = (base_ang_vel_[1] - base_ang_vel_old_[1])/0.001;
    base_ang_acc_[2] = (base_ang_vel_[2] - base_ang_vel_old_[2])/0.001;
    base_ang_vel_old_[0] = base_lin_vel_[0];
    base_ang_vel_old_[1] = base_lin_vel_[1];
    base_ang_vel_old_[2] = base_lin_vel_[2];

	gazebo::math::Pose gzPose = sim_model_->GetWorldPose();
	base_lin_pos_[0] = gzPose.pos.x;
	base_lin_pos_[1] = gzPose.pos.y;
	base_lin_pos_[2] = gzPose.pos.z;
	base_orientation_[0] = gzPose.rot.w;
	base_orientation_[1] = gzPose.rot.x;
	base_orientation_[2] = gzPose.rot.y;
	base_orientation_[3] = gzPose.rot.z;

    //IMU data:

    gazebo::math::Quaternion imu_quat(1, 0, 0, 0);
    gazebo::math::Vector3 imu_ang_vel(0, 0, 0);
    gazebo::math::Vector3 imu_lin_acc(0, 0, 0);

    if(imu_sensor_ != NULL){
        imu_quat = imu_sensor_->Orientation();
        imu_ang_vel = imu_sensor_->AngularVelocity();
        imu_lin_acc = imu_sensor_->LinearAcceleration();
    }
    else
    {
       imu_quat.w = gzPose.rot.w;
       imu_quat.x = gzPose.rot.x;
       imu_quat.y = gzPose.rot.y;
       imu_quat.z = gzPose.rot.z;

       imu_ang_vel.x = gzAngularVel.x;
       imu_ang_vel.y = gzAngularVel.y;
       imu_ang_vel.z = gzAngularVel.z;

       imu_lin_acc.x =  base_ang_acc_[0];
       imu_lin_acc.y =  base_ang_acc_[1];
       imu_lin_acc.z =  base_ang_acc_[2];
    }

    imu_orientation_[0] = imu_quat.w;
    imu_orientation_[1] = imu_quat.x;
    imu_orientation_[2] = imu_quat.y;
    imu_orientation_[3] = imu_quat.z;

    imu_ang_vel_[0] = imu_ang_vel.x;
    imu_ang_vel_[1] = imu_ang_vel.y;
    imu_ang_vel_[2] = imu_ang_vel.z;

    imu_lin_acc_[0] = imu_lin_acc.x;
    imu_lin_acc_[1] = imu_lin_acc.y;
    imu_lin_acc_[2] = imu_lin_acc.z;


*/


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

}





}
