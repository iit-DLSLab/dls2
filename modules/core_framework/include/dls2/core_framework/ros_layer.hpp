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
// =============================================================================
// Includes
// =============================================================================
#include "app_layer.hpp"
#include <boost/process.hpp>

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{
	/// A ROS interface into the framework
	///
	class ROSLayer : public AppLayer
	{
	public:
		/// Default Constructor
		///
		ROSLayer(std::string ID, bool *should_quit_);

		/// Default Destructor
		///
		~ROSLayer() = default;

		/// Run the layer
		///
		Status run() override;

		/// Stop the layer
		///
		Status shutdown() override;

		/// Print the state of this layer
		///
		std::string where() override{return "not yet implemented";}

	private:
		/// Handler to the shutdown SIGINT (ctrl+c) signal
		/// 
		static void handle_signals(int);

		void launchSim();

		void launchCore();

		std::map<std::string, boost::process::child *> processes;
		
	};
} // end namespace dls

#endif /* end of include guard: ROS_LAYER_HPP */
