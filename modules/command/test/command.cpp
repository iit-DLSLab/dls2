#include "dls2/command/command.hpp"

#include <chrono>
#include <iostream>
#include <catch2/catch.hpp>


TEST_CASE("Commands can be added to the framework", "[command]")
{
	dls::impl::initFastdds();
	bool foo_called = false;

	dls::RemoteCommandManager remote_command_manager;
	std::this_thread::sleep_for(std::chrono::seconds(1));

	dls::CommandManager command_manager;
	command_manager.addCommand<void, dls::ARGVOID>
	(
		"example_command_owner",
		"foo",
		"prints foo",
		std::function<void(dls::ARGVOID)>
		(
			[&](dls::ARGVOID)
			{
				foo_called = true;
			}
		)
	);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	SECTION( "Another component may call the command that has been registered")
	{
		auto pCommand = remote_command_manager.find("example_command_owner", "foo");
		REQUIRE(pCommand != nullptr);
		pCommand->pushArg<dls::ARGVOID>(dls::ARGVOID());
		pCommand->call();
		std::this_thread::sleep_for(std::chrono::seconds(1));

		REQUIRE(foo_called == true);
	}
}
