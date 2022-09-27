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
    class Hardware : public PeriodicAppLayerComponent
    {
        friend class HardwareLayer;
        
    public:

        using ID_t = std::string;

        typedef Hardware *create_t(std::shared_ptr<robotlib::RobotBase>);
        typedef void destroy_t(Hardware*);

        Hardware
        (
<<<<<<< Updated upstream
            const std::string&,                                        		 						///< The ID of the controller
            const std::shared_ptr<robotlib::RobotBase>&,                   		 					///< A pointer to the robot model
            const period_t&,                                     		 							///< The period of the controller
=======
            const ID_t&,
            const std::shared_ptr<robotlib::RobotBase>&, 
            const period_t&,
>>>>>>> Stashed changes
            const dls::topicType& controlSignalTopic_ = dls::topics::control_signal,    			///< Topic where control signal should be published
            const dls::topicType& rawSignalTopic_ = dls::topics::low_level_estimation::aliengo_raw	///< Topic where raw signal is being published
        );

        virtual ~Hardware() = default;

    protected:
        
    //     /// Sends the raw info from the robot to the rest of the architecture
    //     ///
    //     void publishSignal();

<<<<<<< Updated upstream
        const std::shared_ptr<const robotlib::RobotBase> pRobot;

    private:
        std::atomic_bool should_run;

        dls::topicType control_signal_topic;
        dls::topicType raw_signal_topic;
=======
        std::shared_ptr<robotlib::RobotBase> pRobot;

        dls::topicType control_signal_topic;
        dls::topicType raw_signal_topic;
        
    //     dls::DDSParticipant ddslink;
    //     dls::DDSReader  	ddsMonitor;

        BlindStateMsg blind_state;
        DesiredTorquesMsg desired_torques;

    private:

    //     std::atomic_bool should_run;
>>>>>>> Stashed changes

        dls::DDSParticipant ddslink;
        dls::DDSReader  	ddsMonitor;

        void executeCommand(std::string cmd);

    //     pthread_t hardwareThread;

    //     static void *runHAL(void *data);

    //     void startRunning();
    };
} // end namespace dls

#endif /* end of include guard: HARDWARE_HPP */