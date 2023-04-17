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
#include <iostream>
#include <thread>
#include <chrono>
#include "dls2/components/periodic_app_layer_component.hpp"
#include "dls2/util/time/time.hpp"

#ifndef NDEBUG
#include <sstream>
#include <chrono>
#include "dls2/log/log.hpp"
#endif

#include <sys/wait.h>
#include <cstdlib>
#include <tuple>
#include <unistd.h>
#include <pthread.h>

#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

int sched_setattr(pid_t pid,
               const struct sched_attr *attr,
               unsigned int flags)
{
    return syscall(__NR_sched_setattr, pid, attr, flags);
}

int sched_getattr(pid_t pid,
               struct sched_attr *attr,
               unsigned int size,
               unsigned int flags)
{
    return syscall(__NR_sched_getattr, pid, attr, size, flags);
}

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
PeriodicAppLayerComponent::PeriodicAppLayerComponent(const std::string &ID) 
	: AppLayerComponent(ID)
	, config_scheduler(YAML::LoadFile("/usr/include/dls2/schedulers/" + ID + "/scheduler.yaml"))
	, period(std::chrono::milliseconds(config_scheduler["period"].as<int>()))
	, should_run(false)
	, pause_mutex()
	, is_paused(false)
	, pause_request()
{
	memset(&scheduler_attributes, 0, sizeof(struct sched_attr));
	scheduler_attributes.size = sizeof(struct sched_attr);
	scheduler_attributes.sched_policy = SCHED_DEADLINE;

	// Period defined in nanoseconds
	double sched_runtime_factor = config_scheduler["runtime_factor"].as<double>();
	double sched_deadline_factor = config_scheduler["deadline_factor"].as<double>();
	scheduler_attributes.sched_period  = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period).count();
	scheduler_attributes.sched_runtime = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*sched_runtime_factor).count();
	scheduler_attributes.sched_deadline = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*sched_deadline_factor).count();

	this->command_manager.addCommand<>
	(
		"pause",
		"Pause the execution of " + this->getID(),
		std::function<bool()>([&]()->bool
        {
			std::lock_guard<std::mutex> lock(this->pause_mutex);
			this->is_paused = true;
			this->pause_request.notify_all();
			std::cout << this->getID() << " EXECUTION PAUSED" << std::endl;
			scout_sys << this->getID() << " paused execution" << std::endl;
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
			std::cout << this->getID() << " continued execution" << std::endl;
			scout_sys << this->getID() << " continued execution" << std::endl;
            return true;
		}),
		{{1,0}},
		true
	);
}

// =============================================================================
// Interface Override Functions
// =============================================================================
AppLayerComponent::Status PeriodicAppLayerComponent::run()
{
	setStatus(Status::RUNNING);
	this->should_run = true;
	// auto next_loop_time = this->period + Time::now();

    int ret;
    unsigned int flags = 0;

    ret = sched_setattr(0, &scheduler_attributes, flags);
    if (ret < 0) {
        perror("sched_setattr");
        exit(-1);
    }
	// auto begin_epoch = std::chrono::high_resolution_clock::now();

	do
	{
		// #ifndef NDEBUG
			// auto begin_epoch = std::chrono::system_clock::now();
		// #endif

		// auto last_loop_time = Time::now();
		// Calculate when the next period needs to start
		// auto next_loop_time = this->period + Time::now();
		// auto next_loop_time = this->period + last_loop_time;

		// Run one epoch
		std::chrono::system_clock::time_point tp =
			std::chrono::time_point_cast
			<
				std::chrono::system_clock::duration,
				std::chrono::system_clock,
				std::chrono::duration<double>
			>(Time::now());

		// #ifndef NDEBUG
		// 	auto begin_run = std::chrono::system_clock::now();
		// #endif
		run(tp);
		// #ifndef NDEBUG
		// {
		// 	auto end_run = std::chrono::system_clock::now();
		// 	double useconds =
		// 		std::chrono::duration<double, std::ratio<1, 1'000'000>>
		// 			(end_run - begin_run).count();
		// 	std::stringstream ss;
		// 	ss << "Component " << this->getID() << " ran in: "
		// 		<< useconds << " useconds " << std::endl;
		// 	logging::cdbg << ss.str() << std::endl;
		// }
		// #/hondif

		// Check realtime
		// if(Time::now() > next_loop_time)
		// {
		// 	setStatus(Status::BREAKING_REALTIME);
		// }

		// // TODO("use realtime sleep here")
		// // std::this_thread::sleep_until(next_loop_time);
		// Time::sleep_until(next_loop_time);
		// next_loop_time = this->period + Time::now(); // TODO ABC

		// #ifndef NDEBUG
		// {
			// auto end_epoch = std::chrono::system_clock::now();
			// double useconds =
			// 	std::chrono::duration<double, std::ratio<1, 1'000'000>>
			// 		(end_epoch - begin_epoch).count();
			// auto delta = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - begin_epoch).count();

			// std::stringstream ss;
			// ss << "Component " << this->getID() << " has period "
			// 	// << std::chrono::duration<double, std::ratio<1, 1'000'000>>(this->period).count() << " useconds. epoch ran in: " << useconds << " useconds "
			// 	<< " useconds. epoch ran in: " << delta << " useconds "
			// 	<< std::endl;
			// std::cout << ss.str() << std::endl;

			// begin_epoch = std::chrono::high_resolution_clock::now();
			
		// }
		// #endif

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

	}while(this->should_run);

	return this->getStatus();
}

AppLayerComponent::Status PeriodicAppLayerComponent::stop()
{
	// inform the component that it should exit at the end of the epoch
	this->should_run = false;

	// unpause the component if it has been paused
	{
		std::lock_guard<std::mutex> lock(this->pause_mutex);
		this->is_paused = false;
		this->pause_request.notify_all();
	}
	
	this->setStatus(Status::STOPPED);
	return this->getStatus();
}
