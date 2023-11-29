#include "dls2/msg_wrappers/mpc_generator_output.hpp"

MPCGeneratorOutput::MPCGeneratorOutput(const std::shared_ptr<robotlib::RobotBase> robot)
	: frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, desired_base_poses_({15, {Eigen::Vector3d::Zero(), Eigen::Quaterniond::Identity()}})
	, desired_base_velocities_({15, {Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero()}})
	, desired_base_accelerations_({15, {Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero()}})
	, desired_joints_positions_({15, robot->makeJointState()})
	, desired_joints_velocities_({15, robot->makeJointState()})
	, desired_joints_accelerations_({15, robot->makeJointState()})
	, desired_torques_({15, robot->makeJointState()})
	, desired_feet_positions_({15, robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())})
	, desired_feet_velocities_({15, robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())})
	, desired_feet_accelerations_({15, robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())})
	, desired_ground_reaction_forces_({15, robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())})
	, desired_contacts_sequence_({15, robot->makeLegDataMap<bool>(false)})
{}

MPCGeneratorOutput::MPCGeneratorOutput(MPCGeneratorOutput& mpc_generator_output)
	: frame_id_(mpc_generator_output.frame_id_)
	, sequence_id_(mpc_generator_output.sequence_id_)
	, timestamp_(mpc_generator_output.timestamp_)
	, desired_base_poses_(mpc_generator_output.desired_base_poses_)
	, desired_base_velocities_(mpc_generator_output.desired_base_velocities_)
	, desired_base_accelerations_(mpc_generator_output.desired_base_accelerations_)
	, desired_joints_positions_(mpc_generator_output.desired_joints_positions_)
	, desired_joints_velocities_(mpc_generator_output.desired_joints_velocities_)
	, desired_joints_accelerations_(mpc_generator_output.desired_joints_accelerations_)
	, desired_torques_(mpc_generator_output.desired_torques_)
	, desired_feet_positions_(mpc_generator_output.desired_feet_positions_)
	, desired_feet_velocities_(mpc_generator_output.desired_feet_velocities_)
	, desired_feet_accelerations_(mpc_generator_output.desired_feet_accelerations_)
	, desired_ground_reaction_forces_(mpc_generator_output.desired_ground_reaction_forces_)
	, desired_contacts_sequence_(mpc_generator_output.desired_contacts_sequence_)
{}

MPCGeneratorOutput::~MPCGeneratorOutput(){}

MPCGeneratorOutput::operator MPCGeneratorOutputMsg() const
{
    MPCGeneratorOutputMsg mpc_generator_output_msg;

	mpc_generator_output_msg.frame_id(frame_id_);
	mpc_generator_output_msg.sequence_id(sequence_id_);
	mpc_generator_output_msg.timestamp(timestamp_);

	for(unsigned int i{0}; i<15; i++)
	{
		for(unsigned int j{0}; j<3; j++)
		{
			mpc_generator_output_msg.desired_base_positions()[i*3+j] = desired_base_poses_[i].toPosition()[j];
			mpc_generator_output_msg.desired_base_linear_velocities()[i*3+j] = desired_base_velocities_[i].getLinear()[j];
			mpc_generator_output_msg.desired_base_angular_velocities()[i*3+j] = desired_base_velocities_[i].getAngular()[j];
			mpc_generator_output_msg.desired_base_linear_accelerations()[i*3+j] = desired_base_accelerations_[i].getLinear()[j];
			mpc_generator_output_msg.desired_base_angular_accelerations()[i*3+j] = desired_base_accelerations_[i].getAngular()[j];
		}

		mpc_generator_output_msg.desired_base_orientations()[i*4] = desired_base_poses_[i].toQuaternion().x();
		mpc_generator_output_msg.desired_base_orientations()[i*4+1] = desired_base_poses_[i].toQuaternion().y();
		mpc_generator_output_msg.desired_base_orientations()[i*4+2] = desired_base_poses_[i].toQuaternion().z();
		mpc_generator_output_msg.desired_base_orientations()[i*4+3] = desired_base_poses_[i].toQuaternion().w();

		int leg_id{0};
		int leg_joint_id{0};
		for(const auto& leg : desired_joints_positions_[i])
		{
			leg_joint_id = 0;

			for(auto &joint : *leg.data_)
			{
				mpc_generator_output_msg.desired_joints_positions()[3*leg_id + leg_joint_id + 12*i] = desired_joints_positions_[i][joint.key_];
				mpc_generator_output_msg.desired_joints_velocities()[3*leg_id + leg_joint_id + 12*i] = desired_joints_velocities_[i][joint.key_];
				mpc_generator_output_msg.desired_joints_accelerations()[3*leg_id + leg_joint_id + 12*i] = desired_joints_accelerations_[i][joint.key_];
				mpc_generator_output_msg.desired_torques()[3*leg_id + leg_joint_id + 12*i] = desired_torques_[i][joint.key_];
				leg_joint_id++;
			}
			leg_id++;
		}

		int j{0};
		for (const auto& leg : desired_feet_positions_[i])
		{
			mpc_generator_output_msg.desired_feet_positions()[j+4*i] = desired_feet_positions_[i][leg.key_][0];
			mpc_generator_output_msg.desired_feet_positions()[j+4*i+1] = desired_feet_positions_[i][leg.key_][1];
			mpc_generator_output_msg.desired_feet_positions()[j+4*i+2] = desired_feet_positions_[i][leg.key_][2];

			mpc_generator_output_msg.desired_feet_velocities()[j+4*i] = desired_feet_velocities_[i][leg.key_][0];
			mpc_generator_output_msg.desired_feet_velocities()[j+4*i+1] = desired_feet_velocities_[i][leg.key_][1];
			mpc_generator_output_msg.desired_feet_velocities()[j+4*i+2] = desired_feet_velocities_[i][leg.key_][2];

			mpc_generator_output_msg.desired_feet_accelerations()[j+4*i] = desired_feet_accelerations_[i][leg.key_][0];
			mpc_generator_output_msg.desired_feet_accelerations()[j+4*i+1] = desired_feet_accelerations_[i][leg.key_][1];
			mpc_generator_output_msg.desired_feet_accelerations()[j+4*i+2] = desired_feet_accelerations_[i][leg.key_][2];

			mpc_generator_output_msg.desired_ground_reaction_forces()[j+4*i] = desired_ground_reaction_forces_[i][leg.key_][0];
			mpc_generator_output_msg.desired_ground_reaction_forces()[j+4*i+1] = desired_ground_reaction_forces_[i][leg.key_][1];
			mpc_generator_output_msg.desired_ground_reaction_forces()[j+4*i+2] = desired_ground_reaction_forces_[i][leg.key_][2];

			j++;
		}

		int k{0};
		for (const auto& leg : desired_contacts_sequence_[i])
		{
			mpc_generator_output_msg.desired_contacts_sequence()[k+4*i] = desired_contacts_sequence_[i][leg.key_];
			k++;
		}
	}

    return mpc_generator_output_msg;
}

MPCGeneratorOutput& MPCGeneratorOutput::operator=(const MPCGeneratorOutputMsg& mpc_generator_output_msg)
{
	frame_id_ = mpc_generator_output_msg.frame_id();
	sequence_id_ = mpc_generator_output_msg.sequence_id();
	timestamp_ = mpc_generator_output_msg.timestamp();

	for(unsigned int i{0}; i<15; i++)
	{
		desired_base_poses_[i].set(Eigen::Vector3d(mpc_generator_output_msg.desired_base_positions()[0 + i*3],
												   mpc_generator_output_msg.desired_base_positions()[1 + i*3],
												   mpc_generator_output_msg.desired_base_positions()[2 + i*3]));
		desired_base_poses_[i].set(Eigen::Quaterniond(mpc_generator_output_msg.desired_base_orientations()[3 + i*4],
												   	  mpc_generator_output_msg.desired_base_orientations()[0 + i*4],
												   	  mpc_generator_output_msg.desired_base_orientations()[1 + i*4],
												   	  mpc_generator_output_msg.desired_base_orientations()[2 + i*4]));
		desired_base_velocities_[i].setLinear(Eigen::Vector3d(mpc_generator_output_msg.desired_base_linear_velocities()[0 + i*3],
												   			  mpc_generator_output_msg.desired_base_linear_velocities()[1 + i*3],
												   			  mpc_generator_output_msg.desired_base_linear_velocities()[2 + i*3]));
		desired_base_velocities_[i].setAngular(Eigen::Vector3d(mpc_generator_output_msg.desired_base_angular_velocities()[0 + i*3],
												   			   mpc_generator_output_msg.desired_base_angular_velocities()[1 + i*3],
												   			   mpc_generator_output_msg.desired_base_angular_velocities()[2 + i*3]));
		desired_base_accelerations_[i].setLinear(Eigen::Vector3d(mpc_generator_output_msg.desired_base_linear_accelerations()[0 + i*3],
												   				 mpc_generator_output_msg.desired_base_linear_accelerations()[1 + i*3],
												   				 mpc_generator_output_msg.desired_base_linear_accelerations()[2 + i*3]));
		desired_base_accelerations_[i].setAngular(Eigen::Vector3d(mpc_generator_output_msg.desired_base_angular_accelerations()[0 + i*3],
												   				  mpc_generator_output_msg.desired_base_angular_accelerations()[1 + i*3],
												   				  mpc_generator_output_msg.desired_base_angular_accelerations()[2 + i*3]));

		int leg_id{0};
		for(const auto &leg : desired_joints_positions_[i])
		{
			int j = leg_id*leg.key_->getNJoints();

			for(auto &joint : *leg.data_)
			{
				desired_joints_positions_[i][joint.key_] = mpc_generator_output_msg.desired_joints_positions()[j + 12*i];
				desired_joints_velocities_[i][joint.key_] = mpc_generator_output_msg.desired_joints_velocities()[j + 12*i];
				desired_joints_accelerations_[i][joint.key_] = mpc_generator_output_msg.desired_joints_accelerations()[j + 12*i];
				desired_torques_[i][joint.key_] = mpc_generator_output_msg.desired_torques()[j + 12*i];
				j++;
			}

			leg_id++;
		}

		int j{0};
		for (auto &leg : desired_feet_positions_[i])
		{
			desired_feet_positions_[i][leg.key_][0] = mpc_generator_output_msg.desired_feet_positions()[j + i*4];
			desired_feet_positions_[i][leg.key_][1] = mpc_generator_output_msg.desired_feet_positions()[j + i*4 + 1];
			desired_feet_positions_[i][leg.key_][2] = mpc_generator_output_msg.desired_feet_positions()[j + i*4 + 2];

			desired_feet_velocities_[i][leg.key_][0] = mpc_generator_output_msg.desired_feet_velocities()[j + i*4];
			desired_feet_velocities_[i][leg.key_][1] = mpc_generator_output_msg.desired_feet_velocities()[j + i*4 + 1];
			desired_feet_velocities_[i][leg.key_][2] = mpc_generator_output_msg.desired_feet_velocities()[j + i*4 + 2];

			desired_feet_accelerations_[i][leg.key_][0] = mpc_generator_output_msg.desired_feet_accelerations()[j + i*4];
			desired_feet_accelerations_[i][leg.key_][1] = mpc_generator_output_msg.desired_feet_accelerations()[j + i*4 + 1];
			desired_feet_accelerations_[i][leg.key_][2] = mpc_generator_output_msg.desired_feet_accelerations()[j + i*4 + 2];

			desired_ground_reaction_forces_[i][leg.key_][0] = mpc_generator_output_msg.desired_ground_reaction_forces()[j + i*4];
			desired_ground_reaction_forces_[i][leg.key_][1] = mpc_generator_output_msg.desired_ground_reaction_forces()[j + i*4 + 1];
			desired_ground_reaction_forces_[i][leg.key_][2] = mpc_generator_output_msg.desired_ground_reaction_forces()[j + i*4 + 2];

			j++;
		}

		int k{0};
		for (auto &leg : desired_contacts_sequence_[i])
		{
			desired_contacts_sequence_[i][leg.key_] = mpc_generator_output_msg.desired_contacts_sequence()[k + i*4];
			k++;
		}
	}

	return *this;
}

MPCGeneratorOutput& MPCGeneratorOutput::operator=(const MPCGeneratorOutput& mpc_generator_output)
{
	frame_id_ = mpc_generator_output.frame_id_;
	sequence_id_ = mpc_generator_output.sequence_id_;
	timestamp_ = mpc_generator_output.timestamp_;

	desired_base_poses_ = mpc_generator_output.desired_base_poses_;
	desired_base_velocities_ = mpc_generator_output.desired_base_velocities_;
	desired_base_accelerations_ = mpc_generator_output.desired_base_accelerations_;
	desired_joints_positions_ = mpc_generator_output.desired_joints_positions_;
	desired_joints_velocities_ = mpc_generator_output.desired_joints_velocities_;
	desired_joints_accelerations_ = mpc_generator_output.desired_joints_accelerations_;
	desired_torques_ = mpc_generator_output.desired_torques_;
	desired_feet_positions_ = mpc_generator_output.desired_feet_positions_;
	desired_feet_velocities_ = mpc_generator_output.desired_feet_velocities_;
	desired_feet_accelerations_ = mpc_generator_output.desired_feet_accelerations_;
	desired_ground_reaction_forces_ = mpc_generator_output.desired_ground_reaction_forces_;
	desired_contacts_sequence_ = mpc_generator_output.desired_contacts_sequence_;

	return *this;
}