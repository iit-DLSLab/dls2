
#ifndef DESIRED_TORQUES_SIGNAL_HPP
#define DESIRED_TORQUES_SIGNAL_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/desired_torques.h"
#include "robotlib/robot_base.hpp"

namespace dls
{
	class DesiredTorques : public Wrapper<DesiredTorquesMsg>
	{
	public:
	    template <typename SignalType>
		friend class SignalWriter;
		friend class PluginBase;

		DesiredTorques(const std::shared_ptr<robotlib::RobotBase> robot);
		DesiredTorques(DesiredTorques& desired_torques);
		DesiredTorques() = delete;
        virtual ~DesiredTorques();

		operator DesiredTorquesMsg() const override;
		DesiredTorques& operator= (const DesiredTorquesMsg& desired_torques_msg) override;
		DesiredTorques& operator=(const dls::DesiredTorques& desired_torques);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

		robotlib::JointState desired_torques_;
	private:
		virtual void setDataFromWrapperBase(WrapperBase* wrapper_base) override;
	};
} // namespace dls

#endif