#include "dls2/command/command.hpp"
#include "dls2/command/command_manager.hpp"

#include <chrono>
#include <iostream>
#include <catch2/catch.hpp>


TEST_CASE("Commands can be added to the framework", "[command]")
{
	bool foo_called = false;

	dls::CommandManager command_manager("example_command_owner");

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
		)
	);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	SECTION( "Another component may call the command that has been registered")
	{
		auto pCommand = command_manager.find("example_command_owner", "foo");

		REQUIRE(!pCommand.empty());

		command_manager.callCommand("foo", {});
		
		std::this_thread::sleep_for(std::chrono::seconds(1));

		REQUIRE(foo_called == true);
	}
}
