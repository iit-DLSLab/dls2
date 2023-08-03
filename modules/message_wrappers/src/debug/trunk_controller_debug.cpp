#include "dls2/msg_wrappers/debug/trunk_controller_debug.hpp"

TrunkControllerDebug::TrunkControllerDebug(const std::shared_ptr<robotlib::RobotBase> robot)
    : frame_id_("")
	, sequence_id_(0)
	, timestamp_(0.0)
	, feet_forces_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
    , desired_forces_(robot->makeLegDataMap<Eigen::Vector3d>(Eigen::Vector3d::Zero()))
	, desired_wrench_(Eigen::Matrix<double,6,1>::Zero())
{}

TrunkControllerDebug::TrunkControllerDebug(TrunkControllerDebug& trunk_controller_debug)
    : frame_id_(trunk_controller_debug.frame_id_)
	, sequence_id_(trunk_controller_debug.sequence_id_)
	, timestamp_(trunk_controller_debug.timestamp_)
	, feet_forces_(trunk_controller_debug.feet_forces_)
	, desired_forces_(trunk_controller_debug.desired_forces_)
	, desired_wrench_(trunk_controller_debug.desired_wrench_)
{}

TrunkControllerDebug::~TrunkControllerDebug(){}

TrunkControllerDebug::operator TrunkControllerDebugMsg() const
{
    TrunkControllerDebugMsg trunk_controller_debug_msg;

	int i_leg{0};
	for(auto &leg : feet_forces_)
	{
		for(unsigned int i{0}; i<3; i++)
		{
			trunk_controller_debug_msg.feet_forces()[i_leg*3 + i] = feet_forces_[leg.key_][i];
			trunk_controller_debug_msg.desired_forces()[i_leg*3 + i] = desired_forces_[leg.key_][i];
		}
		i_leg++;
	}

	for(unsigned int i{0}; i<6; i++)
	{
		trunk_controller_debug_msg.desired_wrench()[i] = desired_wrench_(i);
	}

    return trunk_controller_debug_msg;
}

TrunkControllerDebug& TrunkControllerDebug::operator=(const TrunkControllerDebugMsg& trunk_controller_debug_msg)
{
	int i_leg{0};
	for(auto &leg : feet_forces_)
	{
		for(unsigned int i{0}; i<3; i++)
		{
			feet_forces_[leg.key_][i] = trunk_controller_debug_msg.feet_forces()[i_leg*3 + i];
			desired_forces_[leg.key_][i] = trunk_controller_debug_msg.desired_forces()[i_leg*3 + i];
		}
		i_leg++;
	}

	for(unsigned int i{0}; i<6; i++)
	{
		desired_wrench_(i) = trunk_controller_debug_msg.desired_wrench()[i];
	}

	return *this;
}

TrunkControllerDebug& TrunkControllerDebug::operator=(const TrunkControllerDebug& trunk_controller_debug)
{
    frame_id_ = trunk_controller_debug.frame_id_;
	sequence_id_ = trunk_controller_debug.sequence_id_;
	timestamp_ = trunk_controller_debug.timestamp_;

	feet_forces_ = trunk_controller_debug.feet_forces_;
	desired_forces_ = trunk_controller_debug.desired_forces_;
	desired_wrench_ = trunk_controller_debug.desired_wrench_;

	return *this;
}