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

// messaging
#include "dls2/util/messaging/publisher_base.hpp"
#include "dls2/msg/stringmsgPubSubTypes.h"
#include "dls2/msg/command_registerPubSubTypes.h"
#include "dls2/util/messaging/subscriber.hpp"

// commands
#include "dls2/command/command.hpp"

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
	ROSLayer(std::string ID);

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
	std::atomic_bool should_quit;

	/// Handler to the shutdown SIGINT (ctrl+c) signal
	/// 
	static void handle_signals(int);
	
};
} // end namespace dls

#endif /* end of include guard: ROS_LAYER_HPP */
