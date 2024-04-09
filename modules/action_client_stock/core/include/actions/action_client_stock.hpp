#ifndef COMMON_ACTIONS_CLIENT_HPP
#define COMMON_ACTIONS_CLIENT_HPP

#include "dls2/action/action_client_base.hpp"
# include "robotlib/robot_base.hpp"

namespace dls
{
    class ActionClientStock
    {
        public:
            ActionClientStock(const std::string& robot_name);
            ~ActionClientStock() = default;

        private:
            const std::shared_ptr<robotlib::RobotBase> pRobot;
            const std::shared_ptr<DDSParticipant> dds_participant;
            const std::shared_ptr<CommandManager> command_manager;

            std::shared_ptr<robotlib::RobotBase> loadRobot(const std::string& robot_name);

            //! Stock of action clients
            std::map<std::string, std::shared_ptr<ActionClientBase>> action_clients;
    };
}

#endif