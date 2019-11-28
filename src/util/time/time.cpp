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
// =============================================================================
// includes
// =============================================================================
#include "util/time/time.hpp"
#include "topics/simulation_time.hpp"

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls;

// =============================================================================
// Class Implementation
// =============================================================================
// -----------------------------------------------------------------------------
// Static Members
// -----------------------------------------------------------------------------
bool Time::use_simulated_time = false;
std::shared_ptr<SubscriberBase<TimeMsgPubSubType>> Time::pTime_sub = nullptr;
Time::time_point_t Time::tick;
std::shared_mutex Time::tick_mutex;

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------
void Time::set_use_simulated_time(bool b)
{
	Time::use_simulated_time = b;
	if(b)
	{
		if(Time::pTime_sub == nullptr)
		{
			Time::pTime_sub = std::make_shared<ClockSubscriber>();
		}

	}
	else
	{
		Time::pTime_sub = nullptr;
	}
	DMSG("FINISHED SETTING TIME");
}

Time::time_point_t Time::now()
{
	if(Time::use_simulated_time)
	{
		std::shared_lock lock(Time::tick_mutex);
		return Time::tick;
	}
	else
	{
		return std::chrono::system_clock::now();
	}
}

void Time::sleep_until(time_point_t tp)
{
	if(use_simulated_time)
	{
		auto sim_now = Time::now();
		while(sim_now < tp)
		{
			// Most often, the simulation will not be paused. Therefore, in the
			// general case, this loop is executed only once

			// When the simulation is paused, we do not care about performance.
			// The minimum amount of time that we need to sleep is the wall time
			// plus the remainder of the sleep time. This is the case if the
			// application is unpaused just as the sleep is entered. If the
			// application is not unpaused during this period, the simulated
			// time will not advance and the minimum sleep time will be the same
			// in the next loop
			std::this_thread::sleep_for(tp - sim_now);
			sim_now = Time::now();
		}
	}
	else
	{
		std::this_thread::sleep_until(tp);
	}
}

// =============================================================================
// Helper Class Implementation
// =============================================================================
// -----------------------------------------------------------------------------
// ClockSubscriber
// -----------------------------------------------------------------------------
Time::ClockSubscriber::ClockSubscriber() :
	SubscriberBase<TimeMsgPubSubType>(topics::simulation_time),
	info()
{ }

void Time::ClockSubscriber::onNewDataMessage
(
	eprosima::fastrtps::Subscriber *sub
)
{
	TimeMsg msg;
	if(sub->takeNextData(&msg, &info))
	{
		// read the simulation time
		Time::time_point_t tp;
		std::chrono::duration<double> seconds(msg.seconds());
		tp += seconds;
		{
			std::unique_lock lock(Time::tick_mutex);
			Time::tick = tp;
		}
	}
}
