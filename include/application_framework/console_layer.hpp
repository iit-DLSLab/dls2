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
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef CONSOLE_LAYER_HPP_BXNRZS8Q
#define CONSOLE_LAYER_HPP_BXNRZS8Q
// =============================================================================
// Includes
// =============================================================================
// framework
#include "application_framework/app_layer.hpp"

// messaging
#include "util/messaging/publisher_base.hpp"
#include "msg/stringmsgPubSubTypes.h"

// stdlib
#include <map>
#include <functional>
#include <mutex>

// =============================================================================
// Class Interface
// =============================================================================
class ConsoleLayer : public AppLayer
{
	// Console completion is handled by readline, which is a C-library.
	// Therefore, need to declare this as a friend
	friend char *command_completion(const char *text, int state);
	friend char **console_completion(const char *text, int start, int end);
	friend char *arg_completion(const char * text, int state);

public:
	struct Command
	{
		Command
		(
			const std::string &command_name,
			const std::function<void(const std::vector<std::string>&)> &function,
			const std::string &docstring,
			const std::vector<std::string> &default_completions = {}
		);

		std::string                                    command_name;
		std::function<void(std::vector<std::string>)>  function;
		std::string                                    docstring;
		std::vector<std::string>                       default_completions;
	};

	ConsoleLayer();
	~ConsoleLayer() = default;

	Status run() override;
	Status shutdown() override;

	void addCommand(const Command&);

private:
	std::string build_prompt();

	// Begin critical section
		std::mutex commands_mutex;
		std::map<std::string, Command> commands;
	// End critical section
};

#endif /* end of include guard: CONSOLE_LAYER_HPP_BXNRZS8Q */
