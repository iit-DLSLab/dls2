#ifndef GO_HOME_MSG_WRAPPER_TYPES
#define GO_HOME_MSG_WRAPPER_TYPES

#include "dls2/msg_wrappers/joint_state.hpp"
#include "dls2/msg_wrappers/trajectory_generator.hpp"
#include "dls2/msg_wrappers/bool.hpp"

namespace dls{
    namespace actions_msgs{
        namespace goHome{
            typedef JointState GOAL_t;
            typedef TrajectoryGenerator FEEDBACK_t;
            typedef Bool RESULT_t;
        }
    }
}

#endif