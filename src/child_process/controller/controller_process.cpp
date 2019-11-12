#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <csignal>

#include "util/class_loader.hpp"
#include "controller/controller.hpp"
#include "util/debug/debug.hpp"

#include "path_prefixes/path_prefixes.hpp"

std::shared_ptr<Controller> pController;
void signal_handler(int signal);

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <controller_name>" << std::endl;
		std::cerr << "argc: " << argc << std::endl;
		exit(EXIT_FAILURE);
	}

	try
	{
		pController =
			ClassLoader::loadClass<Controller>(std::string(LIBRARY_PROCESS_PATH "lib") + argv[1] + ".so");
	}
	catch(const std::exception&)
	{
		TODO("Inform user that file not found");
		DMSG("Controller not found");
		exit((int)Controller::Status::FATAL_ERROR);
	}

	std::signal(SIGTERM, signal_handler);
	DMSG("CONTROLLER LOADED IN CHILD PROCESS");
	pController->run();

	return static_cast<int>(pController->getStatus());
}

void signal_handler(int signal)
{
	if(signal == SIGTERM)
	{
		DMSG("SIGTERM DEBOUNCE");
		pController->stop();
		exit(static_cast<int>(pController->getStatus()));
	}
}

// If this class isn't delcared, the linker will link the whole class away, then
// the call to loadClass will fail
class VoidController : public Controller
{
public:
	VoidController
	(
	) :
		Controller
		(
			nullptr,
			"dummy_controller",
			std::chrono::duration<double>(1),
			ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
		)
	{ }

	~VoidController() = default;

	void run(const std::chrono::system_clock::time_point &time)override
	{
		time.time_since_epoch();
	};

	Status eStop() override { return getStatus(); }
}voidController;
