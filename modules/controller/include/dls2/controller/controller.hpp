#ifndef CONTROLLER_HPP_RSFU8GQS
#define CONTROLLER_HPP_RSFU8GQS

// =============================================================================
// Includes
// =============================================================================
#include "dls2/application/periodic_app.hpp"
#include "dls2/controller/controller_data.hpp"
#include "dls2/controller/control_modes.hpp"

#include "dls2/util/messaging/dds_participant.hpp"

#include "robotlib/robot_factory.hpp"

namespace dls
{
    class Controller : public PeriodicApp
    {
    public:
        // Plugin typedefs
        typedef Controller* create_t(std::string);
        typedef void destroy_t(Controller*);

        Controller
        (
            const std::string&,                                 ///< The ID of the controller
            const std::shared_ptr<robotlib::RobotBase>);

        virtual ~Controller();

        std::shared_ptr<dls::DDSParticipant> getParticipant();
		std::shared_ptr<robotlib::RobotBase> getRobot();

    protected:
        /// Function gets called each epoch.
        ///
        /// @param time The time when this function is called,
        virtual void run(const std::chrono::system_clock::time_point &time) = 0;

        /// Pointer to the robot model
        const std::shared_ptr<robotlib::RobotBase> pRobot;
    private:

        std::shared_ptr<dls::DDSParticipant> ddsLink;
};
} // end namespace dls

#endif /* end of include guard: CONTROLLER_HPP_RSFU8GQS */