#ifndef PERIODIC_APP_HPP_RY9LWBZG
#define PERIODIC_APP_HPP_RY9LWBZG

#include "dls2/application/app.hpp"
#include <dls2/util/time/time.hpp>
#include "dls2/application/resource_monitor.hpp"

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
		/// Automatically calls the abstract run function at the correct frequency - used in state machine
		AppStatus run() override;

		/// Stops this component
		///
		void close() override;
		
		//! Get period
		period_t getPeriod();

		//! Run the activation function - used in state machine
		virtual bool deactivating() override;

		//! Run the deactivation function. It is used by the state machine with RT scheduling
		virtual bool deactivation(const std::chrono::system_clock::time_point&);
		
		/// Virtual run function
		///
		/// Overwrite this function with the function that needs to be called at the
		/// correct rate
		virtual void run(const std::chrono::system_clock::time_point&) = 0;

		//! Set SCHED_DEADLINE policy
		void setRTSchedulerPolicy();

		//! Pause the current execution
		void pauseExecution();

		//! Check whether the time factor has changed
		bool newTimeFactor();
		
		//! Check failure
		bool checkFailure();

		//! Check if the app is running in real time
		void checkRT();
		//! Check if a pause request was sent
		bool isPaused();
		
		//! Set the app in failure state
		void setFailure();

		double getDesiredFrequency() const;

		void childMonitor() override;

		double dt;

	protected:
        //! Config variable to load scheduler settings
		YAML::Node config_scheduler;

		//! The period of this component
		const period_t period;
		//! Runtime factor scaling the period to get the runtime
		double sched_runtime_factor;
		//! Deadline factor scaling the period to get the deadline
		double sched_deadline_factor;
		//! Runtime attribute
		const period_t runtime;
		//! Deadline attribute
		const period_t deadline;
		//! Variable identifying if the periodic app is in failure state
		bool failure;
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

		/// The component's time rate
		///
		Time time_factor;

		double cur_time_factor;

		pid_t pid;

		bool realtime_prec;
		bool realtime_curr;
		std::chrono::time_point<std::chrono::steady_clock> loop_time_prec;

		std::unique_ptr<ResourceMonitor> resource_monitor_;

		double current_frequency_;
		std::mutex frequency_mutex_;
	};
} // end namespace dls

#endif /* end of include guard: PERIODIC_APP_HPP_RY9LWBZG */
