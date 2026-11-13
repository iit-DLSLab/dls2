#ifndef COMMAND_MANAGER_CPP
#define COMMAND_MANAGER_CPP

#include "dls2/command/command_manager.hpp"
#include "dls2/util/utils.hpp"

#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/rtps/common/InstanceHandle.hpp>

#include <chrono>

using namespace dls;

namespace
{
	// Each poll of waitCommand is a list query (a round trip with the node), so do not poll too fast
	constexpr int wait_poll_period_ms = 100;

	// Suffix of the participant name of a CommandManager (<owner>::commands_monitor)
	const std::string monitor_suffix = "::commands_monitor";

	// RELIABLE + KEEP_ALL: a sample is resent until every matched reader has acknowledged it, and a
	// reader never overwrites a sample it has not taken yet. VOLATILE: samples are not kept for late
	// joiners (with KEEP_ALL, TRANSIENT_LOCAL would keep every sample ever written).
	eprosima::fastdds::dds::DataWriterQos reliableKeepAllWriterQos()
	{
		eprosima::fastdds::dds::DataWriterQos qos(eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT);
		qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
		qos.reliability().max_blocking_time = 10000; // waiting at most 10s for the arrival confirmation from the data reader
		qos.history().kind = eprosima::fastdds::dds::KEEP_ALL_HISTORY_QOS;
		qos.durability().kind = eprosima::fastdds::dds::VOLATILE_DURABILITY_QOS;
		return qos;
	}

	eprosima::fastdds::dds::DataReaderQos reliableKeepAllReaderQos()
	{
		eprosima::fastdds::dds::DataReaderQos qos(eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
		qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
		qos.history().kind = eprosima::fastdds::dds::KEEP_ALL_HISTORY_QOS;
		qos.durability().kind = eprosima::fastdds::dds::VOLATILE_DURABILITY_QOS;
		return qos;
	}
}

CommandManager::CommandManager(std::string owner_, int list_reply_timeout_ms)
	: commands()
	, owner(owner_)
	, monitorWriterName(owner_ + "::commands_monitor")
	, commandsReaderName(owner_ + "::commands_listener")
	, listWriterName(owner_ + "::commands_list")
	, listReaderName(owner_ + "::commands_list_reader")
	, commandsReader(nullptr)
	, listReader(nullptr)
	, level(0)
	, trigger_level_watcher(false)
	, queryCacheGeneration(0)
	, listCacheValidityMs(DEFAULT_LIST_CACHE_VALIDITY_MS)
	, queryCounter(0)
	, listReplyTimeoutMs(DEFAULT_LIST_REPLY_TIMEOUT_MS)
{
	setListReplyTimeout(list_reply_timeout_ms);

	// Single participant of the command manager. It is named <owner>::commands_monitor, which is
	// used by the clients to know which command managers are expected to reply to a list query
	commands_monitor = std::make_shared<dls::DDSWriter>(
		monitorWriterName,
		domains::command,
		dls::topics::command_call,
		reliableKeepAllWriterQos()
	);

	// Writer used to reply to the list queries
	commands_monitor->addWriter(listWriterName, dls::topics::command_list, reliableKeepAllWriterQos());

	// Start the threads before creating the reader, so that every received call can be served
	levelThread = std::jthread([this](std::stop_token stop_token){ this->levelWatcher(stop_token); });
	executorThread = std::jthread([this](std::stop_token stop_token){ this->executorLoop(stop_token); });
	replierThread = std::jthread([this](std::stop_token stop_token){ this->replierLoop(stop_token); });

	// Single reader of the command_call topic, serving all the commands of this manager
	commandsReader.store(commands_monitor->addReader(
		commandsReaderName,
		dls::topics::command_call,
		std::function<void(void *)>([this](void* msg){ this->onCommandCall(msg); }),
		reliableKeepAllReaderQos()
	));

	if (commandsReader.load() == nullptr)
		std::cerr << "[CommandManager] " << owner << ": could not create the command_call reader" << std::endl;

	// Reader of the list replies, created now (and not at the first query) so that it is already
	// matched with the reply writers of the other nodes when the first query is sent: the replies
	// are volatile, a reply sent before the match would be lost
	ensureListReader();
}

CommandManager::~CommandManager()
{
	// Stop receiving: no DDS callback can reference this object after this point
	if (commandsReader.exchange(nullptr) != nullptr)
		commands_monitor->deleteReader(commandsReaderName);
	{
		std::lock_guard<std::mutex> lock(listReaderMutex);
		if (listReader.exchange(nullptr) != nullptr)
			commands_monitor->deleteReader(listReaderName);
	}

	// Stop the replier and the executor (the stop requests wake them up), before stopping the level watcher
	replierThread.request_stop();
	executorThread.request_stop();
	if (replierThread.joinable())
		replierThread.join();
	if (executorThread.joinable())
		executorThread.join();

	// Stop the level watcher (the stop request wakes it up)
	levelThread.request_stop();
	if (levelThread.joinable())
		levelThread.join();

	{
		std::lock_guard<std::mutex> lock(commandsMutex);
		for(auto elem : this->commands){
			elem.second->deactivate();
		}
	}
}

// =============================================================================
// Command execution
// =============================================================================

bool CommandManager::isValidCommandName(const std::string& name)
{
	if (name.empty() || name.find(',') != std::string::npos || name.rfind("__dls", 0) == 0)
	{
		std::cerr << "[CommandManager] " << owner << ": invalid command name '" << name
				  << "' (it cannot be empty, contain ',' or start with \"__dls\"). The command is not added" << std::endl;
		return false;
	}
	return true;
}

std::vector<std::string> CommandManager::parseArgs(std::string args)
{
	std::vector<std::string> result;
	size_t pos = 0;
	while ((pos = args.find(",")) != std::string::npos) {
		result.push_back(args.substr(0, pos));
		args.erase(0, pos + 1);
	}
	return result;
}

void CommandManager::onCommandCall(void* msg)
{
	handleCommandCall(*static_cast<dls2_interface::msg::CommandCall*>(msg));

	// The listener takes a single sample per notification, while Fast DDS may notify once for
	// several samples: drain the reader so that no sample is left behind
	auto reader = commandsReader.load();
	if (reader == nullptr)
		return;

	dls2_interface::msg::CommandCall sample;
	eprosima::fastdds::dds::SampleInfo info;
	while (reader->take_next_sample(&sample, &info) == eprosima::fastdds::dds::RETCODE_OK)
	{
		if (info.valid_data)
			handleCommandCall(sample);
	}
}

void CommandManager::handleCommandCall(const dls2_interface::msg::CommandCall& msg)
{
	// if the owner is known but it is not this one, do nothing
	if (msg.owner() != "" && msg.owner() != this->owner)
		return;

	// List requests are answered by the replier thread, so that a running command cannot delay
	// the reply and nothing is published from the listener
	if (msg.command_name() == LIST_COMMANDS_REQUEST)
	{
		{
			std::lock_guard<std::mutex> lock(listRequestMutex);
			if (replierThread.get_stop_token().stop_requested())
				return;
			listRequestQueue.push_back(msg.args());
		}
		listRequestCv.notify_one();
		return;
	}

	{
		std::lock_guard<std::mutex> lock(queueMutex);
		if (executorThread.get_stop_token().stop_requested())
			return;
		callQueue.emplace_back(msg.command_name(), parseArgs(msg.args()));
	}
	queueCv.notify_one();
}

void CommandManager::replyCommandsList(const std::string& request_id)
{
	std::string list;
	{
		std::lock_guard<std::mutex> lock(commandsMutex);
		for (auto& cmd : commands)
		{
			if (cmd.second->isActive())
			{
				list.append(cmd.first);
				list.append(",");
			}
		}
	}

	dls2_interface::msg::CommandCall reply;
	reply.owner(this->owner);
	reply.command_name(request_id);
	reply.args(list);
	commands_monitor->DDSParticipant::sendMessage(listWriterName, &reply);
}

void CommandManager::replierLoop(std::stop_token stop_token)
{
	while (!stop_token.stop_requested())
	{
		std::string request_id;
		{
			std::unique_lock<std::mutex> lock(listRequestMutex);
			// returns false when woken up by a stop request with no request queued
			if (!listRequestCv.wait(lock, stop_token, [this]{ return !listRequestQueue.empty(); }))
				return;
			if (stop_token.stop_requested())
				return;
			request_id = std::move(listRequestQueue.front());
			listRequestQueue.pop_front();
		}

		replyCommandsList(request_id);
	}
}

void CommandManager::executorLoop(std::stop_token stop_token)
{
	while (!stop_token.stop_requested())
	{
		std::pair<std::string, std::vector<std::string>> call;
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			// returns false when woken up by a stop request with no call queued
			if (!queueCv.wait(lock, stop_token, [this]{ return !callQueue.empty(); }))
				return;
			if (stop_token.stop_requested())
				return;
			call = std::move(callQueue.front());
			callQueue.pop_front();
		}

		// An inactive command does not serve the calls (as when each command had its own reader)
		std::shared_ptr<CommandBase> cmd;
		{
			std::lock_guard<std::mutex> lock(commandsMutex);
			auto it = commands.find(call.first);
			if (it == commands.end() || !it->second->isActive())
				continue;
			cmd = it->second;
		}

		// Executed without holding any lock: the command may use the command manager
		cmd->call(call.second);
	}
}

// =============================================================================
// Command discovery
// =============================================================================

bool CommandManager::ensureListReader()
{
	if (listReader.load() != nullptr)
		return true;

	std::lock_guard<std::mutex> lock(listReaderMutex);
	if (listReader.load() != nullptr)
		return true;

	listReader.store(commands_monitor->addReader(
		listReaderName,
		dls::topics::command_list,
		std::function<void(void *)>([this](void* msg){ this->onCommandsList(msg); }),
		reliableKeepAllReaderQos()
	));

	if (listReader.load() == nullptr)
	{
		std::cerr << "[CommandManager] " << owner << ": could not create the command_list reader" << std::endl;
		return false;
	}
	return true;
}

void CommandManager::onCommandsList(void* msg)
{
	handleCommandsList(*static_cast<dls2_interface::msg::CommandCall*>(msg));

	auto reader = listReader.load();
	if (reader == nullptr)
		return;

	dls2_interface::msg::CommandCall sample;
	eprosima::fastdds::dds::SampleInfo info;
	while (reader->take_next_sample(&sample, &info) == eprosima::fastdds::dds::RETCODE_OK)
	{
		if (info.valid_data)
			handleCommandsList(sample);
	}
}

void CommandManager::handleCommandsList(const dls2_interface::msg::CommandCall& msg)
{
	{
		std::lock_guard<std::mutex> lock(queryMutex);
		auto it = pendingQueries.find(msg.command_name());
		if (it == pendingQueries.end())
			return;

		if (!it->second.replied_owners.insert(msg.owner()).second)
			return; // duplicated reply

		for (auto& name : parseArgs(msg.args()))
			it->second.commands.insert({name, msg.owner()});
	}
	queryCv.notify_all();
}

std::set<std::string> CommandManager::getMatchedOwners()
{
	std::set<std::string> owners;

	// This command manager always has its own reader
	owners.insert(this->owner);

	std::shared_ptr<dls::DDSPubListener> listener;
	try
	{
		listener = commands_monitor->getPubListener(monitorWriterName);
	}
	catch (const std::exception&)
	{
		return owners;
	}
	if (listener == nullptr)
		return owners;

	// Find the participant of each matched data reader, and keep the ones of the command managers
	auto participants_info = commands_monitor->getDiscoveredParticipantsInfo();
	for (auto datareader_instance : listener->get_matched_datareaders_instances())
	{
		auto prefix = eprosima::fastdds::rtps::iHandle2GUID(datareader_instance).guidPrefix;
		for (auto& participant_info : participants_info)
		{
			if (participant_info.second.guidPrefix != prefix)
				continue;

			const std::string& participant_name = participant_info.first;
			if (participant_name.size() > monitor_suffix.size() &&
				participant_name.compare(participant_name.size() - monitor_suffix.size(), monitor_suffix.size(), monitor_suffix) == 0)
			{
				owners.insert(participant_name.substr(0, participant_name.size() - monitor_suffix.size()));
			}
		}
	}

	return owners;
}

std::multimap<std::string, std::string> CommandManager::queryCommands(const std::string& target)
{
	const int validity_ms = listCacheValidityMs.load();
	if (validity_ms <= 0)
		return sendListRequest(target);

	// One query at a time: a caller waiting here then finds the fresh result of the previous query
	std::lock_guard<std::mutex> lock(queryCacheMutex);

	const auto now = std::chrono::steady_clock::now();
	const uint64_t generation = queryCacheGeneration.load();
	auto isFresh = [&](const CachedQuery& cached)
	{
		return cached.generation == generation && (now - cached.time) < std::chrono::milliseconds(validity_ms);
	};

	auto it = queryCache.find(target);
	if (it != queryCache.end() && isFresh(it->second))
		return it->second.commands;

	// A fresh result of all the nodes also answers a query to a single node
	if (!target.empty())
	{
		auto all = queryCache.find("");
		if (all != queryCache.end() && isFresh(all->second))
		{
			std::multimap<std::string, std::string> commands;
			for (auto& entry : all->second.commands)
				if (entry.second == target)
					commands.insert(entry);
			return commands;
		}
	}

	// The time is taken before sending the request, so that the result is never considered newer than it is
	CachedQuery cached{std::chrono::steady_clock::now(), generation, sendListRequest(target)};
	auto commands = cached.commands;
	queryCache[target] = std::move(cached);
	return commands;
}

void CommandManager::invalidateQueryCache()
{
	queryCacheGeneration.fetch_add(1);
}

std::multimap<std::string, std::string> CommandManager::sendListRequest(const std::string& target)
{
	if (!ensureListReader())
		return {};

	const std::set<std::string> expected = target.empty() ? getMatchedOwners() : std::set<std::string>{target};
	const std::string request_id = owner + "#" + std::to_string(queryCounter.fetch_add(1));

	{
		std::lock_guard<std::mutex> lock(queryMutex);
		pendingQueries[request_id] = PendingQuery();
	}

	// Send the request without holding any lock: the intra-process replies are delivered synchronously
	dls2_interface::msg::CommandCall request;
	request.owner(target);
	request.command_name(LIST_COMMANDS_REQUEST);
	request.args(request_id);
	this->commands_monitor->sendMessage(&request);

	std::unique_lock<std::mutex> lock(queryMutex);
	queryCv.wait_for(lock, std::chrono::milliseconds(listReplyTimeoutMs.load()), [&]
	{
		const auto& replied = pendingQueries[request_id].replied_owners;
		for (auto& expected_owner : expected)
		{
			if (!replied.contains(expected_owner))
				return false;
		}
		return true;
	});

	auto result = std::move(pendingQueries[request_id].commands);
	pendingQueries.erase(request_id);
	return result;
}

std::multimap<std::string, std::string> CommandManager::findByOwner
(
	std::string owner_
)
{
	auto cmds = this->getCommandsList();

	if (cmds.size() == 0)
		return {};

	// search for commands with same owner
	std::multimap<std::string, std::string> cmdlst;
	for (auto it = cmds.begin(); it != cmds.end(); ++it)
	{
		if (it->second == owner_)
			cmdlst.insert(*it);
	}

	return cmds;
}

std::multimap<std::string, std::string> CommandManager::findByName
(
	std::string name_
)
{
	auto cmds = this->getCommandsList();

	if (cmds.count(name_) == 0)
		return {};

	// find all commands with same name and put in a new multimap
	auto range = cmds.equal_range(name_);
	std::multimap<std::string, std::string> cmdlst;
	for (auto it = range.first; it != range.second; ++it)
	{
		cmdlst.insert(*it);
	}

	return cmdlst;
}

std::multimap<std::string, std::string> CommandManager::find
(
	std::string owner_,
	std::string name_
)
{
	// Ask only the node owner_
	auto cmds = this->queryCommands(owner_);

	std::multimap<std::string, std::string> cmdlst;
	auto range = cmds.equal_range(name_);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->second == owner_)
			cmdlst.insert(*it);
	}

	return cmdlst;
}

std::multimap<std::string, std::string> CommandManager::getCommandsList()
{
	return this->queryCommands();
}

std::set<std::string> CommandManager::getOwnersList()
{
	auto remCommands = commands_monitor->getParticipants();

	std::set<std::string> set;
	std::lock_guard<std::mutex> lock(commandsMutex);
	for(auto it = commands.begin(); it != commands.end(); ++it)
	{
		set.insert(it->second->getOwner());
	}

	return set;
}

int CommandManager::callCommand(std::string name_, std::vector<std::string> args_, std::string owner_)
{
	std::multimap<std::string, std::string> cmdList;

	// find the commands
	if(owner_ == "")
	{
		if(args_.size() > 0 && args_.back() == "all")
		{
			cmdList.insert({name_, ""});
			args_.pop_back();
		}
		else
		{
			cmdList = this->findByName(name_);
		}
	}
	else
	{
		cmdList = this->find(owner_, name_);
	}

	if (cmdList.size() != 1)
		return cmdList.size();

	if (cmdList.begin()->second == this->getOwner())
	{
		std::shared_ptr<CommandBase> cmd;
		{
			std::lock_guard<std::mutex> lock(commandsMutex);
			auto it = this->commands.find(name_);
			if (it == this->commands.end())
				return 0;
			cmd = it->second;
		}
		cmd->call(args_);
		this->changeLevel(cmd->getNextLevel(this->getCurrentLevel()));
	}
	else
	{
		this->sendMessage(*(cmdList.begin()), args_);
	}

	return 1;
}

void CommandManager::sendMessage(std::pair<std::string, std::string> cmdData_, std::vector<std::string> args_)
{
	dls2_interface::msg::CommandCall msg;
	std::string outString;

	if (args_.empty())
	{
		outString.append(" ");
	}
	else
	{
		for (auto elem : args_)
		{
			outString.append(elem);
			outString.append(",");
		}
	}

	msg.owner(cmdData_.second);
	msg.command_name(cmdData_.first);
	msg.args(outString);

	this->commands_monitor->sendMessage(&msg);
}

void CommandManager::setListReplyTimeout(int timeout_ms)
{
	if (timeout_ms <= 0)
	{
		std::cerr << "[CommandManager] " << owner << ": the list reply timeout must be positive, keeping "
				  << listReplyTimeoutMs.load() << " ms" << std::endl;
		return;
	}
	listReplyTimeoutMs.store(timeout_ms);
}

int CommandManager::getListReplyTimeout()
{
	return listReplyTimeoutMs.load();
}

void CommandManager::setListCacheValidity(int validity_ms)
{
	listCacheValidityMs.store(validity_ms > 0 ? validity_ms : 0);
}

int CommandManager::getListCacheValidity()
{
	return listCacheValidityMs.load();
}

std::string CommandManager::getOwner()
{
	return this->owner;
}

uint CommandManager::getCurrentLevel()
{
	return this->level;
}

void CommandManager::changeLevel(uint level_)
{
	{
		std::unique_lock<std::mutex> lock(this->levelMutex);
		this->level = level_;
	}
	triggerLevelWatcher();
}

void CommandManager::triggerLevelWatcher()
{
	trigger_level_watcher.wait(true);
	trigger_level_watcher.store(true);
	this->levelCondVar.notify_one();
}

void CommandManager::verifyLevel()
{
	invalidateQueryCache();
	std::lock_guard<std::mutex> lock(commandsMutex);
	for(auto cmd : this->commands)
	{
		if(cmd.second->testLevel(this->level))
		{
			cmd.second->activate();
		}
		else
		{
			cmd.second->deactivate();
		}
	}
}

void CommandManager::levelWatcher(std::stop_token stop_token)
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(this->levelMutex);
		// returns false when woken up by a stop request without a pending trigger
		const bool triggered = this->levelCondVar.wait(lock, stop_token, [&]{ return trigger_level_watcher.load(); });
		if (triggered)
		{
			// release a triggerLevelWatcher() waiting for the previous trigger to be consumed
			trigger_level_watcher.store(false);
			trigger_level_watcher.notify_one();
		}
		if (stop_token.stop_requested())
			break;
		this->verifyLevel();
	}
}

void CommandManager::disableCommand(std::string name)
{
	invalidateQueryCache();
	std::lock_guard<std::mutex> lock(commandsMutex);
	if(commands.contains(name))
		this->commands[name]->deactivate();
}

void CommandManager::enableCommand(std::string name)
{
	invalidateQueryCache();
	std::lock_guard<std::mutex> lock(commandsMutex);
	if(commands.contains(name))
		this->commands[name]->activate();
}

void CommandManager::changeTransitionSet(const std::string& command_name, const dls::CommandBase::LevelType& transition_set)
{
	std::lock_guard<std::mutex> lock(commandsMutex);
	if(this->commands.contains(command_name))
		this->commands[command_name]->changeTransitionSet(transition_set);
	else
		std::cout << "Could not find command " << command_name << std::endl;
}

bool CommandManager::waitCommand(const std::string& owner, const std::string& name, bool& stop_wait, int timeout_ms){
		if(!utils::wait(std::function<bool()>([&](){
					if(this->find(owner,name).size()==0){
						return false;
					}
					return true;
				}), timeout_ms, wait_poll_period_ms, stop_wait)){
			if(!stop_wait)
				std::cerr << "Command " << owner << "::" << name<<" not found" << std::endl;
			return false;
		}
		return true;
}

bool CommandManager::waitCommand(const std::string& owner, const std::string& name, std::atomic_bool& stop_wait, int timeout_ms){
		if(!utils::wait(std::function<bool()>([&](){
					if(this->find(owner,name).size()==0){
						return false;
					}
					return true;
				}), timeout_ms, wait_poll_period_ms, stop_wait)){
			if(!stop_wait.load())
				std::cerr << "Command " << owner << "::" << name<<" not found" << std::endl;
			return false;
		}
		return true;
}

std::shared_ptr<CommandBase> CommandManager::getCommand(const std::string& name){
	std::lock_guard<std::mutex> lock(commandsMutex);
	if(this->commands.contains(name))
		return this->commands[name];
	else{
		std::cerr << "Error in getCommand: command " << name << " not found" << std::endl;
		return nullptr;
	}
}

#endif /* end of include guard: COMMAND_MANAGER_CPP */
