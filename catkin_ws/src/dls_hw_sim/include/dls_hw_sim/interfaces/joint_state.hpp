#ifndef GAZEBO_ROS_CONTROL___DLS_GAZEBO_JOINT_STATE_H
#define GAZEBO_ROS_CONTROL___DLS_GAZEBO_JOINT_STATE_H


#include <hardware_interface/joint_state_interface.h>
#include <sensor_msgs/JointState.h>
#include <angles/angles.h>

namespace dls_hw_sim
{

	class JointStateRosInterface : public RosInterface<sensor_msgs::JointState> {
	public:
		JointStateRosInterface(std::vector<gazebo::physics::JointPtr> joint) : joint_(joint) { }

		virtual void initMsg() override {
			msg_.name.resize(joint_.size());
			msg_.position.resize(joint_.size());
			msg_.velocity.resize(joint_.size());
			msg_.effort.resize(joint_.size());
			msg_.header.frame_id = "joint";
			for (int i=0;i<joint_.size();i++) {
				msg_.name[i] = joint_[i]->GetName();
			}
		}

		virtual void fill() override {
			for (int i=0;i<joint_.size();i++)
			{
				msg_.position[i] += angles::shortest_angular_distance(msg_.position[i],joint_[i]->GetAngle(0).Radian());
				msg_.velocity[i] = joint_[i]->GetVelocity(0);
				msg_.effort[i] = joint_[i]->GetForce(0);
			}
			msg_.header.stamp = ros::Time::now();
		}
	private:
		std::vector<gazebo::physics::JointPtr>	joint_;
	};


	class DlsGazeboJointState : public GazeboHardwareInterface {
	public:
		DlsGazeboJointState() : ros(nullptr) {}

		~DlsGazeboJointState() { if (ros) delete(ros); }

		void init(gazebo::physics::ModelPtr sim_model, std::vector<transmission_interface::TransmissionInfo> transmissions, ros::NodeHandle &model_nh) {
			GazeboHardwareInterface::init(sim_model);

			for (auto const& transmission: transmissions) {
				if (transmission.joints_.size() == 1) {
					gazebo::physics::JointPtr joint = sim_model->GetJoint(transmission.joints_[0].name_);
					if (joint!=nullptr) {
						joint_.push_back(joint);
						name_.push_back(transmission.joints_[0].name_);
					}
				}
			}

			position_.resize(joint_.size());
			velocity_.resize(joint_.size());
			effort_.resize(joint_.size());
			
			ros = new JointStateRosInterface(joint_);
			ros->init(sim_model_,model_nh,"joint_state",1);
			add_interface(ros);
		}

		

		void fillJointStateInterface(ros::Time time)
		{
			for (unsigned int j=0; j < joint_.size(); j++) {
				if (!joint_[j]) continue;
				position_[j] += angles::shortest_angular_distance(position_[j],joint_[j]->GetAngle(0).Radian());
				velocity_[j] = joint_[j]->GetVelocity(0);
				effort_[j] = joint_[j]->GetForce((unsigned int)(0));
			}
		}


	public:
		std::vector<std::string>				name_;
		std::vector<double>						position_;
		std::vector<double>						velocity_;
		std::vector<double>						effort_;
	private:
		std::vector<gazebo::physics::JointPtr>	joint_;
		JointStateRosInterface *ros;
	};

}

#endif