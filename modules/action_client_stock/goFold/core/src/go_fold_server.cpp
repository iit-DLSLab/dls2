#include "actions/goFold/go_fold_server.hpp"
#include "actions/goFold/topics.hpp"
#include "robotlib/robot_factory.hpp"

namespace dls
{
    GoFoldServer::GoFoldServer(const std::string &ID, const std::shared_ptr<robotlib::RobotBase> pRobot) : 
                                ActionServerBase(
                                ID,
                                topics::goFold::goal,
                                topics::goFold::feedback,
                                topics::goFold::result,
                                actions_msgs::goFold::GOAL_t(pRobot),
                                actions_msgs::goFold::FEEDBACK_t(pRobot),
                                actions_msgs::goFold::RESULT_t()),
                                pRobot(pRobot),
                                blind_state(pRobot)
    {
		this->buildInput<BlindStateWrapper>(dls::topics::low_level_estimation::blind_state, &this->blind_state);
    }

    void GoFoldServer::runAction()
    {
        for(auto joint : this->feedback.joints_position.getJoints()){
            feedback.joints_position[joint] = (1 - fCoeff) * feedback.joints_position[joint] + fCoeff * this->goal.q[joint];
        }
        feedback.joints_velocity = 0.0;

        pRobot->forwardKinematics(feedback.joints_position, feedback.touch_down);
        feedback.joints_velocity.setZero();

        feedback.nominal_touch_down = feedback.touch_down;
        feedback.stance_legs = true;

        // Check that the configuration is reached
        double norm=0;

        for(auto joint : this->feedback.joints_position.getJoints()){
                norm += pow(feedback.joints_position[joint]-this->goal.q[joint],2);
            }

        norm = sqrt(norm);
        if(norm < 0.01) {
            scout_sys<<"Fold configuration reached" <<std::endl;
            this->result.data = true;
            this->writeResult();
            this->stopAction();
        } 
    }

    bool GoFoldServer::checkActivation()
	{          
        if(areInputsReceivingData() && areOutputsUnique())
        {
            read();
            feedback.joints_position = this->blind_state.joints_position;
            return true;
        }
		return false;
    }

    extern "C" PeriodicAppPlugin *create(const std::string &ID, const std::string &robot_name)
    {
        if (robot_name == "")
        {
            std::string e = "Parameter robot_name is not defined, verify if the parameter server is running";
            throw std::runtime_error(e);
        }

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
        return new GoFoldServer(ID, pRobot);
    }

    extern "C" void destroy(PeriodicAppPlugin *p)
    {
        delete p;
    }
}