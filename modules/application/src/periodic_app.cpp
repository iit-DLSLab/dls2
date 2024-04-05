#include "dls2/application/periodic_app.hpp"
#include "dls2/util/time/time.hpp"
#include <fstream>
#include <filesystem>


using namespace dls;

PeriodicApp::PeriodicApp(const std::string &ID) 
	: App(ID)
	, config_scheduler(YAML::LoadFile("/usr/include/dls2/schedulers/" + ID + "/scheduler.yaml"))
	, period(std::chrono::milliseconds(config_scheduler["period"].as<int>()))
	, sched_runtime_factor(config_scheduler["runtime_factor"].as<double>())
	, sched_deadline_factor(config_scheduler["deadline_factor"].as<double>())
	, runtime(period*sched_runtime_factor)
	, deadline(period*sched_deadline_factor)
	, failure(false)
	, pause_mutex()
	, is_paused(false)
	, pause_request()
	, time_factor()
{
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
}

AppStatus PeriodicApp::run()
{
	//set RT scheduling policy
	setRTSchedulerPolicy();

	bool failure = false;
	bool realtime_prec = true;
	bool realtime_curr = realtime_prec;
	while(      !sm.isRaised(sm.deactivation_request)
	        &&  !sm.isRaised(sm.quit_request)
	        &&  !failure)
	{
	    // Compute when the next period should start
	    auto next_loop_time = getPeriod() + std::chrono::system_clock::now();

	    // Run
	    run(std::chrono::time_point_cast<std::chrono::system_clock::duration, std::chrono::system_clock, std::chrono::duration<double>>(std::chrono::system_clock::now()));

	    // Check failure
	    failure = checkFailure();

	    // Check realtime
	    realtime_curr =  checkRT(next_loop_time);
	    if  (realtime_curr!=sm.state->realtime || 
	        (realtime_curr==sm.state->realtime && !realtime_prec))
	    {
	        sm.notifyRT(realtime_curr);
	    }
	    realtime_prec = realtime_curr;

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
bool PeriodicApp::checkRT(const std::chrono::time_point<	std::chrono::_V2::system_clock, 
													std::chrono::duration<double, std::ratio<1, 1000000000>>>& next_loop_time)
{
	return std::chrono::system_clock::now() <= next_loop_time;
}
void PeriodicApp::setFailure()
{
	failure = true;
}

void PeriodicApp::deactivation()
{
	bool deactivated = false;
	bool realtime_prec = true;
	bool realtime_curr = realtime_prec;
	while(!deactivated && !sm.isRaised(sm.quit_request))
	{
	    // Compute when the next period should start
	    auto next_loop_time = getPeriod() + std::chrono::system_clock::now();

	    // Run
	    deactivated = deactivation(std::chrono::time_point_cast<std::chrono::system_clock::duration, std::chrono::system_clock, std::chrono::duration<double>>(std::chrono::system_clock::now()));

	    // Check realtime
	    realtime_curr = checkRT(next_loop_time);
	    if  (realtime_curr!=sm.state->realtime || 
	        (realtime_curr==sm.state->realtime && !realtime_prec))
	    {
	        sm.notifyRT(realtime_curr);
	    }
	    realtime_prec = realtime_curr;

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

	if (deactivated)
	{
	    sm.nextState(sm.deactivated);
	}
	else if (sm.isRaised(sm.quit_request))
	{
	    sm.nextState(sm.quit_request);
	}
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