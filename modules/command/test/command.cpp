#include "dls2/command/command.hpp"
#include "dls2/command/command_manager.hpp"

#include <chrono>
#include <iostream>
#include <catch2/catch.hpp>


TEST_CASE("Commands can be added to the framework", "[command]")
{
	bool foo_called = false;

	dls::CommandManager command_manager("first_owner");
	dls::CommandManager command_manager_2("second_owner");

	command_manager.addCommand<>
	(
		"foo",
		"prints foo",
		std::function<bool()>([&]()->bool
		{
			foo_called = true;
            return true;
		}),
		{},
		true
	);
	std::this_thread::sleep_for(std::chrono::seconds(2));

	SECTION( "Another component may call the command that has been registered")
	{
		auto commandList = command_manager_2.find("first_owner", "foo");

		REQUIRE(!commandList.empty());

		command_manager_2.callCommand("foo", {});
		
		std::this_thread::sleep_for(std::chrono::seconds(1));

		REQUIRE(foo_called == true);
	}
}
