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

#include "dls2/msg_wrappers/blind_state.hpp"
#include "dls2/msg_wrappers/control_signal.hpp"

namespace dls
{
    class Hardware : public PeriodicAppLayerComponent
    {
        friend class HardwareLayer;
        
    public:

        using ID_t = std::string;

        typedef Hardware *create_t(std::shared_ptr<robotlib::RobotBase>);
        typedef void destroy_t(Hardware*);

        Hardware
        (
            const std::string&,                                        		 						///< The ID of the controller
            const std::shared_ptr<robotlib::RobotBase>&,                   		 					///< A pointer to the robot model
            const period_t&,                                     		 							///< The period of the controller
            const dls::topicType& controlSignalTopic_ = dls::topics::control_signal,    			///< Topic where control signal should be published
            const dls::topicType& rawSignalTopic_ = dls::topics::low_level_estimation::blind_state	///< Topic where raw signal is being published
        );

        virtual ~Hardware() = default;

        virtual void run(const std::chrono::system_clock::time_point&) = 0;

    private:

        std::atomic_bool should_run;

        dls::topicType control_signal_topic;
        dls::topicType blind_state_topic;        

        void executeCommand(std::string cmd);

    protected:
        
        /// Sends the raw info from the robot to the rest of the architecture
        ///
        void publishSignal();

        std::shared_ptr<robotlib::RobotBase> pRobot;

        dls::DDSParticipant ddslink;
        dls::DDSReader  	ddsMonitor;

        BlindStateMsg blind_state;
        DesiredTorquesMsg control_signal;
    };
} // end namespace dls

#endif /* end of include guard: HARDWARE_HPP */