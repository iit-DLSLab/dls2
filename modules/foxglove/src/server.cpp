#include <foxglove/websocket/server.hpp>

#include <nlohmann/json.hpp>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include <fstream>
#include <boost/asio.hpp>

using json = nlohmann::json;
using namespace boost;

static uint64_t nanosecondsSinceEpoch() {
  return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count());
}

int main() {
  foxglove::websocket::Server server{8765, "example server"};

  std::ifstream trns("FrameTransform.json");
  json jsonFrameTransform = json::parse(trns);

  const auto chanId = server.addChannel({
    "transforms",
    "json",
    "foxglove.FrameTransform",
    jsonFrameTransform.dump(),
  });

  std::ifstream scene("SceneUpdate.json");
  json jsonSceneUpdate = json::parse(scene);

  const auto chanId2 = server.addChannel({
    "urdf_msg",
    "json",
    "foxglove.SceneUpdate",
    jsonSceneUpdate.dump()
  });

  server.setSubscribeHandler([&](foxglove::websocket::ChannelId chanId) {
    std::cout << "first client subscribed to " << chanId << std::endl;
  });
  server.setUnsubscribeHandler([&](foxglove::websocket::ChannelId chanId) {
    std::cout << "last client unsubscribed from " << chanId << std::endl;
  });

  std::shared_ptr<asio::steady_timer> timer;
  std::function<void()> setTimer = [&] {
    timer = server.getEndpoint().set_timer(1000, [&](std::error_code const& ec) {
      if (ec) {
        std::cerr << "timer error: " << ec.message() << std::endl;
        return;
      }

      std::ifstream frame("frames.json");
      json jsonFramesMsg = json::parse(frame);

      for( auto elem : jsonFramesMsg["transforms"])
      {
        server.sendMessage(chanId, nanosecondsSinceEpoch(),
                        elem.dump());
      }

      std::ifstream scn("aliengo.json");
      json jsonMsg = json::parse(scn);

      server.sendMessage(chanId2, nanosecondsSinceEpoch(),
                         jsonMsg.dump());
      setTimer();
    });
  };

  setTimer();

  asio::signal_set signals(server.getEndpoint().get_io_service(), SIGINT);

  signals.async_wait([&](std::error_code const& ec, int sig) {
    if (ec) {
      std::cerr << "signal error: " << ec.message() << std::endl;
      return;
    }
    std::cerr << "received signal " << sig << ", shutting down" << std::endl;
    server.removeChannel(chanId);
    server.stop();
    if (timer) {
      timer->cancel();
    }
  });

  server.run();

  return 0;
}