#ifndef CONTROLLER_DATA_HPP
#define CONTROLLER_DATA_HPP

#include "dls2/application/app_data.hpp"
#include "dls_messages/dds/control_signal.hpp"
#include "dls2/signal/reader.hpp"
#include "dls2/math/ramp.hpp"
#include "robotlib/robot_base.hpp"

#include <memory>

namespace dls
{
    class ControllerData : public AppData
    {
    public:
        ControllerData
        (
            const std::string&,
            std::shared_ptr<dls::DDSParticipant> participant,
            const dls::topicType& topic,
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

        Reader<dls2_interface::msg::ControlSignal> reader_control_signal;
    };
}// end namespace dls

#endif /* end of include guard: CONTROLLER_DATA_HPP */
