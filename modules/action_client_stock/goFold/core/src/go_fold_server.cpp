#include "actions/goFold/go_fold_server.hpp"
#include "actions/goFold/topics.hpp"
#include "robotlib/robot_factory.hpp"

namespace dls
{
    GoFoldServer::GoFoldServer(const std::string &ID, const std::shared_ptr<robotlib::RobotBase> pRobot) : 
                                ActionServerBase(
                                ID,
                                topics::goFold::goal_topic,
                                topics::goFold::feedback_topic,
                                topics::goFold::result_topic,
                                actions_msg_wrappers::goFold::GOAL_t(pRobot),
                                actions_msg_wrappers::goFold::FEEDBACK_t(pRobot),
                                actions_msg_wrappers::goFold::RESULT_t()),
                                pRobot(pRobot),
                                blind_state(pRobot)
    {
		this->buildInput<BlindState>(   "blind_state_reader",
                                        dls::topics::low_level_estimation::blind_state,
                                        &this->blind_state, 
                                        [&](){
                                            if(!initialized)
                                            {
                                                read("blind_state_reader");
                                                feedback.desired_joints_position_ = this->blind_state.joints_position_;
                                                initialized = true;
                                            }
                                        });
        this->initialized = false;
    }

    void GoFoldServer::runAction()
    {
        for(auto &leg: feedback.desired_joints_position_)
        {
            for(auto &joint: *leg.data_)
            {
                feedback.desired_joints_position_[joint.key_] = (1 - fCoeff) * feedback.desired_joints_position_[joint.key_] + fCoeff * this->goal.q_[joint.key_];
            }
        }
        feedback.desired_joints_velocity_ = 0.0;

        pRobot->forwardKinematics(feedback.desired_joints_position_, feedback.touch_down_);
        feedback.desired_joints_velocity_.setZero();

        feedback.nominal_touch_down_ = feedback.touch_down_;
        feedback.stance_legs_ = true;

        // Check that the configuration is reached
        double norm=0;
        for(auto &leg: feedback.desired_joints_position_)
        {
            for(auto &joint: *leg.data_)
            {
                norm += pow(feedback.desired_joints_position_[joint.key_]-this->goal.q_[joint.key_],2);
            }
        }
        norm = sqrt(norm);
        if(norm < 0.01) {
            scout_sys<<"Fold configuration reached" <<std::endl;
            this->result.data_ = true;
            this->stopAction();
        } 
    }

    extern "C" PeriodicPluginBase *create(const std::string &ID, const std::string &robot_name)
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

    extern "C" void destroy(PeriodicPluginBase *p)
    {
        delete p;
    }
}