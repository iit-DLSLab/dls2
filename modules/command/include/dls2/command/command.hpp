#ifndef COMMAND_HPP_RSTXNA3I
#define COMMAND_HPP_RSTXNA3I

// =============================================================================
// Includes
// =============================================================================

#include <string>
#include <functional>
#include <utility>
#include <vector>
#include <memory>

#include "dls2/command/command_base.hpp"
#include "dls2/util/messaging/dds_reader.hpp"
#include "dls2/domains/domains.hpp"

namespace dls
{
	// =============================================================================
	// Class Interface
	// =============================================================================
	/// Template class representing a local command
	///
	/// This class is used to register a command with the framework. Users should
	/// prefer using CommandManager instead of using Command directly
	template <typename...arg_ts>
	class Command : public CommandBase
	{
	friend class CommandManager;
    friend class CommandHelper;

	public:
		/// Constructor
		///
		/// @param name the name of the command as seen from the rest of the
		/// framework
		/// @param owner the component that registered this command
		/// @param docstring some documentation for the command
		/// @param f the function associated with the command. The function may take
		/// any number of any type of argument, and returns any type
		/// @param level execution level of the command
		/// @param enabled set if the command is enabled or not
		Command
		(
			std::string name,
			CommandManager *owner,
			std::string docstring,
			std::function<bool(arg_ts...)> f,
			LevelType level = {{0,0}},
			bool enabled = false
		);

		/// Destructor
		///
		~Command();

		/// Enable the command
		/// 
		void setEnabled();

		/// Disable the command
		///
		void setDisabled();

		/// Activate the command
		/// 
		void activate();

		/// Deactivate the command
		///
		void deactivate();


	private:
		// ============================= Data Members ==============================
		/// Callback of the command
		///
		const std::function<bool(arg_ts...)> f;

		/// Link the command with the rest of the framework
		///
		std::shared_ptr<dls::DDSReader> ddslink;

		/// Register command on command domain
		///
		void registerCommand();

		/// Unregister command on command domain
		///
		void unregisterCommand();

		/// Call method
		///
		bool call(std::vector<std::string>);

	};

	// ========================== class helpers ==========================
	template <typename... Args, std::size_t... Is>
	auto create_tuple_impl(std::index_sequence<Is...>, const std::vector<std::string>& arguments);

	template <typename... Args>
	auto create_tuple(const std::vector<std::string>& args);

    class CommandManager;

    class CommandHelper
    {
    public:

        static void changeCommandLevel(CommandManager *manager, int level);
        static int getCurrentLevel(CommandManager *manager);
    };

} // end namespace dls

#include "dls2/command/command.tpp"

#endif /* end of include guard: COMMAND_HPP_RSTXNA3I */
