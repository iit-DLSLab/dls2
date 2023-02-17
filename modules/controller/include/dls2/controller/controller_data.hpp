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
#ifndef CONTROLLER_DATA_HPP
#define CONTROLLER_DATA_HPP

#include "dls2/components/app_data.hpp"
#include "dls2/msg_wrappers/control_signal.hpp"
#include "dls2/msg_wrappers/signal_reader.hpp"
#include "dls2/math/ramp.hpp"

#include <memory>

namespace dls
{
    class ControllerData : public AppData
    {
    public:
        ControllerData
        (
            const std::string&,
            dls::DDSParticipant* participant,
            const dls::topicType& topic,
            const std::shared_ptr<robotlib::RobotBase>,
            std::shared_ptr<math::SplineBase<double>> pSpline_in,
            std::shared_ptr<math::SplineBase<double>> pSpline_out,
            const std::chrono::duration<double> &duration_in,
            const std::chrono::duration<double> &duration_out
        );

        std::atomic<double> premultiplier; ///< Spline value to premutilply the torque signal
        const std::chrono::duration<double> spline_in_duration;
        const std::chrono::duration<double> spline_out_duration;
        const std::shared_ptr<math::SplineBase<double>> pSpline_in;
        const std::shared_ptr<math::SplineBase<double>> pSpline_out;

        SignalReader<ControlSignal> control_signal;
    };
}// end namespace dls

#endif /* end of include guard: CONTROLLER_DATA_HPP */
