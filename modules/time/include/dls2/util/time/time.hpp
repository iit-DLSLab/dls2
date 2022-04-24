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
#ifndef TIME_HPP_2XUSQ5WF
#define TIME_HPP_2XUSQ5WF

#include "dls2/util/messaging/dds_reader.hpp"

#include <chrono>
#include <memory>
// #include <mutex>
#include <map>

namespace dls
{

	/// Static utility time class
	///
	/// This class is a thin wrapper that provides a unified abstract interface into
	/// wall time for running on the robot, as well as simulated time
	class Time
	{
		using time_point_t = std::chrono::time_point
			<std::chrono::system_clock, std::chrono::duration<double>>;
	public:
		// note this function is not thread-safe and should only be set at library
		// initialisation time
		static void set_use_simulated_time(bool);
		static time_point_t now();
		static void sleep_until(time_point_t);
		static decltype(std::chrono::system_clock::now()) pause_start_time;

	private:
		static bool use_simulated_time;
		/// The offset between simulated time and real time
		///
		/// Keeps track of the time difference between real time and wall time
		static std::chrono::duration<double> time_offset;
		// static std::shared_ptr<SubscriberBase<TimeMsgPubSubType>> pTime_sub;
		static std::shared_ptr<DDSReader> pPause_sub;
		static bool simulation_paused;
	};

} // namespace dls

#endif /* end of include guard: TIME_HPP_2XUSQ5WF */
