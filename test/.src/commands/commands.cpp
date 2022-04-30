#include <chrono>
#include <iostream>
#include <catch2/catch.hpp>
#include "dls2/command/command_manager.hpp"

TEST_CASE("Commands can be added to the framework", "[command]")
{
	bool foo_called = false;

	std::this_thread::sleep_for(std::chrono::seconds(1));

	dls::CommandManager command_manager("commands_test");
	command_manager.addCommand<void>
	(
		"foo",
		"prints foo",
		std::function<void()>
		(
			[&]()
			{
				foo_called = true;
			}
		),
		{{0,0}},
		true
	);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	SECTION( "Another component may call the command that has been registered")
	{
		command_manager.callCommand("foo", {""});
		std::this_thread::sleep_for(std::chrono::seconds(1));

		REQUIRE(foo_called == true);
	}
}
