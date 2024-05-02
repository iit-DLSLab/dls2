
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
		// Functions for autocompletion with "Tab" key, implemented using the GNU Readline library.
		// Since these functions are part of the GNU Readline library, they could be static (but in that case
		// they could not access DLS2 member variables), so we make them friend of "dls::ConsoleLayer" class
		// (we then need forward declaration and a "dls::readline_completion::ConsoleLayer" object visible from them).

		// Generate possible matches corresponding to the partial input entered by the user.
		// "start" and "end" are indices in "rl_line_buffer" defining the boundaries of "text"
		friend char **command_completion(const char *text, int start, int end);

		// Take the partial input entered by the user and a state as arguments.
		// The "state" is zero the first time the function is called for a partial input and non-zero otherwise.
		// It generates matches for commands in the form "process::command"
		friend char *command_generator(const char *text, int state);

		// Same logic of the "command_generator" function, but it generates matches for the "load[Layer]" arguments
		friend char *load_layer_argument_generator(const char *text, int state);

		// Print the list of possible matches when "Tab" key is pressed.
		// "matches" is the array of matching strings, "num_matches" is the number of strings in that array and 
		// "max_length" is the length of the longest string in that array
		friend void display_matches(char **matches, int num_matches, int max_length);

	public:
		ConsoleLayer(std::string ID);
		virtual ~ConsoleLayer();

		// Return the complete set of available processes and commands sorted by name, or just the processes associated to "command"
		const std::map<std::string, std::set<std::string>> getProcessesAndCommands(const std::string& command = std::string());

		// Return the "load_layers_paths_" class variable
		const std::map<std::string, std::string> getLoadLayersPaths();

		// Print the processes and their commands according to the values of "matches" (the matched processes and commands) and "command"
		void printProcessesAndCommands(const std::string& command = std::string());

		void close() override;
		std::string where() override;

		void monitor() override;

		// needed to unblock the console from the readline
		void stop() override;

	private:
		// Map with "load[Layer]" commands and associated installation folders
		const std::map<std::string, std::string> load_layers_paths_;
	};
} // namespace dls

#endif