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
, ddslink("FoxServer::monitor", dls::domains::signals)
{
  this->serverThread = new std::thread(&FoxServer::serverFunc, this);
  ddslink.setTopicListener(this);
}

FoxServer::~FoxServer(){}

void FoxServer::serverFunc()
{
  foxserver.run();
  std::cout << "#### Foxglove Server Stopped #####" << std::endl;
} 

void FoxServer::run()
{
  
}

void FoxServer::stop()
{
  // foxserver.removeChannel(1);
  // foxserver.removeChannel(2);
  foxserver.stop();

  if (timer) {
    timer->cancel();
  }

  this->serverThread->join();
  delete this->serverThread;
  this->serverThread = nullptr;
}

void FoxServer::on_topic_discovery(const std::string& topic_name, const std::string& type_name)
{
  std::cout << "Topic discovered: " << topic_name << " [ " << type_name << " ]" << std::endl;

  if(type_name == "DesiredTorquesMsg")
  {
    std::ifstream jsonSchemaFile("/home/dwbertol/dls2_ws/dls2_deploy/dls2/modules/foxglove/json/DesiredTorques.json");
    json jsonSchema = json::parse(jsonSchemaFile);

    auto chanId = this->foxserver.addChannel({
      topic_name,
      "json",
      jsonSchema["title"],
      jsonSchema.dump()
    });

    this->ddslink.addReader("desired_torques_foxglove",
		dls::topics::desired_torques,
		std::function<void(void *)>
		{
			[&, chanId](void *tuple)
			{
				DesiredTorquesMsg msg = *((DesiredTorquesMsg*) tuple);

        json jsonMsg;
        jsonMsg["torques"] = json::array();

        for(long unsigned int i = 0; i < msg.desired_torques().size(); i++)
        {
          jsonMsg["torques"].push_back(msg.desired_torques()[i]);
        }

        foxserver.sendMessage(chanId, nanosecondsSinceEpoch(), jsonMsg.dump());

  		}
		});
  }
  else if(type_name == "BlindStateMsg")
  {
    // Create frames
    std::ifstream jsonFrameSchemaFile("/home/dwbertol/dls2_ws/dls2_deploy/dls2/modules/foxglove/json/FrameTransform.json");
    json jsonFrameSchema = json::parse(jsonFrameSchemaFile);

    const auto chanFrame = this->foxserver.addChannel({
      "frames",
      "json",
      jsonFrameSchema["title"],
      jsonFrameSchema.dump()
    });

    std::ifstream jsonFramesFile("/home/dwbertol/dls2_ws/dls2_deploy/dls2/modules/foxglove/json/frames.json");
    json jsonFramesMsg = json::parse(jsonFramesFile);

    // Update scene
    std::ifstream jsonSceneSchemaFile("/home/dwbertol/dls2_ws/dls2_deploy/dls2/modules/foxglove/json/SceneUpdate.json");
    json jsonSceneSchema = json::parse(jsonSceneSchemaFile);

    auto chanScene = this->foxserver.addChannel({
      "scene",
      "json",
      jsonSceneSchema["title"],
      jsonSceneSchema.dump()
    });

    std::ifstream jsonAliengoFile("/home/dwbertol/dls2_ws/dls2_deploy/dls2/modules/foxglove/json/aliengo.json");
    json jsonAliengoMsg = json::parse(jsonAliengoFile);

    // Handler for connection
    foxserver.setSubscribeHandler([&, jsonFramesMsg, chanFrame, jsonAliengoMsg, chanScene](ChannelId chanId) {
      if(chanId == chanFrame)
      {
        for(auto frame : jsonFramesMsg["transforms"])
        {
          foxserver.sendMessage(chanId, nanosecondsSinceEpoch(), frame.dump());
        }
      }
      else if(chanId == chanScene)
      {
        foxserver.sendMessage(chanId, nanosecondsSinceEpoch(), jsonAliengoMsg.dump());
      }
    });

    // Frames update
    std::ifstream jsonBlindSchemaFile("/home/dwbertol/dls2_ws/dls2_deploy/dls2/modules/foxglove/json/BlindState.json");
    json jsonBlindSchema = json::parse(jsonBlindSchemaFile);

    auto chanBlind = this->foxserver.addChannel({
      topic_name,
      "json",
      jsonBlindSchema["title"],
      jsonBlindSchema.dump()
    });

    this->ddslink.addReader("blind_state_foxglove",
		dls::topics::low_level_estimation::blind_state,
		std::function<void(void *)>
		{
			[&, chanBlind, chanFrame, jsonFramesMsg](void *tuple)
			{
				BlindStateMsg msg = *((BlindStateMsg*) tuple);

        json jsonBlindMsg;
        jsonBlindMsg["joint_pos"] = json::array();
        jsonBlindMsg["joint_vel"] = json::array();
        jsonBlindMsg["joint_acc"] = json::array();
        jsonBlindMsg["joint_eff"] = json::array();

        for(long unsigned int i = 0; i < msg.joint_pos().size(); i++)
        {
          jsonBlindMsg["joint_pos"].push_back(msg.joint_pos()[i]);
          jsonBlindMsg["joint_vel"].push_back(msg.joint_vel()[i]);
          jsonBlindMsg["joint_acc"].push_back(msg.joint_acc()[i]);
          jsonBlindMsg["joint_eff"].push_back(msg.joint_eff()[i]);
        }

        jsonBlindMsg["base_pos"]["x"] = msg.base_pos_world()[0];
        jsonBlindMsg["base_pos"]["y"] = msg.base_pos_world()[1];
        jsonBlindMsg["base_pos"]["z"] = msg.base_pos_world()[2];

        jsonBlindMsg["base_ori"]["x"] = msg.base_ori_world()[0];
        jsonBlindMsg["base_ori"]["y"] = msg.base_ori_world()[1];
        jsonBlindMsg["base_ori"]["z"] = msg.base_ori_world()[2];
        jsonBlindMsg["base_ori"]["w"] = msg.base_ori_world()[3];

        jsonBlindMsg["base_lin_vel"]["x"] = msg.base_lin_vel_world()[0];
        jsonBlindMsg["base_lin_vel"]["y"] = msg.base_lin_vel_world()[1];
        jsonBlindMsg["base_lin_vel"]["z"] = msg.base_lin_vel_world()[2];

        jsonBlindMsg["base_ang_vel"]["x"] = msg.base_ang_vel_world()[0];
        jsonBlindMsg["base_ang_vel"]["y"] = msg.base_ang_vel_world()[1];
        jsonBlindMsg["base_ang_vel"]["z"] = msg.base_ang_vel_world()[2];

        jsonBlindMsg["base_lin_acc"]["x"] = msg.base_lin_acc_world()[0];
        jsonBlindMsg["base_lin_acc"]["y"] = msg.base_lin_acc_world()[1];
        jsonBlindMsg["base_lin_acc"]["z"] = msg.base_lin_acc_world()[2];

        jsonBlindMsg["base_ang_acc"]["x"] = msg.base_ang_acc_world()[0];
        jsonBlindMsg["base_ang_acc"]["y"] = msg.base_ang_acc_world()[1];
        jsonBlindMsg["base_ang_acc"]["z"] = msg.base_ang_acc_world()[2];

        foxserver.sendMessage(chanBlind, nanosecondsSinceEpoch(), jsonBlindMsg.dump());

        
        for(auto frame : jsonFramesMsg["transforms"])
        {
          if(frame["child_frame_id"] == "trunk")
          {
            frame["translation"]["x"] = msg.base_pos_world()[0];
            frame["translation"]["y"] = msg.base_pos_world()[1];
            frame["translation"]["z"] = msg.base_pos_world()[2];

            frame["rotation"]["x"] = msg.base_ori_world()[0];
            frame["rotation"]["y"] = msg.base_ori_world()[1];
            frame["rotation"]["z"] = msg.base_ori_world()[2];
            frame["rotation"]["w"] = msg.base_ori_world()[3];
          }
          else
          {
            int i = 0;
            for(auto joint_name : msg.joint_name())
            {
              if(joint_name == frame["child_frame_id"])
              {
                double a = frame["rotation"]["w"].get<double>();
                double b = frame["rotation"]["x"].get<double>();
                double c = frame["rotation"]["y"].get<double>();
                double d = frame["rotation"]["z"].get<double>();

                double e = cos(msg.joint_pos()[i]/2);
                double f = 0;
                double g = 0;
                double h = sin(msg.joint_pos()[i]/2);

                frame["rotation"]["x"] = b*e + a*f + c*h - d*g;
                frame["rotation"]["y"] = a*g - b*h + c*e + d*f;
                frame["rotation"]["z"] = a*h + b*g - c*f + d*e;
                frame["rotation"]["w"] = a*e - b*f - c*g - d*h;
                break;
              }
              i++;
            }
          }
          foxserver.sendMessage(chanFrame, nanosecondsSinceEpoch(), frame.dump());
        }
			}
		});
  }
}