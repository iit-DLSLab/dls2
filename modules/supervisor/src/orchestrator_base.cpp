#include "dls2/supervisor/orchestrator_base.hpp"

namespace dls
{
    OrchestratorBase::OrchestratorBase(
        size_t telemetry_thread_period_ms, 
        size_t event_to_publish, 
        const std::string &ID, 
        const std::shared_ptr<state_machine::StateMachine> &sm)
        : dls::PeriodicAppPlugin(ID)
        , event_listener_(ID)
        , telemetry_thread_period_ms_(telemetry_thread_period_ms) 
        , event_to_publish_(event_to_publish) 
        , telemetry_manager_(telemetry_readers_, telemetry_writers_)
        , sm_(sm)
    {};

    void OrchestratorBase::activation()
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
        dls::App::activation();
    }

    void OrchestratorBase::telemetryCallback()
    {   
        while (!should_quit)
        {
            for (size_t i = 0; i < telemetry_readers_.size(); ++i) {
                auto& reader = telemetry_readers_[i];
                if (!reader) {
                    std::cerr << "[orchestrator telemetry] null reader at index " << i
                                << " for " << this->getID() << std::endl;
                    continue;
                }
                if(!reader->is_receiving_data()){
                    continue;
                }
                reader->read();
            }

            // Prepare events for publication
            std::vector<dls2_interface::msg::EventLog> events_to_publish;
            {
                std::lock_guard<std::mutex> lock(event_mutex_);
				size_t event_count = 0;
				while (!telemetry_events_.empty() && event_count < event_to_publish_) {
					events_to_publish.push_back(telemetry_events_.front());
					telemetry_events_.pop_front();
					event_count++;
				}
            }

            telemetryMain(events_to_publish);

            for (size_t i = 0; i < telemetry_writers_.size(); ++i) {
                auto& writer = telemetry_writers_[i];
                if (!writer) {
                    std::cerr << "[orchestrator telemetry] null writer at index " << i
                                << " for " << this->getID() << std::endl;
                    continue;
                }

                writer->publish();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(telemetry_thread_period_ms_));
        }
    }

    void OrchestratorBase::run(const std::chrono::system_clock::time_point &time)
    {
        try
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
					telemetry_events_.push_back(event);
                }

                events_priority_queue_tmp = events_priority_queue_;
            }

            orchestrate(time, events_priority_queue_tmp);

            {
                std::lock_guard<std::mutex> lock(event_mutex_);
                events_priority_queue_ = std::move(events_priority_queue_tmp);
            }

            write();
        }
        catch (const std::exception& e)
        {
            std::cerr << "[orchestrator run] exception in " << this->getID() << ": " << e.what() << std::endl;
            throw;
        }
        catch (...)
        {
            std::cerr << "[orchestrator run] unknown exception in " << this->getID() << std::endl;
            throw;
        }
    }

    extern "C" PeriodicAppPlugin *create(size_t telemetry_thread_period_ms, size_t event_to_publish, const std::string& ID)
    {
        /*call_plugin_constructor*/
        return new OrchestratorBase(telemetry_thread_period_ms, event_to_publish, ID);
    }

    extern "C" void destroy(PeriodicAppPlugin *p)
    {
        delete p;
    }
  
}
