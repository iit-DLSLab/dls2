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
#ifndef APP_HPP
#define APP_HPP

#include "dls2/core/app_status.hpp"
#include "dls2/command/command_manager.hpp"
#include "dls2/log/log.hpp"


namespace dls
{
	/// An application
	class App
	{
	public:

		/// Constructor
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
		Status getStatus() const;

		/// Set the status of the app
		///
		/// @param status the status
		void setStatus(Status status);

		/// Runs the app
		///
		virtual Status run() = 0;

		/// Shutdown the app
		///
		virtual Status stop() = 0;

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
		virtual Status eStop();

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

	private:
		/// The ID of this app
		///
		const std::string ID;

		// BEGIN critical section
			mutable std::mutex status_mutex;
			Status status;
		// END critical section

	};
} // end namespace dls

#endif /* end of include guard: APP_HPP */
