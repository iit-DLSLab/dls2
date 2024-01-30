#include "actions/action_client_stock.hpp"
#include "robotlib/robot_factory.hpp"

#include "actions/goHome/go_home_client.hpp"
#include "actions/goFold/go_fold_client.hpp"

namespace dls
{
    ActionClientStock::ActionClientStock(const std::string& robot_name)
                        :   pRobot(loadRobot(robot_name)),
                            dds_participant(std::make_shared<DDSParticipant>("ActionClientStock", dls::domains::signals)),
                            command_manager(std::make_shared<CommandManager>("ActionClientStock"))
    {
        action_clients["goHome"] = std::make_shared<GoHomeClient>("goHome", dds_participant, command_manager, pRobot);
        action_clients["goFold"] = std::make_shared<GoFoldClient>("goFold", dds_participant, command_manager, pRobot);

    }

    void ActionClientStock::checkAndUnload()
    {
        for(auto it = action_clients.begin(); it != action_clients.end(); it++)
        {
            if(it->second->isActionComplete())
            {
                it->second->unloadAction();
            }
        }
    }

    std::shared_ptr<robotlib::RobotBase> ActionClientStock::loadRobot(const std::string& robot_name)
    {
        std::shared_ptr<robotlib::RobotBase> pRobot;

        try
        {
            pRobot = robotlib::RobotFactory::openRobot(robot_name);
        }
        catch (const std::exception &e)
        {
            std::cerr << "child_process: Could not open the robot " << robot_name << std::endl;
            std::cerr << e.what() << std::endl;
        }
        return pRobot;
    }
}