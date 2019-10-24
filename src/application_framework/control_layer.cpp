#include "application_framework/control_layer.hpp"
#include <iostream> // TODO temp
// #include <dlfcn.h>
// #include <stdexcept>
// #include <sstream>
// =============================================================================
// Constructors
// =============================================================================
ControlLayer::ControlLayer() :
	controllers(),
	controllers_mutex()
{ }

// =============================================================================
// Interface Override Functions
// =============================================================================
ControlLayer::Status ControlLayer::run()
{
	std::cout << "Running program layer" << std::endl;
	{
		std::lock_guard<std::mutex> lock(this->components_mutex);
		// TODO this is wrong, shouldn't automatically run all of the
		// controllers
		// for(const auto &el : controllers)
		// {
		// 	// el.second->run();
		// }
	}

	// TODO spawn realtime thread for managing controllers

	// TODO spawn nonrealtime thread for user interaction

	return getStatus();
}

ControlLayer::Status ControlLayer::shutdown()
{
	return getStatus();
}

// =============================================================================
// Class Implementation Functions
// =============================================================================
bool ControlLayer::activateController(const Controller::ID_t &ID)
{
	std::lock_guard<std::mutex> lock(this->controllers_mutex);
	auto it = this->controllers.find(ID);

	if(it == this->controllers.end()) return false;

	it->second->run();
	return true;
}

bool ControlLayer::deactivateController(const Controller::ID_t &ID)
{
	std::lock_guard<std::mutex> lock(this->controllers_mutex);
	auto it = this->controllers.find(ID);

	if(it == this->controllers.end()) return false;

	// TODO shutdown might need to be called stop or pause or whatever. Maybe
	// add another virtual function in AppLayerComponent
	it->second->shutdown();
	return true;
}

void ControlLayer::loadController(const Controller::ID_t &name)
{
	std::shared_ptr<Controller> pController = ControlLayer::loadClass<Controller>(name);
	this->addController(pController);
}
