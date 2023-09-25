
#ifndef CONSOLE_LAYER_HPP
#define CONSOLE_LAYER_HPP

#include "dls2/application/layer.hpp"
#include <readline/readline.h>
#include <readline/history.h>
#include <filesystem>

namespace dls
{
	class ConsoleLayer : public Layer
	{
		// Functions for autocompletion with "Tab" key, implemented using the GNU Readline library
		// Since these functions are part of GNU Readline library, they could be static (but in that case
		// they could not access DLS2 member variables), so we make them friend of dls::ConsoleLayer class
		// (we then need forward declaration and an additional ConsoleLayer object visible from them).

		// Generate possible matches corresponding to the partial input entered by the user.
		// "start" and "end" are indices in rl_line_buffer defining the boundaries of "text"
		friend char **command_completion(const char *text, int start, int end);

		// Takes the partial input entered by the user and a state as arguments.
		// The "state" is zero the first time the function is called for a partial input and non-zero otherwise.
		friend char *command_generator(const char *text, int state);

		friend char *load_layer_argument_generator(const char *text, int state);

		// Display the list of possible matches when "Tab" key is pressed.
		// "matches" is the array of matching strings, "num_matches" is the number of strings in that array and 
		// "max_length" is the length of the longest string in that array
		friend void display_matches(char **matches, int num_matches, int max_length);

	public:
		ConsoleLayer(std::string ID);
		virtual ~ConsoleLayer();

		// Return all the available commands sorted by name, or just the ones of specific processes
		const std::map<std::string, std::set<std::string>> getProcessesAndCommands(const std::set<std::string>& matches = std::set<std::string>({}));

		// Return the "load_layers_paths" map with "load[Layer]" commands and associated installation folder
		const std::map<std::string, std::string> getLoadLayersPaths();

		// "matches" can contain both processes and commands. Show:
		// - all the available commands [if the set is empty]
		// - all the available commands of a specific process [if the set element is a process]
		// - the process names with the command specified [if the set element is a command available under different processes]
		void printProcessesAndCommands(const std::set<std::string>& matches = std::set<std::string>({}));

		// Show the possible arguments for "load[Layer]" commands
		void printLoadArguments(const std::string& load_command);

		AppStatus run() override;
		AppStatus stop() override;
		std::string where() override;

	private:
		const std::map<std::string, std::string> load_layers_paths_;
	};
} // namespace dls

#endif