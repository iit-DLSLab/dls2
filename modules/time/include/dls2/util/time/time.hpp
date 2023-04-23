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

#include "dls2/util/messaging/dds_participant.hpp"

#include <chrono>
#include <memory>
#include <map>

namespace dls
{
	/// Static utility time class
	/// This class is a thin wrapper that provides a unified time for the framework
	///
	class Time
	{
	public:
		Time();
		~Time();

		double getRealTimeFactor();
		void setRealTimeFactor(double);

	private:
		
		/// The offset between simulated time and real time
		/// Keeps track of the time difference between real time and wall time
		///
		double time_factor;
		
		DDSParticipant timeLink;
		
	};

} // namespace dls

#endif /* end of include guard: TIME_HPP_2XUSQ5WF */
