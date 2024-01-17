#include "actions/goHome/topics.hpp"

// Include the TypeSupport of each message associated to each topic
#include <dls_messages/dds/vector3dPubSubTypes.h> // # off-the-shelf message
#include <dls_messages/dds/boolPubSubTypes.h>     // # off-the-shelf message
#include "dls2/topics/topics.hpp"

namespace dls
{
    namespace topics
    {
        namespace goHome
        {
            dls::topicType goal_topic = dls::topicType("goHome_goal", new Vector3dMsgPubSubType());
            dls::topicType feedback_topic = dls::topics::trajectory_generator;
            dls::topicType result_topic = dls::topicType("goHome_result", new BoolMsgPubSubType());
        }
    }
}
