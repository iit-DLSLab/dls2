#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <csignal>

#include "util/class_loader.hpp"
#include "gait_generator/gait_generator.hpp"
#include "util/debug/debug.hpp"
#include "path_prefixes/path_prefixes.hpp"

std::shared_ptr<GaitGenerator> pGaitGenerator;
void signal_handler(int signal);

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <gait_generator_name>" << std::endl;
		std::cerr << "argc: " << argc << std::endl;
		exit(EXIT_FAILURE);
	}

	try
	{
		pGaitGenerator =
			ClassLoader::loadClass<GaitGenerator>(std::string(LIBRARY_PROCESS_PATH "lib") + argv[1] + ".so");
	}
	catch(const std::exception&)
	{
		TODO("Inform user that file not found");
		DMSG("GaitGenerator " << argv[1] << " not found");
		exit((int)GaitGenerator::Status::FATAL_ERROR);
	}

	std::signal(SIGTERM, signal_handler);
	DMSG("GaitGenerator LOADED IN CHILD PROCESS");
	pGaitGenerator->run();

	return static_cast<int>(pGaitGenerator->getStatus());
}

void signal_handler(int signal)
{
	if(signal == SIGTERM)
	{
		DMSG("SIGTERM DEBOUNCE");
		pGaitGenerator->stop();
		exit(static_cast<int>(pGaitGenerator->getStatus()));
	}
}

// If this class isn't delcared, the linker will link the whole class away, then
// the call to loadClass will fail
class VoidGaitGenerator : public GaitGenerator
{
public:
	VoidGaitGenerator
	(
	) :
		GaitGenerator
		(
			nullptr,
			"dummy_controller",
			std::chrono::duration<double>(1)
		)
	{ }

	~VoidGaitGenerator() = default;

	void run(const std::chrono::system_clock::time_point &time)override
	{
		time.time_since_epoch();
	};

	Status eStop() override { return getStatus(); }
}voidGaitGenerator;
