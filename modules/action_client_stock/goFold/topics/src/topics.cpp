#include "actions/goFold/topics.hpp"

// Include the TypeSupport of each message associated to each topic
#include <dls_messages/dds/joint_statePubSubTypes.h> // # off-the-shelf message
#include <dls_messages/dds/boolPubSubTypes.h>     // # off-the-shelf message
#include "dls2/topics/topics.hpp"

namespace dls
{
    namespace topics
    {
        namespace goFold
        {
            dls::topicType goal_topic = dls::topicType("goFold_goal", new JointStateMsgPubSubType());
            dls::topicType feedback_topic = dls::topics::trajectory_generator;
            dls::topicType result_topic = dls::topicType("goFold_result", new BoolMsgPubSubType());
        }
    }
}
