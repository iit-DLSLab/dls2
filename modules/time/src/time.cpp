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
#include "dls2/util/time/time.hpp"
#include "dls2/topics/topics.hpp"

using namespace dls;

bool Time::use_simulated_time = false;
std::chrono::duration<double> Time::time_offset(0);
std::shared_ptr<DDSReader> Time::pPause_sub = nullptr;
bool Time::simulation_paused(false);
decltype(std::chrono::system_clock::now()) Time::pause_start_time;

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------
void Time::set_use_simulated_time(bool b)
{
	Time::use_simulated_time = b;
	if(b)
	{
		if(Time::pPause_sub == nullptr)
		{
			Time::pPause_sub = std::make_shared<DDSReader>(
				"Time::timeSource",
				dls::domains::control,
				dls::topics::simulation_pause,
				std::function<void(void *)>
				{
					[&](void *tuple)
					{	
						BoolMsg msg = *((BoolMsg *)tuple);
						
						// std::cout << "Got pause bool: " << msg.val() << std::endl;
						if(msg.val()) // if paused
						{
							Time::pause_start_time = std::chrono::system_clock::now();
						}
						else
						{
							Time::time_offset += std::chrono::system_clock::now() - Time::pause_start_time;
						}
						Time::simulation_paused = msg.val();
						// DMSG("Real time: " << std::chrono::system_clock::now().time_since_epoch().count() << " simulation time: " << Time::now().time_since_epoch().count());
					}
				}
			);
		}
	}
	else
	{
		// Time::pTime_sub = nullptr;
		Time::pPause_sub = nullptr;
	}
}

Time::time_point_t Time::now()
{
	if(Time::use_simulated_time)
	{
		if(Time::simulation_paused)
		{
			return pause_start_time - Time::time_offset;
		}
		else
		{
			return std::chrono::system_clock::now() - Time::time_offset;
		}
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
