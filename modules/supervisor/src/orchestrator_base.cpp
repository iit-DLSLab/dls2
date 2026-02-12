#include "dls2/supervisor/orchestrator_base.hpp"

namespace dls
{
    OrchestratorBase::OrchestratorBase(const std::string &ID, const std::shared_ptr<state_machine::StateMachine> &sm)
        : dls::PeriodicAppPlugin(ID)
        , event_listener_(ID)
        , telemetry_manager_(telemetry_readers_, telemetry_writers_)
        , sm_(sm)
    {
        if(!this->telemetry_started_.load()){
            try{
                telemetry_thread_ = std::thread(&OrchestratorBase::telemetryCallback, this);
                this->telemetry_started_.store(true);

            }catch (const std::system_error &e)
            {
                std::cerr << "Failed to start telemetry thread for app " << this->getID() << " @ " << this
                        << " : " << e.what() << " (code " << e.code() << "\n";
                throw;
            }
        }
    };

    void OrchestratorBase::telemetryCallback()
    {   
        while (!should_quit)
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

                if(events_to_publish.empty() || 
                  (events_to_publish.back().component_name() != events_priority_queue_tmp.top().component_name() && 
                   events_to_publish.back().event_id() != events_priority_queue_tmp.top().event_id()))
                {
                    events_to_publish.push_back(events_priority_queue_tmp.top());
                }

                events_priority_queue_tmp.pop();
                event_count++;
            }

            telemetryMain(events_to_publish);

            // Sending msgs to Control Station
            for(auto& writer : telemetry_writers_){
                writer->publish();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(telemetry_thread_period_ms_));
        }
    }

    void OrchestratorBase::run(const std::chrono::system_clock::time_point &time)
    {
        read();

        // Collecting events from DLS2
	    static long int idx_read = 0;
        const auto events_fifo = event_listener_.readEvents(idx_read);
        EventsPriorityQueue events_priority_queue_tmp;
        {
            // Update internal events representation
            std::lock_guard<std::mutex> lock(event_mutex_);

            for(const auto& event : events_fifo){
                events_priority_queue_.push(event);
            }

            events_priority_queue_tmp = events_priority_queue_;
        }

        orchestrate(time, events_priority_queue_tmp);

        write();
    }

    extern "C" PeriodicAppPlugin *create(const std::string& ID)
    {
        /*call_plugin_constructor*/
        return new OrchestratorBase(ID);
    }

    extern "C" void destroy(PeriodicAppPlugin *p)
    {
        delete p;
    }
  
}
