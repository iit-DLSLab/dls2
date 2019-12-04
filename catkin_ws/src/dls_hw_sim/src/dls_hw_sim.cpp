#include <cassert>
#include <gazebo/sensors/SensorManager.hh>
#include <gazebo/math/Vector3.hh>
#include <gazebo/common/common.hh>
#include <geometry_msgs/Vector3Stamped.h>

#include "dls_hw_sim/dls_hw_sim.h"

PLUGINLIB_EXPORT_CLASS(dls_hw_sim::DlsRobotHwSim, gazebo_ros_control::RobotHWSim)


// dls2
#include "util/messaging/publisher_base.hpp"
#include <memory>
#include "msg/boolPubSubTypes.h"
#include "topics/simulation_pause.hpp"
std::shared_ptr<dls::PublisherBase<BoolMsgPubSubType>> pPause_pub;

void callback_pause(bool is_paused)
{
	std::cout << "pause callback triggered" << std::endl;
	BoolMsg msg;
	msg.val(is_paused);
	pPause_pub->publish(msg);
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

	// Resize vectors to our DOF
	n_dof_ = transmissions.size();
	joint_name_.resize(n_dof_);
	joint_types_.resize(n_dof_);
	joint_lower_limits_.resize(n_dof_);
	joint_upper_limits_.resize(n_dof_);
	joint_effort_limits_.resize(n_dof_);
	joint_position_.resize(n_dof_);
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
	
	pPause_pub = std::make_shared<dls::PublisherBase<BoolMsgPubSubType>>(dls::topics::simulation_pause);
	pause_connection =  gazebo::event::Events::ConnectPause(std::function<void(bool)>(callback_pause));
	// gazebo::event::Events::ConnectPause();

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
	
/*	base_orientation_.resize(4);
	base_ang_vel_.resize(3);
	base_ang_acc_.resize(3);
	base_lin_acc_.resize(3);
	base_lin_pos_.resize(3);
	base_lin_vel_.resize(3);
	std::string frame("base_link"); //Some quick hack to pass the data over; */


	// Register interfaces
	registerInterface(&joint_command_interface_);
	registerInterface(&joint_state_interface_);
	registerInterface(&imu_sensor_interface_);
	registerInterface(&blind_state_interface_);
	
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
	//odometry_msg_.twist.covariance left at I
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
		joint_acceleration_[j] = 0.0; // TODO
		joint_effort_[j] = sim_joints_[j]->GetForce((unsigned int)(0));
	}
	fillJointStateMsgAndPublish(t);
	fillOdometryMsgAndPublish(t);
	fillBlindStateMsgAndPublish(t);

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
	// std::cout << "world is paused? " << std::endl;sim_model_->GetWorld()->IsPaused() <<std::endl;;
}

bool DlsRobotHwSim::checkForConflict(const std::list<hardware_interface::ControllerInfo>& info) const
{
	ROS_ERROR("CHECK FOR CONFLICT");
	return false; // TODO All controllers can run at the same time! dangerous and bad
}





}
