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
#include <filesystem>
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
char **console_completion(const char *text, int start, int /*end*/);
// char *console_completion(const char *text, int state);
char *command_completion(const char *text, int state);
char *arg_completion(const char *text, int state);

// char *dummy(const char

// TODO these should not be hardcoded
std::shared_ptr<PublisherBase<StringMsgPubSubType>> pActivate_controller_pub;
std::shared_ptr<PublisherBase<StringMsgPubSubType>> pDeactivate_controller_pub;
std::shared_ptr<PublisherBase<StringMsgPubSubType>> pActivate_gait_generator_pub;
std::shared_ptr<PublisherBase<StringMsgPubSubType>> pDeactivate_gait_generator_pub;

// =============================================================================
// Constructors
// =============================================================================
ConsoleLayer::ConsoleLayer() :
	commands_mutex(),
	commands()
{
	pInstance = this;

	// TODO remove these
	pActivate_controller_pub = std::make_shared<PublisherBase<StringMsgPubSubType>>
		(
			topics::activate_controller
		);
	pDeactivate_controller_pub = std::make_shared<PublisherBase<StringMsgPubSubType>>
		(
			topics::deactivate_controller
		);
	pActivate_gait_generator_pub = std::make_shared<PublisherBase<StringMsgPubSubType>>
		(
			topics::activate_gait_generator
		);
	pDeactivate_gait_generator_pub = std::make_shared<PublisherBase<StringMsgPubSubType>>
		(
			topics::deactivate_gait_generator
		);

	// TODO expose process name here for readline
	rl_readline_name = "DLS2_READLINE";

	rl_attempted_completion_function = console_completion;

	// Turn off the default file matching completion
	//
	// rl_completion_entry_function is NULL by default, but if it is null, then
	// readline will call rl_filename_completion_function by default.
	// We cannot simply use rl_completion_entry_function, since we need the more
	// flexible rl_attempted_completion_function
	rl_completion_entry_function = [](const char *, int)->char*{return nullptr;};

	// populate commands
	addCommand
	(
		Command
		(
			"activateController",
			// TODO lambda is currently copied everywhere, but this will
			// diseappear when everything is made more flexible
			[](const std::vector<std::string> &vec)
			{
				StringMsg msg;
				if(vec.size() > 0)
				{
					msg.msg(vec[0]);
					pActivate_controller_pub->publish(msg);
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
			[](const std::vector<std::string> &vec)
			{
				StringMsg msg;
				if(vec.size() > 0)
				{
					msg.msg(vec[0]);
					pDeactivate_controller_pub->publish(msg);
				}
			},
			"deactivates the controller"
		)
	);

	addCommand
	(
		Command
		(
			"activateGaitGenerator",
			[](const std::vector<std::string> &vec)
			{
				StringMsg msg;
				if(vec.size() > 0)
				{
					msg.msg(vec[0]);
					pActivate_gait_generator_pub->publish(msg);
				}
			},
			"deactivates the controller"
		)
	);

	addCommand
	(
		Command
		(
			"deactivateGaitGenerator",
			[](const std::vector<std::string> &)
			{
				StringMsg msg;
				pDeactivate_gait_generator_pub->publish(msg);
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
// -----------------------------------------------------------------------------
// Completion chooser
// -----------------------------------------------------------------------------
char **console_completion(const char *text, int start, int /*end*/)
{
	char **matches = nullptr;
	if(start == 0)
	{
		matches = rl_completion_matches(text, command_completion);
	}
	else
	{
		matches = rl_completion_matches(text, arg_completion);
	}

	return matches;
}

// -----------------------------------------------------------------------------
// Command Completion
// -----------------------------------------------------------------------------
char *command_completion(const char *text, int state)
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

		// Do command completion
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

// -----------------------------------------------------------------------------
// Argument Completion
// -----------------------------------------------------------------------------
char *arg_completion(const char * text, int state)
{
	static std::vector<std::string> files;
	// static decltype(files.cbegin()) it;
	static size_t index;

	if(state == 0)
	{
		// it = files.cbegin();
		index = 0;
		files.clear();
		std::filesystem::directory_iterator dir_it
			(
				std::filesystem::current_path()
			);

		// TODO add support for locating installed libs
		for(auto &file : dir_it)
		{
			std::string filename(file.path().filename());
			if
			(
				filename.find_first_of("lib") == 0 &&
				filename.find_last_of(".so") == filename.size() - 1
			)
			{
				files.push_back
				(
					// strip "lib" and ".so" from file
					filename.substr(3, filename.size() - 6)
				);
			}
		}
	}

	while(index != files.size())
	{
		auto match_candidate = const_cast<char *>(files[index].c_str());
		++index;
		if(strncmp(match_candidate, text, std::strlen(text)) == 0)
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

