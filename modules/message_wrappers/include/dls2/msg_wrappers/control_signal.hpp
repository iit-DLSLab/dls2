/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef CONTROL_SIGNAL_HPP_QCFRROHM
#define CONTROL_SIGNAL_HPP_QCFRROHM

#include "dls2/msg_wrappers/wrapper.hpp"
#include "dls_messages/dds/control_signalPubSubTypes.h"

#include "robotlib/robot_base.hpp"

/// A struct representing the control signal that is output by a Controller
namespace dls
{
    class ControlSignal : public Wrapper<ControlSignalMsg>
    {
    public:
        enum class SignalReconstructionMethod : uint64_t
        {
            ZERO_ORDER_HOLD,
            IMPULSE
        };

        ControlSignal(const std::shared_ptr<robotlib::RobotBase>);
        ControlSignal(const ControlSignal&);
        ControlSignal() = delete;
        ~ControlSignal();

        operator ControlSignalMsg() const override;
        ControlSignal& operator=(const ControlSignalMsg&) override;
        ControlSignal& operator=(const ControlSignal&);

        robotlib::JointState torques;
        SignalReconstructionMethod signal_reconstruction_method;
        double time;
    };
} // end namespace dls
#endif /* end of include guard: CONTROL_SIGNAL_HPP_QCFRROHM */
