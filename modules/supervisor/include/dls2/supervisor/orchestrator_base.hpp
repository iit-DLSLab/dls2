
#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <list>
#include <thread>
#include <queue>
#include <deque>

#include "dls2/plugin/periodic_app_plugin.hpp"
#include "dls2/state_machine/state_machine.hpp"
#include "dls2/log/event_logger.hpp"
#include "dls2/supervisor/telemetry_base.hpp"

namespace dls
{

	using EventsPriorityQueue = std::priority_queue<dls2_interface::msg::EventLog, 
                            std::vector<dls2_interface::msg::EventLog>, 
                            logging::CompareEvents>;

	class OrchestratorBase : public PeriodicAppPlugin {

	public:

		explicit OrchestratorBase(
			size_t telemetry_thread_period_ms, 
			size_t event_to_publish, 
			const std::string &ID, 
			const std::shared_ptr<state_machine::StateMachine> &sm = nullptr);
		~OrchestratorBase() = default;

    	void run(const std::chrono::system_clock::time_point &time) override;

	protected:

		/**
		 * @brief Virtual method implementing core processing in run method related to specifically typed messages. 
		 * A logical structure like the following one is expected but not enforced in this method body.
		 * 
		 *  get data sent from Control Station
		 *  update state machine state
		 *  fill in dls status msgs
		 *  fill in dls action msgs
		 */
		virtual void orchestrate(const std::chrono::system_clock::time_point&, EventsPriorityQueue&) {};

		/**
		 * @brief Virtual method implementing core processing in telemetry callback related to specifically typed messages.
		 * A logical structure like the following one is expected but not enforced in this method body.
		 * 
		 * 	telemetry_manager.tick(input_struct, output_struct)
		 * 
		 */
		virtual void telemetryMain(const std::vector<dls2_interface::msg::EventLog>&) {};

		void activation() override;

		// Events
		std::mutex event_mutex_;
		logging::EventListener event_listener_;
        EventsPriorityQueue events_priority_queue_;
		std::deque<dls2_interface::msg::EventLog> telemetry_events_;

		// Telemetry
		std::vector<std::shared_ptr<dls::ReaderBase>> telemetry_readers_;
		std::vector<std::shared_ptr<dls::WriterBase>> telemetry_writers_;
		size_t telemetry_thread_period_ms_{500};
		size_t event_to_publish_{1};
		std::atomic<bool> telemetry_started_{false};
		std::thread telemetry_thread_;
		TelemetryBase telemetry_manager_;

		// State machine
		std::shared_ptr<state_machine::StateMachine> sm_;

	private:

		void telemetryCallback();

	};

} // end namespace dls