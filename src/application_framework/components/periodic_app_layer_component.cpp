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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#include <iostream>
#include <thread>
#include <chrono>
#include "dls2/application_framework/components/periodic_app_layer_component.hpp"
#include "dls2/util/time/time.hpp"

#ifndef NDEBUG
#include <sstream>
#include <chrono>
#include "dls2/util/log/log.hpp"
#endif

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
PeriodicAppLayerComponent::PeriodicAppLayerComponent(const ID_t &ID, const period_t &inPeriod) :
	AppLayerComponent(ID),
	period(inPeriod),
	should_run(false),
	pause_mutex(),
	is_paused(false),
	pause_request(),
	command_manager(),
	scout(ID)
{
	this->command_manager.addCommand<void, ARGVOID>
	(
		this->getID(),
		std::string("pause"),
		std::string("Pause the execution of ") + this->getID(),
		std::function<void(ARGVOID)>
		(
			[&](ARGVOID)
			{
				std::lock_guard<std::mutex> lock(this->pause_mutex);
				this->is_paused = true;
				this->pause_request.notify_all();
				std::cout << this->getID() << " paused execution" << std::endl;
				scout << this->getID() << " paused execution" << std::endl;
			}
		)
	);

	this->command_manager.addCommand<void, ARGVOID>
	(
		this->getID(),
		"continue",
		std::string("Continue the execution of ") + this->getID(),
		std::function<void(ARGVOID)>
		(
			[&](ARGVOID)
			{
				std::lock_guard<std::mutex> lock(this->pause_mutex);
				this->is_paused = false;
				this->pause_request.notify_all();
				std::cout << this->getID() << " continued execution" << std::endl;
				scout << this->getID() << " continued execution" << std::endl;
			}
		)
	);
}

// =============================================================================
// Interface Override Functions
// =============================================================================
AppLayerComponent::Status PeriodicAppLayerComponent::run()
{
	setStatus(Status::RUNNING);
	this->should_run = true;
	auto next_loop_time = this->period + Time::now();
	do
	{
		// #ifndef NDEBUG
		// 	auto begin_epoch = std::chrono::system_clock::now();
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
		if(Time::now() > next_loop_time)
		{
			setStatus(Status::BREAKING_REALTIME);
		}

		// TODO("use realtime sleep here")
		// std::this_thread::sleep_until(next_loop_time);
		Time::sleep_until(next_loop_time);
		next_loop_time = this->period + Time::now(); // TODO ABC

		// #ifndef NDEBUG
		// {
		// 	auto end_epoch = std::chrono::system_clock::now();
		// 	double useconds =
		// 		std::chrono::duration<double, std::ratio<1, 1'000'000>>
		// 			(end_epoch - begin_epoch).count();

		// 	std::stringstream ss;
		// 	ss << "Component " << this->getID() << " has period "
		// 		<< std::chrono::duration<double, std::ratio<1, 1'000'000>>(this->period).count() << " useconds. epoch ran in: " << useconds << " useconds "
		// 		<< std::endl;
		// 	logging::clog << ss.str() << std::endl;
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
