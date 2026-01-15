#include "dls2/application/periodic_app.hpp"
#include <fstream>
#include <filesystem>


using namespace dls;

PeriodicApp::PeriodicApp(const std::string &ID) 
	: App(ID)
	, failure(false)
	, pause_mutex()
	, is_paused(false)
	, pause_request()
	, time_factor()
	, realtime_prec(true)
	, realtime_curr(true)
{
	getSchedulerConfig();

	period = std::chrono::milliseconds(config_scheduler["period"].as<int>());
	sched_runtime_factor = config_scheduler["runtime_factor"].as<double>();
	sched_deadline_factor = config_scheduler["deadline_factor"].as<double>();
	runtime = period * sched_runtime_factor;
	deadline = period * sched_deadline_factor;

    this->pid = syscall(SYS_gettid);
	this->cur_time_factor = this->time_factor.getRealTimeFactor();

	memset(&scheduler_attributes, 0, sizeof(struct sched_attr));
	scheduler_attributes.size = sizeof(struct sched_attr);
	scheduler_attributes.sched_policy = SCHED_DEADLINE;

	scheduler_attributes.sched_period  = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*cur_time_factor).count();
	scheduler_attributes.sched_runtime = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(runtime*cur_time_factor).count();
	scheduler_attributes.sched_deadline = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(deadline*cur_time_factor).count();

	// set RUN and DEACTIVATION state as real time state
	sm.RUN.makeRealTime();
	sm.DEACTIVATION.makeRealTime();

	dt = period.count()/(1000000.0);
	current_frequency_ = getDesiredFrequency();
	loop_time_prec = std::chrono::steady_clock::now();

	this->command_manager.addCommand<>
	(
		"pause",
		"Pause the execution of " + this->getID(),
		std::function<bool()>([&]()->bool
        {
			std::lock_guard<std::mutex> lock(this->pause_mutex);
			this->is_paused = true;
			this->pause_request.notify_all();
			scout_sys << this->getID() << " execution paused" << std::endl;
            return true;
		}),
		{{0,1}},
		true
	);

	this->command_manager.addCommand<>
	(
		"continue",
		"Continue the execution of " + this->getID(),
		std::function<bool()>([&]()->bool
		{
			std::lock_guard<std::mutex> lock(this->pause_mutex);
			this->is_paused = false;
			this->pause_request.notify_all();
			scout_sys << this->getID() << " execution continued" << std::endl;
            return true;
		}),
		{{1,0}},
		true
	);

	process_resource_monitor_ = std::make_unique<ProcessResourceMonitor>(
						this->pid, this->safety_layer_config_->process_monitor_window_size);
}

void PeriodicApp::getSchedulerConfig()
{
	char * val;
	val = getenv("DLS_SCHEDULER_PATH");
	std::string scheduler_path = "";
	if (val != NULL) {
		scheduler_path = val;
		std::cout << "SCHEDULER: env var\n";
	}
	else{
		std::string default_path = "/usr/include/dls2/util/messaging/scheduler.yaml";
		std::ifstream f(default_path.c_str());
    	auto default_file_exists = f.good();
		if(default_file_exists){
			scheduler_path = default_path;
			std::cout << "SCHEDULER: def path\n";
			config_scheduler = YAML::LoadFile(scheduler_path)[this->getID()];
			return;
		}else{
			scheduler_path = "/usr/include/dls2/schedulers/" + this->getID() + "/scheduler.yaml";
			std::cout << "SCHEDULER: plugin path\n";
		}
	}
	config_scheduler = YAML::LoadFile(scheduler_path);
	std::cout << "Period? " << config_scheduler["period"].as<int>() <<"\n";

}

void PeriodicApp::childMonitor()
{
	[[maybe_unused]] static const bool initialized = [this] 
	{
		// Running at first thread callback execution only 
       	this->robust_event_notifier.setSpammingThreshold(this->safety_layer_config_->spam_threshold);
        return true;
    }();

	{
		std::lock_guard<std::mutex> lock(this->frequency_mutex_);
		status_msg.current_frequency() =
			this->current_frequency_;
	}
	status_msg.desired_frequency() = getDesiredFrequency();
	status_msg.realtime() = static_cast<uint8_t>(realtime_curr);
	status_msg.cpu_usage() = process_resource_monitor_->getCpuPercent();
	status_msg.mem_usage() = process_resource_monitor_->getMemPercent();

	// notify if the process is not running at the expected frequency
	if(this->safety_layer_config_->enable_wrong_process_frequency && !realtime_curr){
		this->robust_event_notifier.notify(	
								EventID::WRONG_PROCESS_FREQUENCY,
								EventSeverity::WARNING,
								"Des freq: " + std::to_string(status_msg.desired_frequency()) + " Hz, " 
								+ "Curr freq: " + std::to_string(status_msg.current_frequency()) + " Hz");
	}

	// notify if the process is using more cpu than expected over time
	if(this->safety_layer_config_->enable_cpu_usage_too_high &&
	  (status_msg.cpu_usage() > this->safety_layer_config_->cpu_threshold)){
		this->robust_event_notifier.notify(	
								EventID::CPU_USAGE_TOO_HIGH,
								EventSeverity::WARNING,
								"CPU usage is " + std::to_string(status_msg.cpu_usage()) + " (threshold is: " 
								+ std::to_string(this->safety_layer_config_->cpu_threshold) + ")");
	}

	// notify if the process is using more memory than expected over time
	if(this->safety_layer_config_->enable_mem_usage_too_high && (status_msg.mem_usage() > this->safety_layer_config_->mem_threshold)){
		this->robust_event_notifier.notify(	
								EventID::MEM_USAGE_TOO_HIGH,
								EventSeverity::WARNING,
								"MEM usage is " + std::to_string(status_msg.mem_usage()) + " (threshold is: " 
								+ std::to_string(this->safety_layer_config_->mem_threshold) + ")");
	}
}

AppStatus PeriodicApp::run()
{
	
	//set RT scheduling policy
	setRTSchedulerPolicy();

	bool failure = false;
	realtime_prec = true;
	realtime_curr = realtime_prec;
	while(      !sm.isRaised(sm.deactivation_request)
	        &&  !sm.isRaised(sm.quit_request)
	        &&  !failure)
	{
		
	    // Check realtime
	    checkRT();

		// Check hardware resource usage
		auto process_resource_monitor_success = process_resource_monitor_->update();
		if(process_resource_monitor_success != 0){
			std::cout << this->ID_ << ": resource monitor failed, exit code: " << process_resource_monitor_success << "\n";
		}

	    // Run
	    run(std::chrono::time_point_cast<std::chrono::system_clock::duration, std::chrono::system_clock, std::chrono::duration<double>>(std::chrono::system_clock::now()));

	    // Check failure
	    failure = checkFailure();

		// Pause execution if a pause request was made
	    if(isPaused())
	       pauseExecution();
		
	    // Update scheduler attributes if the current time factor has changed
	    if(newTimeFactor())
	    {
	        setRTSchedulerPolicy();
	    }

	    sched_yield();
	}

	if (failure)
	{
	    sm.nextState(sm.failure);
	}
	else if(sm.isRaised(sm.deactivation_request))
	    sm.nextState(sm.deactivation_request);
	else if (sm.isRaised(sm.quit_request))
	{
	    sm.nextState(sm.quit_request);
	}

	return this->getStatus();
}

void PeriodicApp::setRTSchedulerPolicy()
{
	memset(&scheduler_attributes, 0, sizeof(struct sched_attr));
	scheduler_attributes.size = sizeof(struct sched_attr);
	scheduler_attributes.sched_policy = SCHED_DEADLINE;
	
	scheduler_attributes.sched_period  = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*cur_time_factor).count();
	scheduler_attributes.sched_runtime = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(runtime*cur_time_factor).count();
	scheduler_attributes.sched_deadline = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(deadline*cur_time_factor).count();

	unsigned int flags = 0;
    int ret = sched_setattr(0, &scheduler_attributes, flags);
    if (ret < 0) {
        perror("sched_setattr");
        exit(-1);
    }
}

void PeriodicApp::pauseExecution()
{
	std::unique_lock<std::mutex> lock(this->pause_mutex);
	this->pause_request.wait
	(
		lock,
		[&]{ return !this->is_paused; }
	);
}

bool PeriodicApp::newTimeFactor()
{
	if(abs(this->time_factor.getRealTimeFactor() - this->cur_time_factor) > 0.02)
	{
		this->cur_time_factor = this->time_factor.getRealTimeFactor();
		return true;
	}
	return false;
}

bool PeriodicApp::isPaused()
{
	return is_paused;
}

bool PeriodicApp::checkFailure()
{
	return failure;
}

void PeriodicApp::checkRT()
{
	{
		std::lock_guard<std::mutex> lock(this->frequency_mutex_);
		realtime_curr = Time::checkFrequency(getDesiredFrequency(), loop_time_prec, current_frequency_);
	}

	// notify if the process is not running in real time
	if  (realtime_curr!=sm.state->realtime || 
		(realtime_curr==sm.state->realtime && !realtime_prec))
	{
		sm.notifyRT(realtime_curr);
	}
	realtime_prec = realtime_curr;
}
void PeriodicApp::setFailure()
{
	failure = true;
}

bool PeriodicApp::deactivating()
{
	bool deactivated = false;
	realtime_prec = true;
	realtime_curr = realtime_prec;
	while(!deactivated && !sm.isRaised(sm.quit_request))
	{
	    // Check realtime
	    checkRT();

	    // Run
	    deactivated = deactivation(std::chrono::time_point_cast<std::chrono::system_clock::duration, std::chrono::system_clock, std::chrono::duration<double>>(std::chrono::system_clock::now()));

	    // Pause execution if a pause request was made
	    if(isPaused())
	        pauseExecution();
		
	    // Update scheduler attributes if the current time factor has changed
	    if(newTimeFactor())
	    {
	        setRTSchedulerPolicy();
	    }

	    sched_yield();
	}

	return deactivated;
}

bool PeriodicApp::deactivation(const std::chrono::system_clock::time_point&)
{
	return true;
}

void PeriodicApp::close()
{
	// unpause the component if it has been paused
	{
		std::lock_guard<std::mutex> lock(this->pause_mutex);
		this->is_paused = false;
		this->pause_request.notify_all();
	}
}

PeriodicApp::period_t PeriodicApp::getPeriod(){
	return period;
}

double PeriodicApp::getDesiredFrequency() const
{
	return 1 / dt;
}