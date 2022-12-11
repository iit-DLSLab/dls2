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
#ifndef CONTROLLER_HPP_RSFU8GQS
#define CONTROLLER_HPP_RSFU8GQS

// =============================================================================
// Includes
// =============================================================================
#include "dls2/components/periodic_app_layer_component.hpp"
#include "dls2/controller/controller_data.hpp"

#include "dls2/util/messaging/dds_participant.hpp"

#include "robotlib/robot_factory.hpp"

// =============================================================================
// Class Interface
// =============================================================================
// TODO a lot of functions have been removed from Controller. Many of them need to be put into Dog
namespace dls
{
    class Controller : public PeriodicAppLayerComponent
    {
        friend class ControlLayer;

    public:

        // Plugin typedefs
        typedef Controller* create_t(std::string);
        typedef void destroy_t(Controller*);

        Controller
        (
            const std::string&,                                 ///< The ID of the controller
            const std::shared_ptr<robotlib::RobotBase>&,        ///< A pointer to the robot model
            const period_t&,                                    ///< The period of the controller
            const ControlSignal::SignalReconstructionMethod&    ///< Signal reconstruction used by this controller
        );

        virtual ~Controller() = default;

        dls::DDSParticipant* getParticipant();
		const robotlib::RobotBase* getRobot();

    protected:
        /// Function gets called each epoch.
        ///
        /// @param time The time when this function is called,
        virtual void run(const std::chrono::system_clock::time_point &time) = 0;

        const ControlSignal::SignalReconstructionMethod signal_reconstruction_method;

    private:
        /// Pointer to the robot model
        const std::shared_ptr<const robotlib::RobotBase> pRobot;

        dls::DDSParticipant ddsLink;
};
} // end namespace dls

#endif /* end of include guard: CONTROLLER_HPP_RSFU8GQS */