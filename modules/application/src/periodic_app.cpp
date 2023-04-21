/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/

#include "dls2/application/periodic_app.hpp"
#include "dls2/util/time/time.hpp"

using namespace dls;

PeriodicApp::PeriodicApp(const std::string &ID) 
	: App(ID)
	, config_scheduler(YAML::LoadFile("/usr/include/dls2/schedulers/" + ID + "/scheduler.yaml"))
	, period(std::chrono::milliseconds(config_scheduler["period"].as<int>()))
	, pause_mutex()
	, is_paused(false)
	, pause_request()
	, time_rate(1)
{
	memset(&scheduler_attributes, 0, sizeof(struct sched_attr));
	scheduler_attributes.size = sizeof(struct sched_attr);
	scheduler_attributes.sched_policy = SCHED_DEADLINE;

	// Period defined in nanoseconds
	double sched_runtime_factor = config_scheduler["runtime_factor"].as<double>();
	double sched_deadline_factor = config_scheduler["deadline_factor"].as<double>();

	scheduler_attributes.sched_period  = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*time_rate).count();
	scheduler_attributes.sched_runtime = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*sched_runtime_factor*time_rate).count();
	scheduler_attributes.sched_deadline = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*sched_deadline_factor*time_rate).count();

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
	setStatus(AppStatus::RUNNING);

    int ret;
    unsigned int flags = 0;

    ret = sched_setattr(0, &scheduler_attributes, flags);
    if (ret < 0) {
        perror("sched_setattr");
        exit(-1);
    }

	do
	{
		// Calculate when the next period needs to start
		auto next_loop_time = this->period + Time::now();

		// Run one epoch
		run(std::chrono::time_point_cast<std::chrono::system_clock::duration, std::chrono::system_clock, std::chrono::duration<double>>(Time::now()));

		// Check realtime
		if(Time::now() > next_loop_time)
		{
		 	setStatus(AppStatus::BREAKING_REALTIME);
		}

		// Pause execution if a pause request was made
		{
			std::unique_lock<std::mutex> lock(this->pause_mutex);
			if(this->is_paused)
			{
				this->pause_request.wait
				(
					lock,
					[&]{ return !this->is_paused; }
				);
			}
		}
		sched_yield();

	}while(!this->should_quit);

	return this->getStatus();
}

AppStatus PeriodicApp::stop()
{
	// inform the component that it should exit at the end of the epoch
	this->should_quit = true;

	// unpause the component if it has been paused
	{
		std::lock_guard<std::mutex> lock(this->pause_mutex);
		this->is_paused = false;
		this->pause_request.notify_all();
	}
	
	this->setStatus(AppStatus::STOPPED);
	return this->getStatus();
}
