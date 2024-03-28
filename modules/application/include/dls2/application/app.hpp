#ifndef APP_HPP
#define APP_HPP

#include "dls2/application/app_status.hpp"
#include "dls2/command/command_manager.hpp"
#include "dls2/log/log.hpp"


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

		/// Runs the app
		///
		virtual AppStatus run() = 0;

		/// Shutdown the app
		///
		virtual AppStatus stop() = 0;

		/// Prints the state of the app
		///
		virtual std::string where() = 0;

		/// Verify if the app should terminate
		///
		bool shouldQuit();

        /// Emergency stop
		///
		/// If a app does not override this function, it defaults to the app's
		/// stop function
		virtual AppStatus eStop();

		//! Idle activity - used in state machine
		virtual void idle();

		//! Procedure to activate the app - used in state machine
		virtual void activation();

		//! Procedure to deactivate the app  - used in state machine
		virtual void deactivation();
	
		//! Procedure to handle the failure - used in state machine
		virtual void failure();
	
		//! Procedure to quit - used in state machine
		virtual void quit();
		
		virtual void execute();

		/// Flag of the running loop
		/// Exits when set to true
		bool should_quit;

		/// Stores commands registered in the app
		///
		CommandManager command_manager;

		//! Log system events
		logging::clogstream scout_sys;

		//! Log warning messages
		logging::warnstream scout_warn;

		//! Log errors that occurred, but from which the system can recover. Also log possible future fatal errors for the operator's attention.
		logging::cerrstream scout_err;

		/// The ID of this app
		///
		const std::string ID_;
	private:
		// BEGIN critical section
		mutable std::mutex status_mutex;
		AppStatus status;
		// END critical section
	protected:
		std::string get_current_time();
	};
} // end namespace dls

#endif /* end of include guard: APP_HPP */
