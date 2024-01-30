#ifndef GO_FOLD_MSG_WRAPPER_TYPES
#define GO_FOLD_MSG_WRAPPER_TYPES

#include "dls2/msg_wrappers/joint_state.hpp"
#include "dls2/msg_wrappers/trajectory_generator.hpp"
#include "dls2/msg_wrappers/bool.hpp"

namespace dls{
    namespace actions_msg_wrappers{
        namespace goFold{
            typedef JointState GOAL_t;
            typedef TrajectoryGenerator FEEDBACK_t;
            typedef Bool RESULT_t;
        }
    }
}

#endif