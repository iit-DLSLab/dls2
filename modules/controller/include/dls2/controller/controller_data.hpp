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

#include "dls2/msg_wrappers/control_signal.hpp"
#include "dls2/util/messaging/dds_reader.hpp"
#include "dls2/math/spline/ramp.hpp"
#include <boost/process.hpp>
#include "dls2/components/app_data.hpp"

#include <memory>

/// A struct representing the control signal that is output by a Controller
namespace dls
{
    class ControllerData : public AppData
    {
    public:
        ControllerData
        (
            const std::shared_ptr<robotlib::RobotBase>&,
            std::shared_ptr<spline::SplineBase<double>> pSpline_in,
            std::shared_ptr<spline::SplineBase<double>> pSpline_out,
            const std::chrono::duration<double> &duration_in,
            const std::chrono::duration<double> &duration_out
        );

        ControlSignal getLastPublishedControlSignal();

        std::atomic<double> premultiplier; ///< Spline value to premutilply the torque signal
        const std::chrono::duration<double> spline_in_duration;
        const std::chrono::duration<double> spline_out_duration;
        const std::shared_ptr<spline::SplineBase<double>> pSpline_in;
        const std::shared_ptr<spline::SplineBase<double>> pSpline_out;

        // BEGIN critical section	
            ControlSignal control_signal;
            std::mutex control_signal_mutex;
        // END critical section
    };
}// end namespace dls

#endif /* end of include guard: CONTROL_SIGNAL_HPP_QCFRROHM */
