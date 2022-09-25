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
#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include "dls2/components/periodic_app_layer_component.hpp"
#include "dls2/util/messaging/dds_participant.hpp"
#include "robotlib/robot_factory.hpp"

namespace dls
{
    template<class DataIn = DesiredTorquesMsg, class DataOut = BlindStateMsg>
    class Hardware : public PeriodicAppLayerComponent
    {
        friend class HardwareLayer;

    public:

        typedef Hardware *create_t(std::shared_ptr<robotlib::RobotBase>);
        typedef void destroy_t(Hardware*);

        Hardware
        (
            const std::string&,
            const std::string&,
            const period_t&,
            const dls::topicType& controlSignalTopic_ = dls::topics::control_signal,    			///< Topic where control signal should be published
            const dls::topicType& rawSignalTopic_ = dls::topics::low_level_estimation::aliengo_raw	///< Topic where raw signal is being published
        );

        virtual ~Hardware() = default;

        virtual void run(const std::chrono::system_clock::time_point&) = 0;

    protected:
        
        /// Sends the raw info from the robot to the rest of the architecture
        ///
        void publishSignal();

        std::shared_ptr<robotlib::RobotBase> pRobot;
        
        dls::DDSParticipant ddslink;
        dls::DDSReader  	ddsMonitor;

        DataOut blind_state;
        DataIn desired_torques;

    private:

        std::atomic_bool should_run;

        dls::topicType control_signal_topic;
        dls::topicType raw_signal_topic;

        void executeCommand(std::string cmd);
};
} // end namespace dls

#endif /* end of include guard: HARDWARE_HPP */