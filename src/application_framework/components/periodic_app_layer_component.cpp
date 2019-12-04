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
#include <thread>
#include <chrono>
#include "application_framework/components/periodic_app_layer_component.hpp"
#include "util/time/time.hpp"

#ifndef NDEBUG
#include <sstream>
#include <chrono>
#include "util/log/log.hpp"
#endif

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
PeriodicAppLayerComponent::PeriodicAppLayerComponent(const ID_t &ID, const period_t &inPeriod) :
	AppLayerComponent(ID),
	period(inPeriod),
	should_run(false)
{ }

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
		#ifndef NDEBUG
			auto begin_epoch = std::chrono::system_clock::now();
		#endif
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

		#ifndef NDEBUG
			auto begin_run = std::chrono::system_clock::now();
		#endif
		run(tp);
		#ifndef NDEBUG
		{
			auto end_run = std::chrono::system_clock::now();
			double useconds =
				std::chrono::duration<double, std::ratio<1, 1'000'000>>
					(end_run - begin_run).count();
			std::stringstream ss;
			ss << "Component " << this->getID() << " ran in: "
				<< useconds << " useconds " << std::endl;
			logging::cout << ss.str() << logging::endl;
		}
		#endif

		// Check realtime
		if(Time::now() > next_loop_time)
		{
			setStatus(Status::BREAKING_REALTIME);
		}

		TODO("use realtime sleep here")
		// std::this_thread::sleep_until(next_loop_time);
		Time::sleep_until(next_loop_time);
		next_loop_time = this->period + Time::now(); // TODO ABC

		#ifndef NDEBUG
		{
			auto end_epoch = std::chrono::system_clock::now();
			double useconds =
				std::chrono::duration<double, std::ratio<1, 1'000'000>>
					(end_epoch - begin_epoch).count();

			std::stringstream ss;
			ss << "Component " << this->getID() << " has period "
				<< std::chrono::duration<double, std::ratio<1, 1'000'000>>(this->period).count() << " useconds. epoch ran in: " << useconds << " useconds "
				<< std::endl;
			logging::cout << ss.str() << logging::endl;
		}
		#endif

	}while(this->should_run);

	return this->getStatus();
}

AppLayerComponent::Status PeriodicAppLayerComponent::stop()
{
	this->should_run = false;
	this->setStatus(Status::STOPPED);
	return this->getStatus();
}
