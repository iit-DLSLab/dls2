

#ifndef CONTROLLER_COMMAND_HPP
#define CONTROLLER_COMMAND_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/controller_command.hpp"
#include "dls2/msg_wrappers/pose.hpp"
#include "dls2/msg_wrappers/screw.hpp"
#include <robotlib/robot_base.hpp>

namespace dls
{
    class ControllerCommand : public Wrapper<ControllerCommandMsg>
    {
    public:
	    template <typename SignalType>
		friend class SignalWriter;
		friend class Plugin;

        ControllerCommand(const std::shared_ptr<robotlib::RobotBase> robot);
        ControllerCommand(ControllerCommand& controller_command);
        ControllerCommand() = delete;
        virtual ~ControllerCommand();

        operator ControllerCommandMsg() const override;
        ControllerCommand& operator= (const ControllerCommandMsg& controller_command_msg) override;
        ControllerCommand& operator=(const ControllerCommand& controller_command);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

        double robot_height_{};
        double step_frequency_{};
        double duty_factor_{};
        robotlib::LegDataMap<double> step_height_;

        Pose base_pose_HF_{};
        dls::Screw base_velocity_HF_{};
    
	private:
		virtual void setDataFromWrapperBase(WrapperBase* wrapper_base) override;
    };
} // namespace dls

#endif