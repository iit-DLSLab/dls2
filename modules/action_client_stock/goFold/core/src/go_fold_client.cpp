#include "actions/goFold/go_fold_client.hpp"
#include "actions/goFold/topics.hpp"
#include "robotlib/robot_factory.hpp"

namespace dls
{
    GoFoldClient::GoFoldClient( const std::string &action_name,
                                const std::shared_ptr<DDSParticipant> dds_participant,
                                const std::shared_ptr<CommandManager>  command_manager,
						        const std::shared_ptr<robotlib::RobotBase> pRobot) 
                                : ActionClient(
                                    action_name,
                                    dds_participant,
                                    command_manager,
                                    topics::goFold::goal_topic,
                                    topics::goFold::feedback_topic,
                                    topics::goFold::result_topic,
                                    actions_msg_wrappers::goFold::GOAL_t(pRobot),
                                    actions_msg_wrappers::goFold::FEEDBACK_t(pRobot),
                                    actions_msg_wrappers::goFold::RESULT_t()),
                                    fold_configuration(pRobot->makeJointState(0.0)),
                                    goal(pRobot)
    {
        for(auto &leg_pair : this->fold_configuration)
        {
            for(auto &joint_pair : *leg_pair.data_)
            {
                this->fold_configuration[joint_pair.key_] = config[pRobot->getName()][joint_pair.key_->getName()].as<double>();
            }
        }
    }

    bool GoFoldClient::consoleCommand()
    {
        // Call the action
        this-> goal.q_ = this->fold_configuration; 
        this->callAction(goal);
        return true;
    }
}