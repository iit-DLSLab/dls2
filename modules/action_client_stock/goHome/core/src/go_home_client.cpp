#include "actions/goHome/go_home_client.hpp"
#include "actions/goHome/topics.hpp"
#include "robotlib/robot_factory.hpp"

namespace dls
{
    GoHomeClient::GoHomeClient( const std::string &action_name,
                                const std::shared_ptr<CommandManager>  command_manager,
						        const std::shared_ptr<robotlib::RobotBase> pRobot) 
                                : ActionClient(
                                    action_name,
                                    command_manager,
                                    topics::goHome::goal,
                                    topics::goHome::feedback,
                                    topics::goHome::result,
                                    actions_msgs::goHome::GOAL_t(pRobot),
                                    actions_msgs::goHome::FEEDBACK_t(pRobot),
                                    actions_msgs::goHome::RESULT_t()),
                                    home_configuration(pRobot->makeJointState(0.0)),
                                    goal(pRobot)
    {
        for(auto joint : this->home_configuration.getJoints()){
            this->home_configuration[joint] = config[pRobot->getName()][joint->getName()].as<double>();
        }
    }

    bool GoHomeClient::consoleCommand()
    {
        // Call the action
        this-> goal.q = this->home_configuration; 
        this->callAction(goal);
        return true;
    }
}