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
#ifndef ROS_LAYER_HPP
#define ROS_LAYER_HPP

#include <dls2/simulators/simulator_base.hpp>
#include <boost/process.hpp>

namespace dls
{
	/// A ROS interface into the framework
	///
	class ROSSim : public SimulatorBase
	{
	public:
		/// Default Constructor
		///
		ROSSim(std::string ID);

		/// Default Destructor
		///
		~ROSSim(){};

		/// Print the state of this layer
		///
		std::string where() override {return "ROS simulator";}

    private:
        /// Launch Simulator
        ///
		void launchSim() override;

        /// Exits Simulator
        ///
		void exitSim() override;

        /// Launch ROS Core
        ///
		void launchCore();

        /// Exits ROS Core
        ///
		void exitCore();

        /// Handler to the shutdown SIGINT (ctrl+c) signal
		/// 
		static void handle_signals(int);

        /// List of processes
        ///
		std::map<std::string, boost::process::child *> processes;
	};

    // the types of the class factories
    typedef ROSSim* create_t();
    typedef void destroy_t(ROSSim*);
} // end namespace dls

#endif /* end of include guard: ROS_LAYER_HPP */
