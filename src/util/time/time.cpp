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
std::shared_ptr
	<
		std::multimap
		<
			Time::time_point_t,
			std::shared_ptr<Time::SleepData>
		>
	> Time::pSleep_datas = nullptr;

std::mutex Time::sleep_data_mutex;

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

		if(Time::pSleep_datas == nullptr)
		{
			pSleep_datas = std::make_shared
				<
					std::multimap
					<
						time_point_t,
						std::shared_ptr<SleepData>
					>
				>
			();
		}
	}
	else
	{
		Time::pTime_sub = nullptr;
		Time::pSleep_datas = nullptr;
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
		std::shared_ptr<SleepData> pSleep_data = std::make_shared<SleepData>();

		// lock this mutex now, so that when the sleep data is registered, there
		// exists no chance that it is woken and removed before waiting on the
		// condition variable
		std::unique_lock<std::mutex> lock(pSleep_data->mutex);

		// Register to the Time module that this thread is going to sleep until
		// the simulated time has been reached
		{
			std::lock_guard<std::mutex> lock(Time::sleep_data_mutex);
			Time::pSleep_datas->insert
				(
					std::pair
					<
						time_point_t,
						std::shared_ptr<SleepData>
					>
					(
						tp,
						pSleep_data
					)
				);
		}

		// Wait until woken up by the simulated time
		pSleep_data->condition_variable.wait
			(
				lock,
				[pSleep_data]{return pSleep_data->should_wake;}
			);
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
		// wake any sleeping threads waiting on simulation time
		{
			// std::lock_guard<std::mutex> lock(Time::sleep_data_mutex);
			auto it = Time::pSleep_datas->begin();
			for(; it != Time::pSleep_datas->end() && it->first <= tp; ++it)
			{
				std::lock_guard<std::mutex> lock(it->second->mutex);
				it->second->should_wake = true;
				it->second->condition_variable.notify_all();
			}
			// if(it != Time::pSleep_datas->begin())
			// {
			// 	Time::pSleep_datas->erase(Time::pSleep_datas->begin(), it);
			// }
		}
	}
}
// -----------------------------------------------------------------------------
// SleepData
// -----------------------------------------------------------------------------
Time::SleepData::SleepData() :
	mutex(),
	condition_variable(),
	should_wake(false)
{ }
