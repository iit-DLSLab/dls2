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

// TODO when we are using a proper modern compiler, use the filesystem library
// #include <filesystem>
// TODO when we are using a proper modern compiler, remove this header
#include <dirent.h>

// #include "util/string/string.hpp"

#include "util/messaging/publisher_base.hpp"

//TODO these topics should be removed and instead generated dynamically
#include "msg/stringmsgPubSubTypes.h"
#include "topics/activate_controller.hpp"
#include "topics/deactivate_controller.hpp"
#include "topics/activate_gait_generator.hpp"
#include "topics/deactivate_gait_generator.hpp"
#include "util/debug/debug.hpp"
#include "topics/warn_log_stream.hpp"
#include "util/log/log.hpp"
#include "topics/command_register.hpp"

// =============================================================================
// Foreward Declarations
// =============================================================================
namespace dls
{
	char **console_completion(const char *text, int start, int /*end*/);
	char *command_completion(const char *text, int state);
	char *arg_completion(const char *text, int state);
}

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls;

// TODO move these two to the library
std::string trim(const std::string&);
std::string &trim_inplace(std::string *const);
// =============================================================================
// Local Globals
// =============================================================================
// Indirection to access class methods as C functions
// This will only work if there is only one console object in the process, which
// will always be the case
ConsoleLayer *pInstance = nullptr;

// =============================================================================
// Constructors
// =============================================================================
ConsoleLayer::ConsoleLayer() :
	remote_command_manager
	(
		// [this](std::shared_ptr<const RemoteCommand> p)
		// {
		// 	std::lock_guard<std::mutex> lock(this->commands_mutex);
		// }
	),
	//  string_listener(*this),
	command_manager()
{
	pInstance = this;

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

	command_manager.addCommand<void, std::string>
	(
		"console_layer",
		"help",
		"prints docstring of a command",
		std::function<void(std::string)>
		(
			[&](std::string s)
			{
				for(auto pCommand : this->remote_command_manager.findByName(s))
				{
					std::cout << pCommand->owner << "::" <<
						pCommand->command_name << ": " <<
						pCommand->docstring << std::endl;

					// for now we only have unique commands, so break
					break;
				}
				std::cout << this->build_prompt() << std::endl;
			}
		)
	);

	command_manager.addCommand<double, double, double>
	(
		"console_layer",
		"addTwoDoubles",
		"adds two doubles",
		std::function<double(double,double)>
		{
			[](double a, double b) ->double
			{
				std::cout << "Console layer: " << a + b << std::endl;
				return a + b;
			}
		}
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
				// the format is [namespace::]command_name [arg1 [arg2...]]
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

					// extract the command name and its namespace
					// The format of the string is
					// [namespace::]command_name
					size_t namespace_delimiter_index = input_split[0].find("::");

					// if the command is of the form namepsace::command_name
					if(namespace_delimiter_index != std::string::npos)
					{
						std::string namespace_name =
							input_split[0].substr(0, namespace_delimiter_index);
						std::string command_name =
							input_split[0].substr(namespace_delimiter_index + 2);

						// find the command
						auto pCommand = pInstance->remote_command_manager
							.find(namespace_name, command_name);

						// call the command
						if(pCommand)
						{
							callCommand(*pCommand, args);
						}
						else
						{
							std::cerr << "Command '" << command_name
								<< "' in namespace '" << namespace_name
								<< " not found" << std::endl;
							build_prompt();
						}
					}

					// if the command is of the form command_name
					else
					{
						std::vector<std::shared_ptr<const RemoteCommand>> remote_commands =
							pInstance->remote_command_manager.findByName(input_split[0]);

						if(remote_commands.size() == 1)
						{
							callCommand(*remote_commands[0], args);
						}
						else if(remote_commands.size() == 0)
						{
							std::cerr << "Command '" << input_split[0] <<
								"' not found" << std::endl;
							build_prompt();
						}
						else
						{
							std::cout << "error: ambiguous overload. Command '"
								<< input_split[0] << "' found in these namespaces:\n";
							for(const auto &el : remote_commands)
							{
								std::cout << "\t" << el->owner << "\n";
							}
							std::cout << std::endl;
							build_prompt();
						}
					}


					// std::vector<std::shared_ptr<const RemoteCommand>> remote_commands =
					// 	pInstance->remote_command_manager
					// 		.getCurrentlyRegisteredCommands();

					// auto it = remote_commands.begin();
					// for(;it != remote_commands.end(); ++it)
					// {
					// 	if((*it)->command_name == input_split[0])
					// 	{
					// 		break;
					// 	}
					// }
					// if(it != remote_commands.end())
					// {
					// 	callCommand(**it, args);
					// }
					// else
					// {
					// 	std::cerr << "Command '" << input_split[0]
					// 		<< "' not found" << std::endl;
					// }
				}
			}
			free(line);
		}
		else // if EOF received
		{
			std::cout << std::endl;
		}
	}

	return getStatus();
}

ConsoleLayer::Status ConsoleLayer::shutdown()
{
	return getStatus();
}

// void ConsoleLayer::onNewDataMessage(eprosima::fastrtps::Subscriber *sub)
// {
// 	StringMsg msg;
// 	if(sub->takeNextData(&msg, nullptr))
// 	{
// 		std::cout << "\n" << msg.msg() << std::flush;
// 		std::cout << build_prompt() << " " << rl_line_buffer << std::flush;
// 	}
// }

// =============================================================================
// Implementaton
// =============================================================================
std::string ConsoleLayer::build_prompt()
{
	return "> ";
}

void ConsoleLayer::callCommand
(
	const RemoteCommand &command, const std::vector<std::string> &args
)
{
	// Ensure args are correct size
	if(args.size() != command.args.size())
	{
		std::cout << "Error: incorrect number of arguments" << std::endl;
		build_prompt();
		return;
	}

	// push the args
	for(size_t i = 0; i != args.size(); ++ i)
	{
		std::stringstream ss(args[i]);
		switch(static_cast<CommandBase::ArgumentType>(command.args[i]))
		{
			case CommandBase::ArgumentType::CHAR:
			{
				char val;
				ss >> val;
				command.pushArg<char>(val);
				break;
			}
			case CommandBase::ArgumentType::UINT8:
			{
				uint8_t val;
				ss >> val;
				command.pushArg<uint8_t>(val);
				break;
			}
			case CommandBase::ArgumentType::INT16:
			{
				int16_t val;
				ss >> val;
				command.pushArg<int16_t>(val);
				break;
			}
			case CommandBase::ArgumentType::UINT16:
			{
				uint16_t val;
				ss >> val;
				command.pushArg<uint16_t>(val);
				break;
			}
			case CommandBase::ArgumentType::INT32:
			{
				int32_t val;
				ss >> val;
				command.pushArg<int32_t>(val);
				break;
			}
			case CommandBase::ArgumentType::UINT32:
			{
				uint32_t val;
				ss >> val;
				command.pushArg<uint32_t>(val);
				break;
			}
			case CommandBase::ArgumentType::INT64:
			{
				int64_t val;
				ss >> val;
				command.pushArg<int64_t>(val);
				break;
			}
			case CommandBase::ArgumentType::FLOAT:
			{
				float val;
				ss >> val;
				command.pushArg<float>(val);
				break;
			}
			case CommandBase::ArgumentType::DOUBLE:
			{
				double val;
				ss >> val;
				command.pushArg<double>(val);
				break;
			}
			case CommandBase::ArgumentType::LONG_DOUBLE:
			{
				long double val;
				ss >> val;
				command.pushArg<long double>(val);
				break;
			}
			case CommandBase::ArgumentType::BOOL:
			{
				bool val;
				ss >> val;
				command.pushArg<bool>(val);
				break;
			}
			case CommandBase::ArgumentType::STD_STRING:
			{
				command.pushArg<std::string>(ss.str());
				break;
			}
			default:
			{
				std::cout << "ERROR: unsupported argument type: " <<
					command.args[i] << std::endl;
				std::cout << "STD_STRING: " << static_cast<uint32_t>(CommandBase::ArgumentType::STD_STRING) << std::endl;
				return;
			}
		}
	}

	// call the command
	command.call();
}

// =============================================================================
// Readline
// =============================================================================
//// This namespace contains global variables that should only be used by
//// readline.
////
//// Readline does not provide for passing userdata, therefore it is not possible
//// to get around using some global structures
//namespace readline_globals
//{
//	std::map<std::string, ConsoleLayer::Command>::iterator current_command;
//}

// -----------------------------------------------------------------------------
// Completion chooser
// -----------------------------------------------------------------------------
char **dls::console_completion(const char *text, int start, int /*end*/)
{
	char **matches = nullptr;

	if(start == 0)
	{
		matches = rl_completion_matches(text, command_completion);
	}
	else
	{
		// Find the command name from the entire command line
		std::stringstream ss(rl_line_buffer);
		std::string command_name;
		ss >> command_name;

		// Find the command structure associated with the command name
		//
		// It may not exist if garbage was typed into the command line
		{
			// std::lock_guard<std::mutex> lock(pInstance->commands_mutex);

			std::vector<std::shared_ptr<const RemoteCommand>> remote_commands =
				pInstance->remote_command_manager.getCurrentlyRegisteredCommands();

			auto it = remote_commands.begin();
			for(; it != remote_commands.end(); ++it)
			{
				if((*it)->command_name == command_name)
					break;
			}

			if(it != remote_commands.end())
			{
				matches = rl_completion_matches(text, arg_completion);
			}
		}

	}

	return matches;
}

// -----------------------------------------------------------------------------
// Command Completion
// -----------------------------------------------------------------------------
char *dls::command_completion(const char *text, int state)
{
	static decltype(pInstance->remote_command_manager.getCurrentlyRegisteredCommands()) commands;
	static decltype(pInstance->remote_command_manager.getCurrentlyRegisteredCommands().cbegin()) it_commands;
	static decltype(pInstance->remote_command_manager.getCurrentlyRegisteredOwners()) owners;
	static decltype(pInstance->remote_command_manager.getCurrentlyRegisteredOwners().cbegin()) it_owners;
	static int string_length;

	// Safety check is not currently needed, but putting it now for future
	// robustness
	if(pInstance)
	{
		// if first time this completer is being run
		if(state == 0)
		{
			commands = pInstance->remote_command_manager.getCurrentlyRegisteredCommands();
			owners = pInstance->remote_command_manager.getCurrentlyRegisteredOwners();
			// pInstance->commands_mutex.lock();
			// it = pInstance->commands.begin();
			it_commands = commands.begin();
			it_owners = owners.begin();
			string_length = std::strlen(text);
		}

		// Do command completion
		while(it_commands != commands.cend())
		{
			const char *match_candidate = (*it_commands)->command_name.c_str();
			++it_commands;

			// TODO move this into a function, since it is copied in the other
			// completers
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
		// Do owner completion
		while(it_owners != owners.cend())
		{
			const char *match_candidate = it_owners->c_str();
			++it_owners;

			// TODO move this into a function, since it is copied in the other
			// completers
			if(strncmp(match_candidate, text, string_length) == 0)
			{
				char *ret_str = static_cast<char *>
				(
					// malloc is required by readline
					// readline will free the string
					malloc
					(
						strlen(match_candidate) + 3 // 2 for '::', 1 for '\0'
					)
				);
				strcpy
				(
					ret_str,
					(
						std::string
						(
							match_candidate
						) + "::"
					).c_str()
				);
				return ret_str;
			}
		}

		// pInstance->commands_mutex.unlock();
	}

	return nullptr;
}

// -----------------------------------------------------------------------------
// Argument Completion
// -----------------------------------------------------------------------------
char *dls::arg_completion(const char * text, int state)
{
	static std::vector<std::string> files;
	static size_t index;

	if(state == 0)
	{
		index = 0;
		files.clear();

		// fill the default completions
		// for
		// (
		// 	const auto &el :
		// 	readline_globals::current_command->second.default_completions
		// )
		// {
		// 	files.push_back(el);
		// }


		// TODO use this code when we are using a proper C++17 compiler
		// fill libraries in the current path
		// std::filesystem::directory_iterator dir_it
		// 	(
		// 		std::filesystem::current_path()
		// 	);

		// for(auto &file : dir_it)
		// {
		// 	std::string filename(file.path().filename());
		// 	if
		// 	(
		// 		filename.find_first_of("lib") == 0 &&
		// 		filename.find_last_of(".so") == filename.size() - 1
		// 	)
		// 	{
		// 		files.push_back
		// 		(
		// 			// strip "lib" and ".so" from file
		// 			filename.substr(3, filename.size() - 6)
		// 		);
		// 	}
		// ADD UP TO HERE WHEN USING A PROPER C++17 COMPILER

		// TODO remove this code when we are using a proper C++17 compiler
		DIR *dp = nullptr;
		struct dirent *ep = nullptr;
		dp = opendir("./");
		if(dp != nullptr)
		{
			while( (ep = readdir(dp)) )
			{
				std::string filename = ep->d_name;
				if(filename == "." || filename == "..")
				{
					continue;
				}
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
			(void)closedir(dp);
		}
		// REMOVE UP TO HERE WHEN USING A PROPER C++17 compiler

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
// ConsoleLayer::Command::Command
// (
// 	const std::string &command_name_,
// 	const std::function<void(const std::vector<std::string>&)> &function_,
// 	const std::string &docstring_
// 	// const std::vector<std::string> &default_completions_
// ) :
// 	command_name(command_name_),
// 	function(function_),
// 	docstring(docstring_)
// 	// default_completions(default_completions_)
// {}

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

// =============================================================================
// Subscribers
// =============================================================================
// -----------------------------------------------------------------------------
// Strings
// -----------------------------------------------------------------------------
ConsoleLayer::StringListener::StringListener(ConsoleLayer &owner_) :
	SubscriberBase<StringMsgPubSubType>(topics::warn_log_stream),
	owner(owner_)
{ }

void ConsoleLayer::StringListener::onNewDataMessage(eprosima::fastrtps::Subscriber *sub)
// void ConsoleLayer::StringListener::onNewDataMessage(eprosima::fastrtps::Subscriber *)
{
	eprosima::fastrtps::SampleInfo_t info;
	StringMsg msg;
	std::cout << "hit string callback" << std::endl;
	if(sub->takeNextData(&msg, &info))
	{
		std::cout << "\n" << msg.msg() << std::flush;
		std::cout << owner.build_prompt() << " " << rl_line_buffer << std::flush;
	}
}
