#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

#include "dls2/util/messaging/dds_writer.hpp"
#include "dls2/command/command_base.hpp"
#include "dls2/topics/topics.hpp"

#include <string>
#include <utility>
#include <vector>
#include <condition_variable>
#include <atomic>
#include <thread>

namespace dls
{
	/// A class that manages commands
	///
	/// This class updates its internal representations as commands advertise
	/// themselves and remove themselves from the framework
	class CommandManager
	{
	public:
		/// Constructor
		///
		CommandManager(std::string owner_);

		/// Destructor
		///
		~CommandManager();

		/// Find commands by the owner
		///
		std::multimap<std::string, std::string> findByOwner(std::string owner);

		/// Find commands by the name
		///
		std::multimap<std::string, std::string> findByName(std::string name);

		/// Find command by the pair {owner, name}
		///
		std::multimap<std::string, std::string> find(std::string owner, std::string name);

		/// Get list of all enabled commands in distrubuted framework
		///
		std::multimap<std::string, std::string> getCommandsList();

		/// Get a list of the unique owners of the commands
		///
		std::set<std::string> getOwnersList();

		/// Adds a command to the CommandManager
		///
		/// @param name command name as seen by the rest of the framework
		/// @param doc some documentation for the command
		/// @param f the function encapsulated by the command
		/// @param level execution level of the command
		/// @param enabled set command enabled state
		template <typename... arg_ts>
		void addCommand
		(
			std::string name,
			std::string doc,
			const std::function<bool(arg_ts...)> &f,
			dls::CommandBase::LevelType level = {},
			bool enabled = false
		);

		/// Adds a command to the CommandManager
		///
		/// @param name command name as seen by the rest of the framework
		/// @param doc some documentation for the command
		/// @param f the function encapsulated by the command
		/// @param level execution level of the command
		/// @param enabled set command enabled state
		template <class T>
		void addCommand
		(
			std::string name,
			std::string doc,
			bool(T::*function_ptr)(),
			T* obj,
			dls::CommandBase::LevelType level = {},
			bool enabled = false
		);

		/// Removes a command from the manager
		///
		void removeCommand(CommandBase);

		/// Call a local or remote command from the framework
		///
		int callCommand(std::string name, std::vector<std::string> args, std::string owner = "");

		/// Get name of the onner layer of the Command Manager
		///
		std::string getOwner();

		/// Change current command running level
		/// 
		void changeLevel(uint level);

		/// Get current command running level
		///
		uint getCurrentLevel();

		/// Disable local command
		///
		void disableCommand(std::string);

		/// Enable local command
		///
		void enableCommand(std::string);

		void changeTransitionSet(const std::string& command_name, const dls::CommandBase::LevelType& transition_set);

		/*! @brief Wait command to be ready until timeout or the stop_wait variable becomes true 
		*/
		bool waitCommand(const std::string& owner, const std::string& name, bool& stop_wait);

		/*! @brief Wait command to be ready until timeout or the stop_wait variable becomes true 
		 * @details Using atomic_bool instead of bool 
		*/
		bool waitCommand(const std::string& owner, const std::string& name, std::atomic_bool& stop_wait);

		/*! @brief Get command by name
		*/
		std::shared_ptr<CommandBase> getCommand(const std::string& name);

		/*! @brief Trigger the level watcher
		*/
		void triggerLevelWatcher();
	private:

		/// Storage space for the commands
		///
		std::map<std::string, std::shared_ptr<CommandBase>> commands;

		/// Owner layer of the commands
		///
		std::string owner;

		/// fastdds remote commands monitor
		///
		std::shared_ptr<dls::DDSWriter> commands_monitor;

		/// Current command running level
		///
		uint level;

		void levelWatcher();

		void verifyLevel();

		std::mutex levelMutex;
		std::condition_variable levelCondVar;

		std::thread levelThread;

		/// Sends message over the framework distributed system
		///
		void sendMessage(std::pair<std::string, std::string> cmdData, std::vector<std::string> args);

		std::atomic<bool> should_exit;

		std::atomic_bool trigger_level_watcher;

	};

} // end namespace dls

#include "dls2/command/command_manager.tpp"

#endif /* end of include guard: COMMAND_MANAGER_HPP */
