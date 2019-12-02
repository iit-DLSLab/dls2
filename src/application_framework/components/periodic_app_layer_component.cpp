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

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
PeriodicAppLayerComponent::PeriodicAppLayerComponent(const period_t &inPeriod) :
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
		run(tp);

		// Check realtime
		if(Time::now() > next_loop_time)
		{
			setStatus(Status::BREAKING_REALTIME);
		}

		TODO("use realtime sleep here")
		// std::this_thread::sleep_until(next_loop_time);
		Time::sleep_until(next_loop_time);
		next_loop_time = this->period + Time::now(); // TODO ABC

	}while(this->should_run);

	return this->getStatus();
}

AppLayerComponent::Status PeriodicAppLayerComponent::stop()
{
	this->should_run = false;
	this->setStatus(Status::STOPPED);
	return this->getStatus();
}
