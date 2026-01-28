
#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <list>
#include <thread>
#include <queue>

#include "dls2/plugin/periodic_app_plugin.hpp"
#include "dls2/state_machine/state_machine.hpp"
#include "dls2/log/event_logger.hpp"

namespace dls
{

	using EventsPriorityQueue = std::priority_queue<dls2_interface::msg::EventLog, 
                            std::vector<dls2_interface::msg::EventLog>, 
                            logging::CompareEvents>;

	class OrchestratorBase : public PeriodicAppPlugin {

	public:

		explicit OrchestratorBase(const std::string &ID, const std::shared_ptr<state_machine::StateMachine> &sm);
		~OrchestratorBase() = default;

    	void run(const std::chrono::system_clock::time_point &time) override;

	protected:

		 /**
		  * @brief Main orchestration logic (deciding actions to perform considering input/output data copied in getInputOutput).
		  * It implememts the internal state machine step.
		  * 
		  * @param time 
		  */
		virtual void updateState(const std::chrono::system_clock::time_point &time) {};

		virtual void getDlsInputMsgs() {};

        virtual void getDlsOutputMsgs() {};

		virtual void setDlsInputMsgs() {};

        virtual void setDlsOutputMsgs() {};

		/**
		 * @brief Filling in all relevant messages composing the orchestrator action
		 * 
		 * @param time
		 */
		virtual void setActionMsgs(const std::chrono::system_clock::time_point &time) {};


	private:

		/**
		 * @brief Periodically-called thread job managing communication with the Control Station
		 * 
		 */
		void telemetryCallback();

		std::vector<std::shared_ptr<ReaderBase>> telemetry_readers_;
		std::vector<std::shared_ptr<WriterBase>> telemetry_writers_;
		size_t event_to_publish_{ 1 };

		std::mutex event_mutex_;
		logging::EventListener event_listener_;

		bool spawned_telemetry_thread_{ false };
		std::thread telemetry_thread_;
		size_t telemetry_thread_period_ms_{ 200 };

		std::shared_ptr<state_machine::StateMachine> sm_;

        EventsPriorityQueue events_priority_queue_;
	};

} // end namespace dls