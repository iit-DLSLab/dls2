#ifndef GAZEBO_ROS_CONTROL___DLS_GAZEBO_ODOMETRY_H
#define GAZEBO_ROS_CONTROL___DLS_GAZEBO_ODOMETRY_H

#include <nav_msgs/Odometry.h>

namespace dls_hw_sim
{

	class OdometryRosInterface : public RosInterface<nav_msgs::Odometry> {
	public:
		virtual void initMsg() override {
			msg_.child_frame_id = "base";
			for (int i=0;i<36;i++)
			{
				msg_.pose.covariance[i]=0.0;
				msg_.twist.covariance[i]=0.0;
			}
			
			for (int i=0;i<6;i++)
			{
				msg_.pose.covariance[i*7]=1.0;
				msg_.twist.covariance[i*7]=1.0;
			}
		}

		virtual void fill() override {
			msg_.header.stamp = ros::Time::now();
			msg_.pose.pose.position.x = sim_model_->GetWorldPose().pos.x;
			msg_.pose.pose.position.y = sim_model_->GetWorldPose().pos.y;
			msg_.pose.pose.position.z = sim_model_->GetWorldPose().pos.z;
			msg_.pose.pose.orientation.w = sim_model_->GetWorldPose().rot.w;
			msg_.pose.pose.orientation.x = sim_model_->GetWorldPose().rot.x;
			msg_.pose.pose.orientation.y = sim_model_->GetWorldPose().rot.y;
			msg_.pose.pose.orientation.z = sim_model_->GetWorldPose().rot.z;
			//odometry_msg_.pose.covariance left at I
			msg_.twist.twist.linear.x = sim_model_->GetWorldLinearVel().x;
			msg_.twist.twist.linear.y = sim_model_->GetWorldLinearVel().y;
			msg_.twist.twist.linear.z = sim_model_->GetWorldLinearVel().z;
			msg_.twist.twist.angular.x = sim_model_->GetWorldAngularVel().x;
			msg_.twist.twist.angular.y = sim_model_->GetWorldAngularVel().y;
			msg_.twist.twist.angular.z = sim_model_->GetWorldAngularVel().z;
			//odometry_msg_.twist.covariance left at I}
		}
	};

	class DlsGazeboOdometry : public GazeboHardwareInterface {
	public:

		DlsGazeboOdometry() : ros(nullptr) { }

		~DlsGazeboOdometry() { if (ros) delete(ros); }


		void init(gazebo::physics::ModelPtr sim_model,ros::NodeHandle &model_nh) {
			GazeboHardwareInterface::init(sim_model);
			ros = new OdometryRosInterface();
			ros->init(sim_model_,model_nh,"ground_truth",1);
			add_interface(ros);
		}


	private:
		OdometryRosInterface *ros;
	};

}

#endif