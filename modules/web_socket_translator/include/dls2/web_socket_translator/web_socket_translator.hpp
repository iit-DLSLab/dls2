#ifndef WEB_SOCKET_TRANSLATOR_HPP
#define WEB_SOCKET_TRANSLATOR_HPP

#include "dls2/service/service.hpp"
#include "dls2/command/command_manager.hpp"

#include "webserver.hpp"
#include "dynamic_types_utils.hpp"
#include "mcap_writer_utils.hpp"
#include "mcap_reader_utils.hpp"

namespace dls
{
	class WebSocketTranslator : public dls::Service, public dls::DDSPartListener
	{
	public:
		WebSocketTranslator(std::string& ID);
		~WebSocketTranslator();

	private:
        void serverFunc();
        void on_topic_discovery(const std::string& topic_name, const std::string& type_name) override;

		void record_mcap_log(bool record_mcap, const std::string& timestamp = std::string());
		void playback_mcap_log(bool playback_mcap, const std::string &mcap_log_file = std::string());

		CommandManager command_manager_;

        std::shared_ptr<std::thread> server_thread_;
        std::shared_ptr<boost::asio::steady_timer> timer_;
        Server webserver_;

        std::shared_ptr<dls::DDSParticipant> dds_participant_;

        dls::MCAPWriterUtils mcap_writer_utils_;
        dls::MCAPReaderUtils mcap_reader_utils_;

        std::function<void()> set_timer_;

        std::set<int> send_flags_;
        std::set<int> timer_flags_;

        std::mutex send_flags_mutex_;
	};
} // namespace dls

#endif