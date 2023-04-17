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
#ifndef PERIODIC_APP_LAYER_COMPONENT_HPP_RY9LWBZG
#define PERIODIC_APP_LAYER_COMPONENT_HPP_RY9LWBZG
#include "dls2/components/app_layer_component.hpp"
#include "yaml-cpp/yaml.h"

#include <condition_variable>
#include <chrono>
#include <atomic>
#include <mutex>

#include <boost/process.hpp>

#define SCHED_DEADLINE       6
#define __NR_sched_setattr           314
#define __NR_sched_getattr           315

struct sched_attr {
	__u32 size;

	__u32 sched_policy;
	__u64 sched_flags;

	/* SCHED_NORMAL, SCHED_BATCH */
	__s32 sched_nice;

	/* SCHED_FIFO, SCHED_RR */
	__u32 sched_priority;

	/* SCHED_DEADLINE (nsec) */
	__u64 sched_runtime;
	__u64 sched_deadline;
	__u64 sched_period;
};

namespace dls
{
	/// Periodic component
	///
	/// This class automatically calls its own run function at a given period
	class PeriodicAppLayerComponent : public AppLayerComponent
	{
	public:

		typedef std::chrono::duration<double, std::ratio<1, 1'000'000>> period_t;

		/// Constructor
		///
		/// @param ID the name of this component
		PeriodicAppLayerComponent(const std::string &ID);

		virtual ~PeriodicAppLayerComponent() = default;

		/// Runs the component
		///
		/// Automatically calls the abstract run function at the correct frequency
		Status run() override;

		/// Stops this component
		///
		Status stop() override;

		/// Virtual run function
		///
		/// Overwrite this function with the function that needs to be called at the
		/// correct rate
		virtual void run(const std::chrono::system_clock::time_point&) = 0;

	protected:
        //! Config variable to load scheduler settings
		YAML::Node config_scheduler;

		/// The period of this component
		///
		const period_t period;

	private:
		/// Used to determine whether the periodic run function should be called
		///
		std::atomic_bool should_run;

		// BEGIN critical section
			/// mutex handling pausing and unpausing
			///
			std::mutex pause_mutex;

			/// Used to check whether execution should be paused (false until
			/// specified by the user)
			bool is_paused;

			/// Condition variable tracking whether a pause request was made or not
			///
			std::condition_variable pause_request;
		// END critical section

		//! Attrributes of the scheduler
		struct sched_attr scheduler_attributes;
	};
} // end namespace dls

#endif /* end of include guard: PERIODIC_APP_LAYER_COMPONENT_HPP_RY9LWBZG */
