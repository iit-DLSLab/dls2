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
#ifndef CONSOLE_LAYER_HPP_BXNRZS8Q
#define CONSOLE_LAYER_HPP_BXNRZS8Q
// =============================================================================
// Includes
// =============================================================================
// framework
#include "app_layer.hpp"

// stdlib
#include <signal.h>

// =============================================================================
// Class Interface
// =============================================================================
namespace dls
{
/// A console interface into the framework
///
class ConsoleLayer : public AppLayer
{
	// Console completion is handled by readline, which is a C-library.
	// Therefore, need to declare this as a friend
	friend char *command_completion(const char *text, int state);
	friend char **console_completion(const char *text, int start, int end);
	friend char *arg_completion(const char * text, int state);

public:
	/// Default Constructor
	///
	ConsoleLayer(std::string ID, bool *should_quit_);

	/// Default Destructor
	///
	~ConsoleLayer() = default;

	/// Run the console
	///
	/// This is a blocking call that enters an infinite loop
	Status run() override;

	/// Stop the console
	///
	/// Will cause `run` to stop running
	Status shutdown() override;

	/// Print the state of this layer
	///
	/// prints:
	/// * active controllers
	/// * active gait generator
	/// * published torque
	std::string where() override{return "not yet implemented";}

private:
	/// Generates the console prompt
	///
	/// This is currently just a stub function, but it can be expanded apon in
	/// future
	std::string build_prompt();

	/// User feedback subscriber
	///
	// version2::Subscriber<StringMsgPubSubType> consoleFeedback;

	/// Flag of the running loop
	/// Exits when set to true
	bool *should_quit;
	
	/// Handler to the shutdown SIGINT (ctrl+c) signal
	/// 
	static void handle_signals(int);
};
} // end namespace dls

#endif /* end of include guard: CONSOLE_LAYER_HPP_BXNRZS8Q */
