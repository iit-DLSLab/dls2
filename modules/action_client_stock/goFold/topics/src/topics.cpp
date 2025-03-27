#include "actions/goFold/topics.hpp"

// Include the TypeSupport of each message associated to each topic
#include <dls_messages/dds/joint_statesPubSubTypes.hpp> // # off-the-shelf message
#include <dls_messages/dds/boolPubSubTypes.hpp>     // # off-the-shelf message
#include "dls2/topics/topics.hpp"

namespace dls
{
    namespace topics
    {
        namespace goFold
        {
            dls::topicType goal = dls::topicType("goFold_goal", new JointStatesPubSubType());
            dls::topicType feedback = dls::topics::trajectory_generator;
            dls::topicType result = dls::topicType("goFold_result", new BoolPubSubType());
        }
    }
}
