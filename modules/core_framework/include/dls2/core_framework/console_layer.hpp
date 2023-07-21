
#ifndef CONSOLE_LAYER_HPP_BXNRZS8Q
#define CONSOLE_LAYER_HPP_BXNRZS8Q

// framework core
#include "dls2/application/layer.hpp"

#include <signal.h>

namespace dls
{
	/// A console interface into the framework
	///
	class ConsoleLayer : public Layer
	{
		// Console completion is handled by readline, which is a C-library.
		// Therefore, need to declare this as a friend
		friend char *command_completion(const char *text, int state);
		friend char **console_completion(const char *text, int start, int end);
		friend char *arg_completion(const char * text, int state);

	public:
		/// Default Constructor
		///
		ConsoleLayer(std::string ID);

		/// Default Destructor
		///
		~ConsoleLayer();

		/// Run the console
		///
		/// This is a blocking call that enters an infinite loop
		AppStatus run() override;

		/// Stop the console
		///
		/// Will cause `run` to stop running
		AppStatus stop() override;

		/// Print the state of this layer
		///
		/// prints:
		/// * active controllers
		/// * active motion generator
		/// * published torque
		std::string where() override{return "not yet implemented";}

	private:
		/// Generates the console prompt
		///
		/// This is currently just a stub function, but it can be expanded apon in
		/// future
		std::string build_prompt();

		/// Handler to the shutdown SIGINT (ctrl+c) signal
		/// 
		static void handle_signals(int);
	};
} // end namespace dls

#endif /* end of include guard: CONSOLE_LAYER_HPP_BXNRZS8Q */
