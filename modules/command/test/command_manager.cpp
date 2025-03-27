#include "dls2/command/command.hpp"
#include "dls2/command/command_manager.hpp"
#include <catch2/catch.hpp>

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
