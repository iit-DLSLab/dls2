#include <foxglove/foxserver.hpp>

#include <fstream>
#include <Eigen/Dense>

using json = nlohmann::json;
using namespace boost;
using namespace dls;

static uint64_t nanosecondsSinceEpoch() {
    return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count());
}

FoxServer::FoxServer()
    : serverThread(nullptr)
    , foxserver(8765, "example server")
    , ddslink("FoxServer::monitor", dls::domains::signals, false)
{
    this->serverThread = new std::thread(&FoxServer::serverFunc, this);
    ddslink.setTopicListener(this);

    this->setTimer = [&] {
        this->timer = this->foxserver.getEndpoint().set_timer(20, [&](std::error_code const& ec) {
            if (ec) 
            {
                std::cerr << "timer error: " << ec.message() << std::endl;
                return;
            }
            for (auto const &chan: this->timer_flags) 
            {
                this->send_flags.erase(chan);
            }

            this->setTimer();
        });
    };

    this->setTimer();
}

FoxServer::~FoxServer()
{ }

void FoxServer::serverFunc()
{
    foxserver.run();
    std::cout << "#### Foxglove Server Stopped #####" << std::endl;
} 

void FoxServer::on_topic_discovery(const std::string& topic_name, const std::string& type_name)
{
    std::cout << "Topic discovered: " << topic_name << " [ " << type_name << " ]" << std::endl;

    if(topic_name == "blind_state")
    {
        // Create frames
        std::ifstream jsonFrameSchemaFile("/usr/lib/dls2/dls_foxglove/FrameTransform.json");
        json jsonFrameSchema = json::parse(jsonFrameSchemaFile);

        const auto chanFrame = this->foxserver.addChannel({
            "frames",
            "json",
            jsonFrameSchema["title"],
            jsonFrameSchema.dump()
        });

        this->timer_flags.insert(chanFrame);

        // Update scene
        std::ifstream jsonSceneSchemaFile("/usr/lib/dls2/dls_foxglove/SceneUpdate.json");
        json jsonSceneSchema = json::parse(jsonSceneSchemaFile);

        auto chanScene = this->foxserver.addChannel({
            "scene",
            "json",
            jsonSceneSchema["title"],
            jsonSceneSchema.dump()
        });

        // Handler for connection
        foxserver.setSubscribeHandler([&, this](ChannelId chanId) {
            this->send_flags.erase(chanId);
        });

        this->ddslink.addReader("blind_state_foxglove",
		dls::topics::low_level_estimation::blind_state,
		std::function<void(void *)>
		{
            [&, chanFrame, chanScene](void *tuple)
			{
                auto epoch = nanosecondsSinceEpoch();
                BlindStateMsg msg = *((BlindStateMsg*) tuple);

                // if(this->send_flags.find(chanScene) == this->send_flags.end())
                // {
                //     std::ifstream jsonRobotFile("/usr/include/" + msg.robot_name() + "_description/foxglove/" + msg.robot_name() + ".json");
                //     // std::ifstream jsonRobotFile("/home/dwbertol/dls2_ws/dls2_deploy/dls2/modules/foxglove/json/" + msg.robot_name() + ".json");

                //     foxserver.sendMessage(chanScene, nanosecondsSinceEpoch(), json::parse(jsonRobotFile).dump());
                //     this->send_flags.insert(chanScene);
                // }

        //         if(this->send_flags.find(chanFrame) != this->send_flags.end()) 
        //             return;
          
        //         std::ifstream jsonFramesFile("/usr/include/" + msg.robot_name() + "_description/foxglove/" + msg.robot_name() + "_frames.json");
        //         // std::ifstream jsonFramesFile("/home/dwbertol/dls2_ws/dls2_deploy/dls2/modules/foxglove/json/" + msg.robot_name() + "_frames.json");
        //         json jsonFramesMsg = json::parse(jsonFramesFile);
                
        //         for(auto frame : jsonFramesMsg["transforms"])
        //         {
        //             if(frame["child_frame_id"] == "trunk")
        //             {
        //                 frame["translation"]["x"] = msg.base_pos_world()[0];
        //                 frame["translation"]["y"] = msg.base_pos_world()[1];
        //                 frame["translation"]["z"] = msg.base_pos_world()[2];

        //                 frame["rotation"]["x"] = msg.base_ori_world()[0];
        //                 frame["rotation"]["y"] = msg.base_ori_world()[1];
        //                 frame["rotation"]["z"] = msg.base_ori_world()[2];
        //                 frame["rotation"]["w"] = msg.base_ori_world()[3];
        //             }
        //             else
        //             {
        //                 int i = 0;
        //                 for(auto joint_name : msg.joint_name())
        //                 {
        //                     if(joint_name == frame["child_frame_id"])
        //                     {
        //                         double a = frame["rotation"]["w"].get<double>();
        //                         double b = frame["rotation"]["x"].get<double>();
        //                         double c = frame["rotation"]["y"].get<double>();
        //                         double d = frame["rotation"]["z"].get<double>();

        //                         double e = cos(msg.joint_pos()[i]/2);
        //                         double f = 0;
        //                         double g = 0;
        //                         double h = sin(msg.joint_pos()[i]/2);

        //                         frame["rotation"]["x"] = b*e + a*f + c*h - d*g;
        //                         frame["rotation"]["y"] = a*g - b*h + c*e + d*f;
        //                         frame["rotation"]["z"] = a*h + b*g - c*f + d*e;
        //                         frame["rotation"]["w"] = a*e - b*f - c*g - d*h;
        //                         break;
        //                     }
        //                     i++;
        //                 }
        //             }
        //             foxserver.sendMessage(chanFrame, epoch, frame.dump());
        //         }
        //         this->send_flags.insert(chanFrame);
			}
		});
    }

    eprosima::fastrtps::types::DynamicType_ptr type_ = dds::get_type_registered_(type_name);

    auto jsonPair = dds::createJsonSchema(topic_name, type_);

    auto channel = this->foxserver.addChannel({
        topic_name,
        "json",
        jsonPair.first["title"],
        jsonPair.first.dump()
    });

    this->timer_flags.insert(channel);

    this->ddslink.addReader(topic_name,
        dls::topicType({
            topic_name, 
            eprosima::fastdds::dds::TypeSupport(new eprosima::fastrtps::types::DynamicPubSubType(type_))}),
            std::function<void(void *)>{[&, type_, jsonPair, channel](void *tuple)
            {
                if(this->send_flags.find(channel) != this->send_flags.end())
                    return;

                nlohmann::json jsonVar = jsonPair.second;

                dds::getDataToJson(topic_name, type_, (eprosima::fastrtps::types::DynamicData*) tuple, jsonVar);

                foxserver.sendMessage(channel, nanosecondsSinceEpoch(), jsonVar.dump());
                this->send_flags.insert(channel);
            }}
    );
}