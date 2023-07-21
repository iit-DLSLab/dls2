
#ifndef DESIRED_TORQUES_SIGNAL_HPP
#define DESIRED_TORQUES_SIGNAL_HPP


#include "robotlib/robot_base.hpp"

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/desired_torques.h"

namespace dls
{
	class DesiredTorques : public Wrapper<DesiredTorquesMsg>
	{
	public:
		DesiredTorques(const std::shared_ptr<robotlib::RobotBase>);
		DesiredTorques(DesiredTorques&);
		DesiredTorques() = delete;
        ~DesiredTorques();

		operator DesiredTorquesMsg() const override;
		DesiredTorques& operator= (const DesiredTorquesMsg&) override;
		dls::DesiredTorques& operator=(const dls::DesiredTorques& from);

		//data
		robotlib::JointState desired_torques;

		// header
		double time; // microsseconds
		int seq;
		std::string frame_id;

	};
} // end namespace dls

#endif /* end of include guard: DESIRED_TORQUES_SIGNAL_HPP */
