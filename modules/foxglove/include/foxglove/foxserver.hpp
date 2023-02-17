#ifndef FOX_SERVER_HPP
#define FOX_SERVER_HPP

#pragma once

#include "foxglove/webserver.hpp"
#include "foxglove/dynamic_types_utils.hpp"

namespace dls {

    using json = nlohmann::json;

    class FoxServer : public dls::DDSPartListener
    {
    public:
        FoxServer();
        ~FoxServer();

    private:
        void serverFunc();
        void on_topic_discovery(const std::string& topic_name, const std::string& type_name) override;

        std::thread *serverThread;
        std::shared_ptr<boost::asio::steady_timer> timer;
        Server foxserver;

        dls::DDSParticipant ddslink;
        std::function<void()> setTimer;

        std::set<int> send_flags;
        std::set<int> timer_flags;

        std::mutex sendFlagsMutex;
    };
}

#endif