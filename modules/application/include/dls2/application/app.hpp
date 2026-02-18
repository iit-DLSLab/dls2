#ifndef APP_HPP
#define APP_HPP

#include "dls2/application/app_status.hpp"
#include "dls2/command/command_manager.hpp"
#include "dls2/log/log.hpp"
#include <future>
#include <thread>
#include <condition_variable>
#include "dls2/application/state_machine/app_state_machine.hpp"
#include <dls2/application/sched_attr.hpp>
#include <dls_messages/dds/ProcessStatusLight.hpp>
#include <dls2/util/config/safety_layer.hpp>
#include "dls2/util/time/duration_utils.hpp"

namespace dls
{
	/// An application
	class App
	{
	public:
		typedef App *create_t(const std::string&);
		typedef void destroy_t(App*);

		/// Constructor
		/// @parm ID the name of this app
		App(const std::string &ID);

		/// Destructor
		virtual ~App();

		/// Returns the ID of the app
		///
		/// @ret the ID
		std::string getID();

		/// Get the status of the app
		///
		/// @ret the app's status
		AppStatus getStatus() const;

		/// Set the status of the app
		///
		/// @param status the status
		void setStatus(AppStatus status);

	void startMonitoring();
		void monitorApp();

		//! Procedure to quit the app
		virtual void close();

		/// Prints the state of the app
		///
		virtual std::string where();

		/// Shutdown the app
		///
		virtual void stop();

        /// Emergency stop
		///
		/// If a app does not override this function, it defaults to the app's
		/// stop function
		virtual AppStatus eStop();

		//! Idle activity - used in state machine
		virtual void idle();

		//! Procedure to activate the app - used in state machine
		virtual void activation();

		/// Run activity - used in state machine
		virtual AppStatus run() = 0;

		//! Procedure to deactivate the app  - used in state machine
		virtual void deactivation();
	
		//! Procedure to handle the failure - used in state machine
		virtual void fail();
	
		//! Procedure to quit - used in state machine
		virtual void quit();
		
		//! Execute the state machine starting from the initial state
		virtual void execute();

		//! Check if the activation can be performed
		virtual bool checkActivation();

		//! Procedure to deactivate the app, customizable
		virtual bool deactivating();

		/// Flag for quitting, additional threads exiting when set to true
		bool should_quit;
		std::condition_variable quit_cv;
		bool can_die{ false };
		std::mutex quit_mutex;

		/// Stores commands registered in the app
		///
		CommandManager command_manager;

		//! Log system events
		logging::clogstream scout_sys;

		//! Log warning messages
		logging::warnstream scout_warn;

		//! Log errors that occurred, but from which the system can recover. Also log possible future fatal errors for the operator's attention.
		logging::cerrstream scout_err;

		logging::RobustEventNotifier robust_event_notifier;

		DDSWriter status_notifier;
		dls2_interface::msg::ProcessStatus status_msg;

		/// The ID of this app
		///
		const std::string ID_;

		// Appliacation state machine
		state_machine::app::AppStateMachine sm;

		std::stringstream activation_message;

	private:
		// BEGIN critical section
		mutable std::mutex status_mutex;
		AppStatus status;
		// END critical section

	std::thread monitor_thread_;
	std::atomic<bool> monitoring_started_{false};

	protected:
		//! Set SCHED_OTHER policy
		void setDefaultSchedulerPolicy();

		//! Attrributes of the scheduler
		struct sched_attr scheduler_attributes;

		std::string get_current_time();

		virtual void childMonitor() {};

		std::shared_ptr<SafetyLayerConfig> safety_layer_config_;
	};
} // end namespace dls

#endif /* end of include guard: APP_HPP */
