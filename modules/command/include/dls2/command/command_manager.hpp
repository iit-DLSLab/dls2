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
#include <stop_token>
#include <deque>
#include <map>
#include <set>
#include <mutex>
#include <chrono>

#include <fastdds/dds/subscriber/DataReader.hpp>

namespace dls
{
	/// A class that manages commands
	///
	/// This class updates its internal representations as commands advertise
	/// themselves and remove themselves from the framework
	class CommandManager
	{
	public:
		/// Default max time a command list query waits for the replies of the nodes
		///
		static constexpr int DEFAULT_LIST_REPLY_TIMEOUT_MS = 300;

		/// Default time a command list query result is reused by the following queries
		///
		static constexpr int DEFAULT_LIST_CACHE_VALIDITY_MS = 150;

		/// Constructor
		///
		/// @param owner_ name of the owner of the commands
		/// @param list_reply_timeout_ms max time a command list query (getCommandsList, find,
		/// callCommand, waitCommand) waits for the replies of the nodes
		CommandManager(std::string owner_, int list_reply_timeout_ms = DEFAULT_LIST_REPLY_TIMEOUT_MS);

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
		bool waitCommand(const std::string& owner, const std::string& name, bool& stop_wait, int timeout_ms = 3000);

		/*! @brief Wait command to be ready until timeout or the stop_wait variable becomes true 
		 * @details Using atomic_bool instead of bool 
		*/
		bool waitCommand(const std::string& owner, const std::string& name, std::atomic_bool& stop_wait, int timeout_ms = 3000);

		/*! @brief Get command by name
		*/
		std::shared_ptr<CommandBase> getCommand(const std::string& name);

		/*! @brief Trigger the level watcher
		*/
		void triggerLevelWatcher();

		/*! @brief Set the max time a command list query waits for the replies of the nodes (ms, > 0)
		*/
		void setListReplyTimeout(int timeout_ms);

		/*! @brief Get the max time a command list query waits for the replies of the nodes (ms)
		*/
		int getListReplyTimeout();

		/*! @brief Set for how long (ms) the result of a command list query is reused by the following queries,
		 * instead of asking the nodes again (0 disables the reuse). A command activated or deactivated in another
		 * node in the meantime is seen by the first query after this time
		*/
		void setListCacheValidity(int validity_ms);

		/*! @brief Get for how long (ms) the result of a command list query is reused by the following queries
		*/
		int getListCacheValidity();

		/// Reserved command name used to request the list of active commands of the nodes
		///
		inline static const std::string LIST_COMMANDS_REQUEST = "__dls_list_commands__";

	private:

		/// Storage space for the commands (name -> command, i.e. function + argument types)
		///
		std::map<std::string, std::shared_ptr<CommandBase>> commands;

		/// Protects the commands map
		///
		std::mutex commandsMutex;

		/// Owner layer of the commands
		///
		std::string owner;

		/// Single DDS participant of the command manager on the command domain.
		/// It holds the command_call writer, the single command_call reader and the
		/// command_list writer/reader used to discover the commands
		///
		std::shared_ptr<dls::DDSWriter> commands_monitor;

		/// Names of the DDS entities created in commands_monitor
		///
		std::string monitorWriterName;
		std::string commandsReaderName;
		std::string listWriterName;
		std::string listReaderName;

		/// Single reader of the command_call topic
		///
		std::atomic<eprosima::fastdds::dds::DataReader*> commandsReader;

		/// Reader of the command_list topic (replies to the list queries of this manager)
		///
		std::atomic<eprosima::fastdds::dds::DataReader*> listReader;
		std::mutex listReaderMutex;

		/// Current command running level
		///
		uint level;

		/// Level watcher thread loop: re-verifies the active commands at each trigger until a stop
		/// is requested on the level thread
		///
		void levelWatcher(std::stop_token stop_token);

		void verifyLevel();

		std::mutex levelMutex;
		/// condition_variable_any, to be woken up by a stop request on the level thread
		std::condition_variable_any levelCondVar;

		std::jthread levelThread;

		/// Sends message over the framework distributed system
		///
		void sendMessage(std::pair<std::string, std::string> cmdData, std::vector<std::string> args);


		std::atomic_bool trigger_level_watcher;

		// ============================ Command execution ============================

		/// Callback of the command_call reader (DDS thread). It only queues: command calls for
		/// the executor thread, list requests for the replier thread (nothing is published from
		/// the listener)
		///
		void onCommandCall(void* msg);

		/// Handles a single command_call message
		///
		void handleCommandCall(const dls2_interface::msg::CommandCall& msg);

		/// Answers a list request with the active commands of this node
		///
		void replyCommandsList(const std::string& request_id);

		/// Replier thread loop: answers the queued list requests until a stop is requested on the
		/// replier thread. It is separated from the executor so that a long command does not delay
		/// the replies (the node would look gone to the clients)
		///
		void replierLoop(std::stop_token stop_token);

		std::deque<std::string> listRequestQueue;
		std::mutex listRequestMutex;
		std::condition_variable_any listRequestCv;
		std::jthread replierThread;

		/// Executor thread loop: executes the queued command calls in FIFO order until a stop is
		/// requested on the executor thread
		///
		void executorLoop(std::stop_token stop_token);

		std::deque<std::pair<std::string, std::vector<std::string>>> callQueue;
		std::mutex queueMutex;
		/// condition_variable_any, to be woken up by a stop request on the executor thread
		std::condition_variable_any queueCv;
		std::jthread executorThread;

		// ============================ Command discovery ============================

		/// Query the active commands of the node "target" (all nodes if empty).
		/// Returns a multimap {command name, owner}
		///
		/// The result of a recent query is reused (see setListCacheValidity), and concurrent queries
		/// share the same request
		///
		std::multimap<std::string, std::string> queryCommands(const std::string& target = "");

		/// Send a list request to the node "target" (all nodes if empty) and collect the replies
		///
		std::multimap<std::string, std::string> sendListRequest(const std::string& target);

		/// Invalidate the reused query results (the commands of this manager have changed)
		///
		void invalidateQueryCache();

		struct CachedQuery
		{
			std::chrono::steady_clock::time_point time;
			uint64_t generation;
			std::multimap<std::string, std::string> commands;
		};

		/// Recent query results, per target ("" for all the nodes)
		///
		std::map<std::string, CachedQuery> queryCache;

		/// Serializes the queries: a query waiting for another one then reuses its result
		///
		std::mutex queryCacheMutex;

		/// Incremented when the commands of this manager change, to invalidate the cached results
		///
		std::atomic<uint64_t> queryCacheGeneration;

		/// Validity of the cached results (ms)
		///
		std::atomic_int listCacheValidityMs;

		/// Owners of the command managers currently matched with the command_call writer
		///
		std::set<std::string> getMatchedOwners();

		/// Create the command_list reader if not yet created (done in the constructor)
		///
		bool ensureListReader();

		/// Callback of the command_list reader (DDS thread)
		///
		void onCommandsList(void* msg);

		/// Stores a command_list reply if its request is pending
		///
		void handleCommandsList(const dls2_interface::msg::CommandCall& msg);

		struct PendingQuery
		{
			std::set<std::string> replied_owners;
			std::multimap<std::string, std::string> commands;
		};

		std::map<std::string, PendingQuery> pendingQueries;
		std::mutex queryMutex;
		std::condition_variable queryCv;
		std::atomic<uint64_t> queryCounter;

		/// Max time a query waits for the replies of the nodes
		///
		std::atomic_int listReplyTimeoutMs;

		/// Check that a command name can be used: it cannot be empty, contain ',' (used as
		/// separator in the command lists) or start with "__dls" (reserved)
		///
		bool isValidCommandName(const std::string& name);

		/// Split a comma separated string (a trailing piece without comma is ignored)
		///
		static std::vector<std::string> parseArgs(std::string args);

	};

} // end namespace dls

#include "dls2/command/command_manager.tpp"

#endif /* end of include guard: COMMAND_MANAGER_HPP */
