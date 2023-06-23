#ifndef FOX_SERVER_HPP
#define FOX_SERVER_HPP

#pragma once

#include "webserver.hpp"
#include "dynamic_types_utils.hpp"
#include "mcap_writer_utils.hpp"
#include "mcap_reader_utils.hpp"

namespace dls
{
    class FoxServer : public dls::DDSPartListener
    {
    public:
        FoxServer();
        ~FoxServer();

        void record_mcap_log(bool record_mcap, const std::string& timestamp = std::string());
        void playback_mcap_log(bool playback_mcap, const std::string &mcap_log_file = std::string());

    private:
        void serverFunc();
        void on_topic_discovery(const std::string& topic_name, const std::string& type_name) override;

        std::shared_ptr<std::thread> server_thread_;
        std::shared_ptr<boost::asio::steady_timer> timer_;
        Server webserver_;  // Foxglove web server

        dls::MCAPWriterUtils mcap_writer_utils_;
        dls::MCAPReaderUtils mcap_reader_utils_;

        dls::DDSParticipant dds_link_;
        std::function<void()> set_timer_;

        std::set<int> send_flags_;
        std::set<int> timer_flags_;

        std::mutex send_flags_mutex_;
    };
}

#endif