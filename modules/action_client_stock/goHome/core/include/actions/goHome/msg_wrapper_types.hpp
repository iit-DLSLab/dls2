#ifndef GO_HOME_MSG_WRAPPER_TYPES
#define GO_HOME_MSG_WRAPPER_TYPES

// #include "dls2/signal/joint_state.hpp"
// #include "dls_messages/dds/trajectory_generatorWrapper.hpp"
// #include "dls2/signal/bool.hpp"

#include "dls_messages/dds/boolWrapper.hpp"
#include "dls_messages/dds/joint_statesWrapper.hpp"
#include "dls_messages/dds/trajectory_generatorWrapper.hpp"

namespace dls{
    namespace actions_msgs{
        namespace goHome{
            typedef JointStatesWrapper GOAL_t;
            typedef TrajectoryGeneratorWrapper FEEDBACK_t;
            typedef BoolWrapper RESULT_t;
        }
    }
}

#endif