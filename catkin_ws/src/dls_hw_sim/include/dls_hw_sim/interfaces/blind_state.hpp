#ifndef GAZEBO_ROS_CONTROL___DLS_GAZEBO_BLIND_STATE_H
#define GAZEBO_ROS_CONTROL___DLS_GAZEBO_BLIND_STATE_H

#include <dls2_msgs/BlindState.h>
#include <dls2_hardware_interface/blind_state_interface.h>
#include <angles/angles.h>

namespace dls_hw_sim
{

	class BlindStateRosInterface : public RosInterface<dls2_msgs::BlindState> {
	public:
		BlindStateRosInterface(std::vector<gazebo::physics::JointPtr> joint) : joint_(joint) { }
		virtual void initMsg() override { msg_.header.frame_id = "base"; }
		virtual void fill() override {
			msg_.header.stamp = ros::Time::now();
			for (int i=0;i<12;i++)
			{
				msg_.joint_state.position[i] += angles::shortest_angular_distance(msg_.joint_state.position[i],joint_[i]->GetAngle(0).Radian());
				msg_.joint_state.velocity[i] = joint_[i]->GetVelocity(0);
				msg_.joint_state.effort[i] = joint_[i]->GetForce(0);
			}

			msg_.base_pose_world.position[0] = sim_model_->GetWorldPose().pos.x;
			msg_.base_pose_world.position[1] = sim_model_->GetWorldPose().pos.y;
			msg_.base_pose_world.position[2] = sim_model_->GetWorldPose().pos.z;

			// Filled in order x y z w to conform to Eigen's internal representation
			msg_.base_pose_world.quaternion[0] = sim_model_->GetWorldPose().rot.x;
			msg_.base_pose_world.quaternion[1] = sim_model_->GetWorldPose().rot.y;
			msg_.base_pose_world.quaternion[2] = sim_model_->GetWorldPose().rot.z;
			msg_.base_pose_world.quaternion[3] = sim_model_->GetWorldPose().rot.w;

			msg_.base_velocity_world.linear[0] = sim_model_->GetWorldLinearVel().x;
			msg_.base_velocity_world.linear[1] = sim_model_->GetWorldLinearVel().y;
			msg_.base_velocity_world.linear[2] = sim_model_->GetWorldLinearVel().z;
			msg_.base_velocity_world.angular[0] = sim_model_->GetWorldAngularVel().x;
			msg_.base_velocity_world.angular[1] = sim_model_->GetWorldAngularVel().y;
			msg_.base_velocity_world.angular[2] = sim_model_->GetWorldAngularVel().z;

			msg_.base_acceleration_world.linear[0] = sim_model_->GetWorldLinearAccel().x;
			msg_.base_acceleration_world.linear[1] = sim_model_->GetWorldLinearAccel().y;
			msg_.base_acceleration_world.linear[2] = sim_model_->GetWorldLinearAccel().z;
			msg_.base_acceleration_world.angular[0] = sim_model_->GetWorldLinearAccel().x;
			msg_.base_acceleration_world.angular[1] = sim_model_->GetWorldLinearAccel().y;
			msg_.base_acceleration_world.angular[2] = sim_model_->GetWorldLinearAccel().z;

		}
	private:
		std::vector<gazebo::physics::JointPtr>	joint_;
	};

	class DlsGazeboBlindState : public GazeboHardwareInterface {
	public:

		DlsGazeboBlindState() : ros(nullptr) {

			data_.name = "blind_state"; // TODO
			data_.joint_position = &joint_position_[0];
			data_.joint_velocity = &joint_velocity_[0];
			data_.joint_effort = &joint_effort_[0];
			data_.base_pose_world = &base_pose_world_[0];
			data_.base_velocity_world = &base_velocity_world_[0];
			data_.base_acceleration_world = &base_acceleration_world_[0];
		}

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

			joint_position_.resize(joint_.size());
			data_.joint_position = &joint_position_[0];
			joint_velocity_.resize(joint_.size());
			data_.joint_velocity = &joint_velocity_[0];
			joint_effort_.resize(joint_.size());
			data_.joint_effort = &joint_effort_[0];

			ros = new BlindStateRosInterface(joint_);
			ros->init(sim_model_,model_nh,"blind_state",1);
			add_interface(ros);
		}

		void fillBlindStateInterface(ros::Time time)
		{
			for (unsigned int j=0; j < joint_.size(); j++) {
				joint_position_[j] += angles::shortest_angular_distance(joint_position_[j],joint_[j]->GetAngle(0).Radian());
				joint_velocity_[j] = joint_[j]->GetVelocity(0);
				joint_effort_[j] = joint_[j]->GetForce((unsigned int)(0));
			}
			base_pose_world_[0] = sim_model_->GetWorldPose().pos.x;
			base_pose_world_[1] = sim_model_->GetWorldPose().pos.y;
			base_pose_world_[2] = sim_model_->GetWorldPose().pos.z;
			base_pose_world_[3] = sim_model_->GetWorldPose().rot.x;
			base_pose_world_[4] = sim_model_->GetWorldPose().rot.y;
			base_pose_world_[5] = sim_model_->GetWorldPose().rot.z;
			base_pose_world_[6] = sim_model_->GetWorldPose().rot.w;

			base_velocity_world_[0] = sim_model_->GetWorldLinearVel().x;
			base_velocity_world_[1] = sim_model_->GetWorldLinearVel().y;
			base_velocity_world_[2] = sim_model_->GetWorldLinearVel().z;
			base_velocity_world_[3] = sim_model_->GetWorldAngularVel().x;
			base_velocity_world_[4] = sim_model_->GetWorldAngularVel().y;
			base_velocity_world_[5] = sim_model_->GetWorldAngularVel().z;

			base_acceleration_world_[0] = sim_model_->GetWorldLinearAccel().x;
			base_acceleration_world_[1] = sim_model_->GetWorldLinearAccel().y;
			base_acceleration_world_[2] = sim_model_->GetWorldLinearAccel().z;
			base_acceleration_world_[3] = sim_model_->GetWorldAngularAccel().x;
			base_acceleration_world_[4] = sim_model_->GetWorldAngularAccel().y;
			base_acceleration_world_[5] = sim_model_->GetWorldAngularAccel().z;

		}

		


	public:
		hardware_interface::BlindStateHandle::Data data_;
	private:
		std::vector<std::string>				name_;
		std::vector<gazebo::physics::JointPtr>	joint_;
		BlindStateRosInterface *ros;
		std::string blind_state_name_;
		std::vector<double> joint_position_;
		std::vector<double> joint_velocity_;
		std::vector<double> joint_effort_;
		std::array<double,7> base_pose_world_;
		std::array<double,6> base_velocity_world_;
		std::array<double,6> base_acceleration_world_;


	};

}

#endif