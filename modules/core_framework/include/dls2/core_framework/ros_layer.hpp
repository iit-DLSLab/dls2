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
// framework
#include "app_layer.hpp"

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
	/// Flag of the running loop
	/// Exits when set to true
	bool *should_quit;

	pid_t pid;

	/// Handler to the shutdown SIGINT (ctrl+c) signal
	/// 
	static void handle_signals(int);

	void launchSim();

	void killRoscore();
	
};
} // end namespace dls

#endif /* end of include guard: ROS_LAYER_HPP */
