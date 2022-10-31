#ifndef FOX_SERVER_HPP
#define FOX_SERVER_HPP

#include "foxglove/webserver.hpp"

namespace dls {

  using json = nlohmann::json;

  class FoxServer : public dls::DDSPartListener
  {
    public:
      FoxServer();
      ~FoxServer();

      void run();
      void stop();

    private:

      void serverFunc();    
      void ddsBridgeFunc();
      std::thread *serverThread;
      std::shared_ptr<boost::asio::steady_timer> timer;
      Server foxserver;

      dls::DDSParticipant ddslink;

      void on_topic_discovery(const std::string& topic_name, const std::string& type_name) override;
  };
}

#endif