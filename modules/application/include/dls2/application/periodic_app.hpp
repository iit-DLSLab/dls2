#ifndef PERIODIC_APP_HPP_RY9LWBZG
#define PERIODIC_APP_HPP_RY9LWBZG

#include "dls2/application/app.hpp"

#include <dls2/application/sched_attr.hpp>
#include <dls2/util/time/time.hpp>

#include <boost/process.hpp>
#include <yaml-cpp/yaml.h>

namespace dls
{
	/// Periodic component
	///
	/// This class automatically calls its own run function at a given period
	class PeriodicApp : public App
	{
	public:

		typedef std::chrono::duration<double, std::ratio<1, 1'000'000>> period_t;

		/// Constructor
		///
		/// @param ID the name of this component
		PeriodicApp(const std::string &ID);

		virtual ~PeriodicApp() = default;

		/// Runs the component
		///
		/// Automatically calls the abstract run function at the correct frequency
		AppStatus run() override;

		/// Stops this component
		///
		AppStatus stop() override;

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

		/// The component's time rate
		///
		Time time_factor;

		double cur_time_factor;

		pid_t pid;

		double sched_runtime_factor;
		double sched_deadline_factor;
	};
} // end namespace dls

#endif /* end of include guard: PERIODIC_APP_HPP_RY9LWBZG */
