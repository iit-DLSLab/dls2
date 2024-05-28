#include "actions/goHome/topics.hpp"

// Include the TypeSupport of each message associated to each topic
#include <dls_messages/dds/joint_statePubSubTypes.h> // # off-the-shelf message
#include <dls_messages/dds/boolPubSubTypes.h>     // # off-the-shelf message
#include "dls2/topics/topics.hpp"

namespace dls
{
    namespace topics
    {
        namespace goHome
        {
            dls::topicType goal = dls::topicType("goHome_goal", new JointStateMsgPubSubType());
            dls::topicType feedback = dls::topics::trajectory_generator;
            dls::topicType result = dls::topicType("goHome_result", new BoolMsgPubSubType());
        }
    }
}
