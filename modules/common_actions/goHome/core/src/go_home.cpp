#include "actions/goHome/go_home.hpp"
#include "actions/goHome/topics.hpp"

namespace dls
{
    GoHome::GoHome(const std::shared_ptr<robotlib::RobotBase> pRobot) : ActionServerBase(
                                                                            "go_home",
                                                                            topics::goHome::goal_topic,
                                                                            topics::goHome::feedback_topic,
                                                                            topics::goHome::result_topic,
                                                                            Vector3d(),
                                                                            TrajectoryGenerator(pRobot),
                                                                            Bool())
    {
    }

    void GoHome::runAction()
    {
        std::cout << "goal: " << this->goal.data_.transpose() << std::endl;
        this->feedback.stance_legs_["LF"] = true;
        this->feedback.stance_legs_["RF"] = true;
        count += 1;

        if (count >= 100)
        {
            this->result.data_ = true;
            this->stop();
        }
    }
}