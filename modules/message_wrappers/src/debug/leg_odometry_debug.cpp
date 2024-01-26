
#include "dls2/msg_wrappers/debug/leg_odometry_debug.hpp"

LegOdometryDebug::LegOdometryDebug(const std::shared_ptr<robotlib::RobotBase> robot)
	: frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, position_ground_truth_(Eigen::Vector3d::Zero())
	, orientation_ground_truth_(Eigen::Quaterniond::Identity())
	, linear_velocity_ground_truth_(Eigen::Vector3d::Zero())
	, angular_velocity_ground_truth_(Eigen::Vector3d::Zero())
	, position_mean_(Eigen::Vector3d::Zero())
	, orientation_mean_(Eigen::Quaterniond::Identity())
	, linear_velocity_mean_(Eigen::Vector3d::Zero())
	, angular_velocity_mean_(Eigen::Vector3d::Zero())
	, position_error_(Eigen::Vector3d::Zero())
	, orientation_error_(Eigen::Quaterniond::Identity())
	, linear_velocity_error_(Eigen::Vector3d::Zero())
	, angular_velocity_error_(Eigen::Vector3d::Zero())	
	, position_legs_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , orientation_legs_((robot->makeLegDataMap<Eigen::Quaterniond>(Eigen::Quaterniond::Identity())))
    , linear_velocity_legs_((robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())))
    , angular_velocity_legs_((robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())))
    , linear_velocity_error_legs_((robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())))
    , angular_velocity_error_legs_((robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero())))
{}

LegOdometryDebug::LegOdometryDebug(LegOdometryDebug& leg_odometry_debug)
	: frame_id_(leg_odometry_debug.frame_id_)
	, sequence_id_(leg_odometry_debug.sequence_id_)
	, timestamp_(leg_odometry_debug.timestamp_)
	, position_ground_truth_(leg_odometry_debug.position_ground_truth_)
	, orientation_ground_truth_(leg_odometry_debug.orientation_ground_truth_)
	, linear_velocity_ground_truth_(leg_odometry_debug.linear_velocity_ground_truth_)
	, angular_velocity_ground_truth_(leg_odometry_debug.angular_velocity_ground_truth_)
	, position_mean_(leg_odometry_debug.position_mean_)
	, orientation_mean_(leg_odometry_debug.orientation_mean_)
	, linear_velocity_mean_(leg_odometry_debug.linear_velocity_mean_)
	, angular_velocity_mean_(leg_odometry_debug.angular_velocity_mean_)
	, position_error_(leg_odometry_debug.position_error_)
	, orientation_error_(leg_odometry_debug.orientation_error_)
	, linear_velocity_error_(leg_odometry_debug.linear_velocity_error_)
	, angular_velocity_error_(leg_odometry_debug.angular_velocity_error_)
    , position_legs_(leg_odometry_debug.position_legs_)
    , orientation_legs_(leg_odometry_debug.orientation_legs_)
    , linear_velocity_legs_(leg_odometry_debug.linear_velocity_legs_)
    , angular_velocity_legs_(leg_odometry_debug.angular_velocity_legs_)
    , linear_velocity_error_legs_(leg_odometry_debug.linear_velocity_error_legs_)
    , angular_velocity_error_legs_(leg_odometry_debug.angular_velocity_error_legs_)
{}

LegOdometryDebug::~LegOdometryDebug(){}

LegOdometryDebug::operator LegOdometryDebugMsg() const
{
    LegOdometryDebugMsg leg_odometry_debug_msg;

	leg_odometry_debug_msg.frame_id(frame_id_);
	leg_odometry_debug_msg.sequence_id(sequence_id_);
	leg_odometry_debug_msg.timestamp(timestamp_);

	for(unsigned int i{0}; i<3; i++)
	{
		leg_odometry_debug_msg.position_ground_truth()[i] = position_ground_truth_(i);
		leg_odometry_debug_msg.linear_velocity_ground_truth()[i] = linear_velocity_ground_truth_(i);
		leg_odometry_debug_msg.angular_velocity_ground_truth()[i] = angular_velocity_ground_truth_(i);
		leg_odometry_debug_msg.position_mean()[i] = position_mean_(i);
		leg_odometry_debug_msg.linear_velocity_mean()[i] = linear_velocity_mean_(i);
		leg_odometry_debug_msg.angular_velocity_mean()[i] = angular_velocity_mean_(i);
		leg_odometry_debug_msg.position_error()[i] = position_error_(i);
		leg_odometry_debug_msg.linear_velocity_error()[i] = linear_velocity_error_(i);
		leg_odometry_debug_msg.angular_velocity_error()[i] = angular_velocity_error_(i);
	}

	leg_odometry_debug_msg.orientation_ground_truth()[0] = orientation_ground_truth_.x();
	leg_odometry_debug_msg.orientation_ground_truth()[1] = orientation_ground_truth_.y();
	leg_odometry_debug_msg.orientation_ground_truth()[2] = orientation_ground_truth_.z();
	leg_odometry_debug_msg.orientation_ground_truth()[3] = orientation_ground_truth_.w();

	leg_odometry_debug_msg.orientation_mean()[0] = orientation_mean_.x();
	leg_odometry_debug_msg.orientation_mean()[1] = orientation_mean_.y();
	leg_odometry_debug_msg.orientation_mean()[2] = orientation_mean_.z();
	leg_odometry_debug_msg.orientation_mean()[3] = orientation_mean_.w();

	leg_odometry_debug_msg.orientation_error()[0] = orientation_error_.x();
	leg_odometry_debug_msg.orientation_error()[1] = orientation_error_.y();
	leg_odometry_debug_msg.orientation_error()[2] = orientation_error_.z();
	leg_odometry_debug_msg.orientation_error()[3] = orientation_error_.w();

	int i_leg{0};
	for(auto &leg : position_legs_)
	{
        leg_odometry_debug_msg.orientation_legs()[i_leg*4] = orientation_legs_[leg.key_].x();
        leg_odometry_debug_msg.orientation_legs()[i_leg*4+1] = orientation_legs_[leg.key_].y();
        leg_odometry_debug_msg.orientation_legs()[i_leg*4+2] = orientation_legs_[leg.key_].z();
        leg_odometry_debug_msg.orientation_legs()[i_leg*4+3] = orientation_legs_[leg.key_].w();

		for(unsigned int i{0}; i<3; i++)
		{
			leg_odometry_debug_msg.position_legs()[i_leg*3 + i] = position_legs_[leg.key_][i];
            leg_odometry_debug_msg.linear_velocity_legs()[i_leg*3 + i] = linear_velocity_legs_[leg.key_][i];
            leg_odometry_debug_msg.angular_velocity_legs()[i_leg*3 + i] = angular_velocity_legs_[leg.key_][i];
            leg_odometry_debug_msg.linear_velocity_error_legs()[i_leg*3 + i] = linear_velocity_error_legs_[leg.key_][i];
            leg_odometry_debug_msg.angular_velocity_error_legs()[i_leg*3 + i] = angular_velocity_error_legs_[leg.key_][i];
		}
		i_leg++;
	}

    return leg_odometry_debug_msg;
}

LegOdometryDebug& LegOdometryDebug::operator=(const LegOdometryDebugMsg& leg_odometry_debug_msg)
{
	frame_id_ = leg_odometry_debug_msg.frame_id();
	sequence_id_ = leg_odometry_debug_msg.sequence_id();
	timestamp_ = leg_odometry_debug_msg.timestamp();

	position_ground_truth_ = Eigen::Vector3d(leg_odometry_debug_msg.position_ground_truth().data());
	linear_velocity_ground_truth_ = Eigen::Vector3d(leg_odometry_debug_msg.linear_velocity_ground_truth().data());
	angular_velocity_ground_truth_ = Eigen::Vector3d(leg_odometry_debug_msg.angular_velocity_ground_truth().data());
	position_mean_ = Eigen::Vector3d(leg_odometry_debug_msg.position_mean().data());
	linear_velocity_mean_ = Eigen::Vector3d(leg_odometry_debug_msg.linear_velocity_mean().data());
	angular_velocity_mean_ = Eigen::Vector3d(leg_odometry_debug_msg.angular_velocity_mean().data());
	position_error_ = Eigen::Vector3d(leg_odometry_debug_msg.position_error().data());
	linear_velocity_error_ = Eigen::Vector3d(leg_odometry_debug_msg.linear_velocity_error().data());
	angular_velocity_error_ = Eigen::Vector3d(leg_odometry_debug_msg.angular_velocity_error().data());

	orientation_ground_truth_.x() = leg_odometry_debug_msg.orientation_ground_truth()[0];
	orientation_ground_truth_.y() = leg_odometry_debug_msg.orientation_ground_truth()[1];
	orientation_ground_truth_.z() = leg_odometry_debug_msg.orientation_ground_truth()[2];
	orientation_ground_truth_.w() = leg_odometry_debug_msg.orientation_ground_truth()[3];

	orientation_mean_.x() = leg_odometry_debug_msg.orientation_mean()[0];
	orientation_mean_.y() = leg_odometry_debug_msg.orientation_mean()[1];
	orientation_mean_.z() = leg_odometry_debug_msg.orientation_mean()[2];
	orientation_mean_.w() = leg_odometry_debug_msg.orientation_mean()[3];

	orientation_error_.x() = leg_odometry_debug_msg.orientation_error()[0];
	orientation_error_.y() = leg_odometry_debug_msg.orientation_error()[1];
	orientation_error_.z() = leg_odometry_debug_msg.orientation_error()[2];
	orientation_error_.w() = leg_odometry_debug_msg.orientation_error()[3];

	int i_leg{0};
	for(auto &leg : position_legs_)
	{
        orientation_legs_[leg.key_].x() = leg_odometry_debug_msg.orientation_legs()[i_leg*4];
        orientation_legs_[leg.key_].y() = leg_odometry_debug_msg.orientation_legs()[i_leg*4+1];
        orientation_legs_[leg.key_].z() = leg_odometry_debug_msg.orientation_legs()[i_leg*4+2];
        orientation_legs_[leg.key_].w() = leg_odometry_debug_msg.orientation_legs()[i_leg*4+3];

		for(int i=0;i<3;i++)
		{
			position_legs_[leg.key_][i] = leg_odometry_debug_msg.position_legs()[i_leg*3+i];
			linear_velocity_legs_[leg.key_][i] = leg_odometry_debug_msg.linear_velocity_legs()[i_leg*3+i];
			angular_velocity_legs_[leg.key_][i] = leg_odometry_debug_msg.angular_velocity_legs()[i_leg*3+i];
			linear_velocity_error_legs_[leg.key_][i] = leg_odometry_debug_msg.linear_velocity_error_legs()[i_leg*3+i];
			angular_velocity_error_legs_[leg.key_][i] = leg_odometry_debug_msg.angular_velocity_error_legs()[i_leg*3+i];
		}

		i_leg++;
	}

	return *this;
}

LegOdometryDebug& LegOdometryDebug::operator=(const LegOdometryDebug& leg_odometry_debug)
{
	frame_id_ = leg_odometry_debug.frame_id_;
	sequence_id_ = leg_odometry_debug.sequence_id_;
	timestamp_ = leg_odometry_debug.timestamp_;

	position_ground_truth_ = leg_odometry_debug.position_ground_truth_;
	orientation_ground_truth_ = leg_odometry_debug.orientation_ground_truth_;
	linear_velocity_ground_truth_ = leg_odometry_debug.linear_velocity_ground_truth_;
	angular_velocity_ground_truth_ = leg_odometry_debug.angular_velocity_ground_truth_;

	position_mean_ = leg_odometry_debug.position_mean_;
	orientation_mean_ = leg_odometry_debug.orientation_mean_;
	linear_velocity_mean_ = leg_odometry_debug.linear_velocity_mean_;
	angular_velocity_mean_ = leg_odometry_debug.angular_velocity_mean_;

	position_error_ = leg_odometry_debug.position_error_;
	orientation_error_ = leg_odometry_debug.orientation_error_;
	linear_velocity_error_ = leg_odometry_debug.linear_velocity_error_;
	angular_velocity_error_ = leg_odometry_debug.angular_velocity_error_;

    position_legs_ = leg_odometry_debug.position_legs_;
    orientation_legs_ = leg_odometry_debug.orientation_legs_;
    linear_velocity_legs_ = leg_odometry_debug.linear_velocity_legs_;
    angular_velocity_legs_ = leg_odometry_debug.angular_velocity_legs_;
    linear_velocity_error_legs_ = leg_odometry_debug.linear_velocity_error_legs_;
    angular_velocity_error_legs_ = leg_odometry_debug.angular_velocity_error_legs_;

	return *this;
}

void LegOdometryDebug::setDataFromWrapperBase(WrapperBase *wrapper_base)
{
	// Assuming wrapper is of type Wrapper<MsgType>
	*this = static_cast<LegOdometryDebug &>(*wrapper_base);
}