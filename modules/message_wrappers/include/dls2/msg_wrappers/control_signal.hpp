
#ifndef CONTROL_SIGNAL_HPP
#define CONTROL_SIGNAL_HPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/control_signal.h"
#include "robotlib/robot_base.hpp"

/// A struct representing the control signal that is output by a Controller
namespace dls
{
    class ControlSignal : public Wrapper<ControlSignalMsg>
    {
    public:
	    template <typename SignalType>
		friend class SignalWriter;
		friend class PeriodicPluginBase;

        enum class SignalReconstructionMethod : uint64_t
        {
            ZERO_ORDER_HOLD,
            IMPULSE
        };

        ControlSignal(const std::shared_ptr<robotlib::RobotBase> robot);
        ControlSignal(const ControlSignal& control_signal);
        ControlSignal() = delete;
        virtual ~ControlSignal();

        operator ControlSignalMsg() const override;
        ControlSignal& operator=(const ControlSignalMsg& control_signal_msg) override;
        ControlSignal& operator=(const ControlSignal& control_signal);

		std::string frame_id_{};
		uint32_t sequence_id_{};
		double timestamp_{};

        robotlib::JointState torques_;
        SignalReconstructionMethod signal_reconstruction_method_{};
    private:
		virtual void setDataFromWrapperBase(WrapperBase* wrapper_base) override;
    };
} // namespace dls

#endif