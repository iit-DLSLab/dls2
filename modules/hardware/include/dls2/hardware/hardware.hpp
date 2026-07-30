
#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include "dls2/application/periodic_app.hpp"

#include "dls2/util/messaging/dds_participant.hpp"
#include "robotlib/robot_factory.hpp"

#include "dls_hal/hal.hpp"

namespace dls
{
    class Hardware : public PeriodicApp
    {
    public:
        typedef Hardware *create_t(std::string);
        typedef void destroy_t(Hardware*);

        Hardware
        (
            const std::string& ID,                                       ///< The ID of the controller
            const std::string& robot_name_                               ///< The name of the robot
        );

        virtual ~Hardware() = default;

        void run(const std::chrono::system_clock::time_point&) override;

        bool checkActivation() override;

    protected:
        std::shared_ptr<robotlib::RobotBase> pRobot;

        std::shared_ptr<HalBase> hal;
    };
} // end namespace dls

#endif /* end of include guard: HARDWARE_HPP */