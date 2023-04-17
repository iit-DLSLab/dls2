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
    public:
        typedef Hardware *create_t(std::string);
        typedef void destroy_t(Hardware*);

        Hardware
        (
            const std::string& ID,                                       ///< The ID of the controller
            const std::shared_ptr<robotlib::RobotBase>& robot           ///< A pointer to the robot model
        );

        virtual ~Hardware() = default;

        virtual void run(const std::chrono::system_clock::time_point&) = 0;

        std::shared_ptr<dls::DDSParticipant> getParticipant();

    protected:
        
        std::shared_ptr<robotlib::RobotBase> pRobot;

        std::shared_ptr<dls::DDSParticipant> signalLink;
    };
} // end namespace dls

#endif /* end of include guard: HARDWARE_HPP */