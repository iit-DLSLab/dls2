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
#include "application_framework/console_layer.hpp"
#include <readline/readline.h>
#include <readline/history.h>
#include <cstring>
#include <sstream>
// #include "util/string/string.hpp"

#include "util/messaging/publisher_base.hpp"

//TODO these topics should be removed and instead generated dynamically
#include "msg/stringmsgPubSubTypes.h"
#include "topics/activate_controller.hpp"
#include "topics/deactivate_controller.hpp"
#include "topics/activate_gait_generator.hpp"
#include "topics/deactivate_gait_generator.hpp"

// TODO move these two to the library
std::string trim(const std::string&);
std::string &trim_inplace(std::string *const);
// =============================================================================
// Globals
// =============================================================================
// Indirection to access class methods as C functions
// This will only work if there is only one console object in the process, which
// will always be the case
ConsoleLayer *pInstance = nullptr;

// =============================================================================
// Foreward Declarations
// =============================================================================
char *console_completion(const char *text, int state);

PublisherBase<StringMsgPubSubType> activate_controller_pub(topics::activate_controller);

// =============================================================================
// Constructors
// =============================================================================
ConsoleLayer::ConsoleLayer() :
	commands_mutex(),
	commands()
{
	pInstance = this;

	// TODO expose process name here for readline
	rl_readline_name = "DLS2_READLINE";

	// rl_attempted_completion_function = console_completion;
	rl_completion_entry_function = console_completion;

	// populate commands
	addCommand
	(
		Command
		(
			"activateController",
			[](const std::vector<std::string> &vec)
			{
				std::cout << "Called function" << std::endl;
				StringMsg msg;
				if(vec.size() > 0)
				{
					std::cout << "arg is '" << vec[0] << "'" << std::endl;
					msg.msg(vec[0]);
					activate_controller_pub.publish(msg);
				}
			},
			"activates the controller"
		)
	);

	addCommand
	(
		Command
		(
			"deactivateController",
			[](const std::vector<std::string>&)
			{
				std::cout << "Called function" << std::endl;
			},
			"deactivates the controller"
		)
	);

	addCommand
	(
		Command
		(
			"activateGaitGenerator",
			[](const std::vector<std::string>&)
			{
				std::cout << "Called function" << std::endl;
			},
			"deactivates the controller"
		)
	);

	addCommand
	(
		Command
		(
			"deactivateGaitGenerator",
			[](const std::vector<std::string>&)
			{
				std::cout << "Called function" << std::endl;
			},
			"deactivates the controller"
		)
	);
}

// =============================================================================
// Interface Override
// =============================================================================
ConsoleLayer::Status ConsoleLayer::run()
{
	while(true)
	{
		// This calls malloc behind the scenes. Needs to be freed manually
		char *line = readline(this->build_prompt().c_str());

		if(line) // might be nullptr if EOF was sent
		{
			// Convert to cpp line for ease of use
			std::string input(line);
			trim_inplace(&input);

			if(*input.c_str())
			{
				// add_history(line);
				add_history(input.c_str());

				// Parse the arguments
				std::istringstream iss(input);
				std::vector<std::string> input_split
					(
						std::istream_iterator<std::string>{iss},
						std::istream_iterator<std::string>()
					);

				std::vector<std::string> args
					(
						input_split.begin() + 1, input_split.end()
					);

				// Execute the commands
				{
					std::lock_guard<std::mutex> lock(this->commands_mutex);

					auto it = pInstance->commands.find(input_split[0]);
					if(it != pInstance->commands.end())
					{
						it->second.function(args);
					}
				}
			}
			free(line);
		}
	}

	return getStatus();
}

ConsoleLayer::Status ConsoleLayer::shutdown()
{
	return getStatus();
}

// =============================================================================
// Implementaton
// =============================================================================
std::string ConsoleLayer::build_prompt()
{
	return "> ";
}

void ConsoleLayer::addCommand(const Command &c)
{
	std::lock_guard<std::mutex> lock(this->commands_mutex);
	this->commands.insert
	(
		std::pair<std::string, Command>
		(
			c.command_name,
			c
		)
	);
}

// =============================================================================
// Readline
// =============================================================================
char *console_completion(const char *text, int state)
{
	static decltype(pInstance->commands.cbegin()) it;
	static int string_length;

	// Safety check is not currently needed, but putting it now for future
	// robustness
	if(pInstance)
	{
		// if first time this completer is being run
		if(state == 0)
		{
			// Not using lock_guard, since lock must persist across all calls to
			// this generator
			pInstance->commands_mutex.lock();
			it = pInstance->commands.begin();
			string_length = std::strlen(text);
		}

		while(it != pInstance->commands.cend())
		{
			const char *match_candidate = it->second.command_name.c_str();
			++it;

			if(strncmp(match_candidate, text, string_length) == 0)
			{
				char *ret_str = static_cast<char *>
				(
					// malloc is required by readline
					// readline will free the string
					malloc
					(
						strlen(match_candidate) + 1
					)
				);
				strcpy(ret_str, match_candidate);
				return ret_str;
			}
		}

		pInstance->commands_mutex.unlock();
	}

	return nullptr;
}

// =============================================================================
// Helper Class
// =============================================================================
ConsoleLayer::Command::Command
(
	const std::string &command_name_,
	const std::function<void(const std::vector<std::string>&)> &function_,
	const std::string &docstring_,
	const std::vector<std::string> &default_completions_
) :
	command_name(command_name_),
	function(function_),
	docstring(docstring_),
	default_completions(default_completions_)
{}

std::string trim(const std::string &s)
{
	std::string ret(s);
	trim_inplace(&ret);
	return ret;
}

std::string &trim_inplace(std::string *const s)
{
	s->erase(0, s->find_first_not_of(" \t\n\r\f\v"));
	s->erase(s->find_last_not_of(" \t\n\r\f\v") + 1);

	return *s;
}
