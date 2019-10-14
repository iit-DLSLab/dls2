#include "application_framework/program_layer.hpp"
#include <iostream> // TODO temp
// =============================================================================
// Constructors
// =============================================================================
ProgramLayer::ProgramLayer() :
	controllers(),
	controllers_mutex()
{ }

// =============================================================================
// Interface Override Functions
// =============================================================================
ProgramLayer::Status ProgramLayer::run()
{
	std::cout << "Running program layer" << std::endl;
	{
		std::lock_guard<std::mutex> lock(this->components_mutex);
		// TODO put in threads from thread pool
		for(const auto &pController : controllers)
		{
			pController->run();
		}

	}
	return getStatus();
}

ProgramLayer::Status ProgramLayer::shutdown()
{
	return getStatus();
}

// =============================================================================
// Class Implementation Functions
// =============================================================================
