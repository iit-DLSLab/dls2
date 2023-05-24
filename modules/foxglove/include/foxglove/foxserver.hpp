#ifndef FOX_SERVER_HPP
#define FOX_SERVER_HPP

#pragma once

#include "webserver.hpp"
#include "dynamic_types_utils.hpp"
#include "mcap/writer.hpp"
#include <map>

namespace dls {

    using json = nlohmann::json;

    class FoxServer : public dls::DDSPartListener
    {
    public:
        FoxServer();
        ~FoxServer();

        void record_mcap_log(bool record_mcap, const std::string& timestamp = std::string());

    private:
        void serverFunc();
        void on_topic_discovery(const std::string& topic_name, const std::string& type_name) override;

        std::shared_ptr<std::thread> server_thread_;
        std::shared_ptr<boost::asio::steady_timer> timer_;
        Server webserver_;  // Foxglove web server

        mcap::McapWriter mcap_writer_;
        mcap::Message mcap_msg_;
        bool mcap_ongoing_recording_{false};
        std::map<std::string, mcap::ChannelId> mcap_topics_channels_{};

        dls::DDSParticipant dds_link_;
        std::function<void()> set_timer_;

        std::set<int> send_flags_;
        std::set<int> timer_flags_;

        std::mutex send_flags_mutex_;
    };
}

#endif