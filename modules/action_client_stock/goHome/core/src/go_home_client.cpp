#include "actions/goHome/go_home_client.hpp"
#include "actions/goHome/topics.hpp"
#include "robotlib/robot_factory.hpp"

namespace dls
{
    GoHomeClient::GoHomeClient( const std::string &action_name,
                                const std::shared_ptr<DDSParticipant> dds_participant,
                                const std::shared_ptr<CommandManager>  command_manager,
						        const std::shared_ptr<robotlib::RobotBase> pRobot) 
                                : ActionClient(
                                    action_name,
                                    dds_participant,
                                    command_manager,
                                    topics::goHome::goal_topic,
                                    topics::goHome::feedback_topic,
                                    topics::goHome::result_topic,
                                    actions_msg_wrappers::goHome::GOAL_t(pRobot),
                                    actions_msg_wrappers::goHome::FEEDBACK_t(pRobot),
                                    actions_msg_wrappers::goHome::RESULT_t()),
                                    home_configuration(pRobot->makeJointState(0.0)),
                                    goal(pRobot)
    {
        for(auto &leg_pair : this->home_configuration)
        {
            for(auto &joint_pair : *leg_pair.data_)
            {
                this->home_configuration[joint_pair.key_] = config[pRobot->getName()][joint_pair.key_->getName()].as<double>();
            }
        }
    }

    bool GoHomeClient::consoleCommand()
    {
        // Call the action
        this-> goal.q_ = this->home_configuration; 
        this->callAction(goal);
        return true;
    }
}