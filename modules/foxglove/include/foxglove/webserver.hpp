#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include <nlohmann/json.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/asio.hpp>

#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/util/messaging/dds_listeners.hpp"

namespace dls {

  using json = nlohmann::json;
  using namespace std::placeholders;

  using AsioServer = websocketpp::server<websocketpp::config::asio>;
  using ConnHandle = websocketpp::connection_hdl;
  using MessagePtr = AsioServer::message_ptr;
  using OpCode = websocketpp::frame::opcode::value;

  using ChannelId = uint32_t;
  using SubscriptionId = uint32_t;

  struct ChannelWithoutId 
  {
    std::string topic;
    std::string encoding;
    std::string schemaName;
    std::string schema;

    bool operator==(const ChannelWithoutId& other) const 
    {
      return topic == other.topic && encoding == other.encoding && schemaName == other.schemaName &&
            schema == other.schema;
    }
  };

  struct Channel : ChannelWithoutId 
  {
    ChannelId id;

    explicit Channel(ChannelId id, ChannelWithoutId ch)
        : ChannelWithoutId(std::move(ch))
        , id(id) {}

    friend void to_json(json& j, const Channel& channel) 
    {
      j = {
        {"id", channel.id},
        {"topic", channel.topic},
        {"encoding", channel.encoding},
        {"schemaName", channel.schemaName},
        {"schema", channel.schema},
      };
    }

    bool operator==(const Channel& other) const 
    {
      return id == other.id && ChannelWithoutId::operator==(other);
    }
  };

  enum class BinaryOpcode : uint8_t 
  {
    MESSAGE_DATA = 1,
  };

  enum class StatusLevel : uint8_t 
  {
    INFO = 0,
    WARNING = 1,
    ERROR = 2,
  };

  class Server final 
  {
  public:
    static const std::string SUPPORTED_SUBPROTOCOL;

    explicit Server(uint16_t port, std::string name);
    ~Server();

    Server(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(const Server&) = delete;
    Server& operator=(Server&&) = delete;

    void run();
    void stop();

    ChannelId addChannel(ChannelWithoutId channel);
    void removeChannel(ChannelId chanId);

    void setSubscribeHandler(std::function<void(ChannelId)> handler);
    void setUnsubscribeHandler(std::function<void(ChannelId)> handler);

    void sendMessage(ChannelId chanId, uint64_t timestamp, std::string_view data);

    AsioServer::endpoint_type& getEndpoint() & {
      return _server;
    }

  private:
    struct ClientInfo {
      std::string name;
      ConnHandle handle;
      std::unordered_map<SubscriptionId, ChannelId> subscriptions;
      std::unordered_map<ChannelId, std::unordered_set<SubscriptionId>> subscriptionsByChannel;

      ClientInfo(std::string name_, ConnHandle handle_) : name(name_), handle(handle_){};

      ClientInfo(const ClientInfo&) = delete;
      ClientInfo& operator=(const ClientInfo&) = delete;

      ClientInfo(ClientInfo&&) = default;
      ClientInfo& operator=(ClientInfo&&) = default;
    };

    uint16_t _port;
    std::string _name;
    AsioServer _server;

    uint32_t _nextChannelId = 0;
    std::map<ConnHandle, ClientInfo, std::owner_less<>> _clients;
    std::unordered_map<ChannelId, Channel> _channels;
    std::function<void(ChannelId)> _subscribeHandler;
    std::function<void(ChannelId)> _unsubscribeHandler;

    bool validateConnection(ConnHandle hdl);
    void handleConnectionOpened(ConnHandle hdl);
    void handleConnectionClosed(ConnHandle hdl);
    void handleMessage(ConnHandle hdl, MessagePtr msg);

    void sendJson(ConnHandle hdl, json&& payload);
    void sendBinary(ConnHandle hdl, const std::vector<uint8_t>& payload);

    bool anySubscribed(ChannelId chanId) const;
  };
}

#endif