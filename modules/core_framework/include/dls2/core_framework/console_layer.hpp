
#ifndef CONSOLE_LAYER_HPP
#define CONSOLE_LAYER_HPP

#include "dls2/application/layer.hpp"
#include <readline/readline.h>
#include <readline/history.h>

namespace dls
{
	class ConsoleLayer : public Layer
	{
	public:
		ConsoleLayer(std::string ID);
		virtual ~ConsoleLayer();

		// Functions for autocompletion with "Tab" key, implemented using the GNU Readline library
		static char **command_completion(const char *text, int start, int end);
		static char *command_generator(const char *text, int state);

		AppStatus run() override;
		AppStatus stop() override;
		std::string where() override;
	};
} // namespace dls

#endif