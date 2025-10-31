#ifndef GO_FOLD_MSG_WRAPPER_TYPES
#define GO_FOLD_MSG_WRAPPER_TYPES

#include "dls_messages/dds/bool.hpp"
#include "dls_messages/dds/joint_states.hpp"
#include "dls_messages/dds/trajectory_generator.hpp"

namespace dls{
    namespace actions_msgs{
        namespace goFold{
            typedef JointStates GOAL_t;
            typedef TrajectoryGenerator FEEDBACK_t;
            typedef Bool RESULT_t;
        }
    }
}

#endif