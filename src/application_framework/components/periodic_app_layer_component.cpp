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
#include "application_framework/components/periodic_app_layer_component.hpp"

// =============================================================================
// Constructors
// =============================================================================
dls::PeriodicAppLayerComponent::PeriodicAppLayerComponent(const period_t &inPeriod) :
	period(inPeriod),
	should_run(false)
{ }

// =============================================================================
// Interface Override Functions
// =============================================================================
dls::AppLayerComponent::Status dls::PeriodicAppLayerComponent::run()
{
	setStatus(Status::RUNNING);
	this->should_run = true;
	do
	{
		// Calculate when the next period needs to start
		auto next_loop_time = this->period + std::chrono::system_clock::now();

		// Run one epoch
		run(std::chrono::system_clock::now());

		// Check realtime
		if(std::chrono::system_clock::now() > next_loop_time)
		{
			setStatus(Status::BREAKING_REALTIME);
		}

		TODO("use realtime sleep here")
		std::this_thread::sleep_until(next_loop_time);

	}while(this->should_run);

	return this->getStatus();
}

dls::AppLayerComponent::Status dls::PeriodicAppLayerComponent::stop()
{
	this->should_run = false;
	this->setStatus(Status::STOPPED);
	return this->getStatus();
}
