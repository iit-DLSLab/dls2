#include "application_framework/control_layer.hpp"
#include <iostream> // TODO temp
#include <dlfcn.h>
#include <stdexcept>
#include <sstream>
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
bool ControlLayer::activateController(Controller::ID_t ID)
{
	std::lock_guard<std::mutex> lock(this->controllers_mutex);
	auto it = this->controllers.find(ID);

	if(it == this->controllers.end()) return false;

	it->second->run();
	return true;
}

bool ControlLayer::deactivateController(Controller::ID_t ID)
{
	std::lock_guard<std::mutex> lock(this->controllers_mutex);
	auto it = this->controllers.find(ID);

	if(it == this->controllers.end()) return false;

	// TODO shutdown might need to be called stop or pause or whatever. Maybe
	// add another virtual function in AppLayerComponent
	it->second->shutdown();
	return true;
}

void ControlLayer::loadController(const std::string &name)
{
	// std::cout << name << std::endl;
	void *controller_lib = dlopen(name.c_str(), RTLD_NOW);
	// void *controller_lib = dlopen(name.c_str(), RTLD_LAZY);
	if(!controller_lib)
	{
		std::stringstream ss;
		ss << "Error: could not load object " << name << ": " << dlerror();
		throw std::runtime_error(ss.str());
	}

	// clear errors
	dlerror();

	Controller::create_t *create_controller =
		(Controller::create_t*)dlsym(controller_lib, "create");

	if(!create_controller)
	{
		std::stringstream ss;
		ss	<< "Error: could not find instantiation code in " << name
			<< ". Did the module export the class?" << dlerror();
		throw std::runtime_error(ss.str());
	}

	// clear errors
	dlerror();

	Controller::destroy_t *destroy_controller =
		(Controller::destroy_t*)dlsym(controller_lib, "destroy");

	if(!destroy_controller)
	{
		std::stringstream ss;
		ss	<< "Error: could not find destruction code in " << name
			<< ". Did the module export the class?" << dlerror();
		throw std::runtime_error(ss.str());
	}

	// clear errors
	dlerror();

	std::shared_ptr<Controller> pController
		(
			create_controller(),	// create a pointer to be managed by the shared_ptr
			destroy_controller		// use this as the deleter of the shared_ptr
		);

	this->addController(pController);
}
