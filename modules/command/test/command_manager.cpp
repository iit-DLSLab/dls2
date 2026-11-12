#include "dls2/command/command.hpp"
#include "dls2/command/command_manager.hpp"
#include <catch2/catch.hpp>

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

using namespace dls;

// These tests aren't testing for any specific functionality
// sometimes, when changing the version of fastdds, some things will break
// randomly. These tests just build various objects in the code base that may
// use dds functionality. If these tests segfault, it can give a quick
// indication of where things are going wrong in new versions
//
// These tests are therefore very coarse grained, aimed at only giving an
// initial direction for investigation.
TEST_CASE("Creating a command manager and adding a command works", "[command manager]")
{
	dls::DDSParticipant server("test_server", dls::domains::command, eprosima::fastdds::rtps::DiscoveryProtocol::SERVER);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	CommandManager cm("test");

	cm.addCommand<>
	(
		"dummy_command",
		"no docs required",
		std::function<bool()>([&]()->bool
		{
			// empty command
            return true;
		}),
        {},
        true
	);
}

namespace
{
	bool hasCommand(const std::multimap<std::string, std::string>& list, const std::string& name, const std::string& owner)
	{
		auto range = list.equal_range(name);
		for (auto it = range.first; it != range.second; ++it)
			if (it->second == owner)
				return true;
		return false;
	}

	bool waitFor(const std::function<bool()>& pred, int timeout_ms = 3000)
	{
		for (int elapsed = 0; elapsed < timeout_ms && !pred(); elapsed += 10)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		return pred();
	}

	struct BarOwner
	{
		std::atomic_int calls{0};
		bool bar() { calls++; return true; }
	};
}

TEST_CASE("Commands are discovered and called through the single reader of the command manager", "[command manager]")
{
	dls::DDSParticipant server("test_server", dls::domains::command, eprosima::fastdds::rtps::DiscoveryProtocol::SERVER);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::atomic_int foo_value{-1};
	BarOwner bar_owner;

	CommandManager cm_b("B");

	// The list reply timeout is a setting of the command manager
	REQUIRE(cm_b.getListReplyTimeout() == CommandManager::DEFAULT_LIST_REPLY_TIMEOUT_MS);
	cm_b.setListReplyTimeout(500);
	REQUIRE(cm_b.getListReplyTimeout() == 500);
	cm_b.setListReplyTimeout(0); // invalid, ignored
	REQUIRE(cm_b.getListReplyTimeout() == 500);
	{
		CommandManager cm_a("A");

		cm_a.addCommand<int>
		(
			"foo",
			"stores its argument",
			std::function<bool(int)>([&](int value)->bool
			{
				foo_value = value;
				return true;
			}),
			{},
			true
		);
		cm_a.addCommand("bar", "counts the calls", &BarOwner::bar, &bar_owner, {}, true);

		// Reserved and malformed names are rejected
		cm_a.addCommand<>("__dls_reserved", "", std::function<bool()>([]{ return true; }), {}, true);
		cm_a.addCommand<>("a,b", "", std::function<bool()>([]{ return true; }), {}, true);
		REQUIRE(cm_a.getCommand("__dls_reserved") == nullptr);
		REQUIRE(cm_a.getCommand("a,b") == nullptr);

		// Discovery (client B, server A)
		bool stop = false;
		REQUIRE(cm_b.waitCommand("A", "foo", stop, 10000));

		auto list = cm_b.getCommandsList();
		REQUIRE(hasCommand(list, "foo", "A"));
		REQUIRE(hasCommand(list, "bar", "A"));
		REQUIRE(cm_b.find("A", "bar").size() == 1);
		REQUIRE(cm_b.find("A", "not_a_command").empty());

		// A list query also returns the commands of the querying manager itself
		REQUIRE(hasCommand(cm_a.getCommandsList(), "foo", "A"));

		// Remote calls with and without arguments
		REQUIRE(cm_b.callCommand("foo", {"3"}, "A") == 1);
		REQUIRE(waitFor([&]{ return foo_value.load() == 3; }));

		REQUIRE(cm_b.callCommand("bar", {}, "A") == 1);
		REQUIRE(waitFor([&]{ return bar_owner.calls.load() == 1; }));

		// A burst of calls is entirely executed, in order
		for (int i = 10; i < 30; i++)
			cm_b.callCommand("foo", {std::to_string(i)}, "A");
		REQUIRE(waitFor([&]{ return foo_value.load() == 29; }));

		// A deactivated command is neither listed nor callable. Each call of foo changes the level
		// (asynchronously re-verified by the level watcher, which re-activates foo since its
		// transition set is empty): let the level watcher settle before deactivating it
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		cm_a.disableCommand("foo");
		REQUIRE_FALSE(hasCommand(cm_b.getCommandsList(), "foo", "A"));
		REQUIRE(cm_b.callCommand("foo", {"5"}, "A") == 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		REQUIRE(foo_value.load() == 29);
	}

	// Once A is destroyed, its commands are not listed anymore
	REQUIRE_FALSE(hasCommand(cm_b.getCommandsList(), "bar", "A"));
}
