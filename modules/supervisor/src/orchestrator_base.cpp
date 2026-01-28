

#include "dls2/supervisor/orchestrator_base.hpp"

namespace dls
{
    OrchestratorBase::OrchestratorBase(const std::string &ID, const std::shared_ptr<state_machine::StateMachine> &sm)
        : dls::PeriodicAppPlugin(ID)
        , event_listener_(ID)
        , sm_(sm)
    {};

    void OrchestratorBase::run(const std::chrono::system_clock::time_point &time)
    {
        if(!spawned_telemetry_thread_){
            telemetry_thread_ = std::thread(&OrchestratorBase::telemetryCallback, this);
            spawned_telemetry_thread_ = true;
        }

        read();

        // Collecting events from DLS2
        const auto events_fifo = event_listener_.readEvents();
        EventsPriorityQueue events_priority_queue_tmp;
        {
            // Update internal events representation
            std::lock_guard<std::mutex> lock(event_mutex_);

            for(const auto& event : events_fifo){
                events_priority_queue_.push(event);
            }

            events_priority_queue_tmp = events_priority_queue_;
        }

        // TODO: Derived class implementations, ok? Or orchestrate()?
        getDlsInputMsgs();
        updateState(time);
        setDlsOutputMsgs();
        setActionMsgs(time);

        write();
    }

    void OrchestratorBase::telemetryCallback()
    {   
        // Reading msgs from Control Station
        for(auto& reader : telemetry_readers_){
            reader->read();
        }

        // Prepare events for publication
        std::vector<dls2_interface::msg::EventLog> events_to_publish;
        EventsPriorityQueue events_priority_queue_tmp;
        {
            std::lock_guard<std::mutex> lock(event_mutex_);
            events_priority_queue_tmp = events_priority_queue_;
        }
        size_t event_count = 0;
        while (!events_priority_queue_tmp.empty() && event_count < event_to_publish_) {
            events_to_publish.push_back(events_priority_queue_tmp.top());
            events_priority_queue_tmp.pop();
            event_count++;
        }

        // TODO: Derived class implementations, ok? Or telemetrate()?
        setDlsInputMsgs();
        getDlsOutputMsgs();

        // Sending msgs to Control Station
        for(auto& writer : telemetry_writers_){
            writer->publish();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(telemetry_thread_period_ms_));
    }
}
