#include "application_framework/control_layer.hpp"
TODO("remove temp iostream include")
#include <iostream>
// =============================================================================
// Constructors
// =============================================================================
ControlLayer::ControlLayer() :
	controllers(),
	controllers_mutex(),
	generators(),
	gait_generators_mutex(),
	currentActiveGenerator(nullptr)
{ }

// =============================================================================
// Interface Override Functions
// =============================================================================
ControlLayer::Status ControlLayer::run()
{
	std::cout << "Running program layer" << std::endl;
	{
		std::lock_guard<std::mutex> lock(this->components_mutex);
	}

	TODO("spawn realtime thread for managing controllers")
	TODO("spawn nonrealtime thread for user interaction")

	setStatus(Status::RUNNING);
	while(getStatus() == Status::RUNNING)
	{
		decltype(currentActiveGenerator->readSignal()) signal;
		{
			// Read the reference signal from the gait generator
			std::lock_guard<std::mutex> lock(this->gait_generators_mutex);
			if(currentActiveGenerator)
			{
				signal = currentActiveGenerator->readSignal();
			}
		}
		{
			// Send the reference signal to all active controllers
			std::lock_guard<std::mutex> lock(controllers_mutex);
			for(const auto &pair_id_pController : this->controllers)
			{
				if(pair_id_pController.second->getStatus() == Controller::Status::RUNNING)
				{
					TODO("send signal here")
					// pair_id_pController.second->pushSignal(signal);
				}
			}
		}

		TODO("sleep at correct frequency here")
	}

	return getStatus();
}

ControlLayer::Status ControlLayer::shutdown()
{
	return getStatus();
}

// =============================================================================
// Class Implementation Functions
// =============================================================================
// -----------------------------------------------------------------------------
// Controllers
// -----------------------------------------------------------------------------
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

	TODO("shutdown might need to be called stop or pause or whatever. Maybe add another virtual function in AppLayerComponent")
	it->second->stop();
	return true;
}

void ControlLayer::loadController(const Controller::ID_t &name)
{
	std::shared_ptr<Controller> pController = ControlLayer::loadClass<Controller>(name);
	std::lock_guard<std::mutex> lock(this->controllers_mutex);

	TODO("Define properly what this function does when a controller already exists")
	this->addController(pController);
}

// -----------------------------------------------------------------------------
// Gait Generators
// -----------------------------------------------------------------------------
bool ControlLayer::activateGaitGenerator(const GaitGenerator::ID_t &ID)
{
	std::lock_guard<std::mutex> lock(this->gait_generators_mutex);
	auto it = this->generators.find(ID);

	if(it == this->generators.end()) return false;

	if(currentActiveGenerator)
	{
		currentActiveGenerator->stop();
	}
	it->second->run();
	return true;
}

void ControlLayer::deactivateGaitGenerators()
{
	if(currentActiveGenerator) currentActiveGenerator->stop();
}

void ControlLayer::loadGaitGenerator(const std::string &name)
{
	std::shared_ptr<GaitGenerator> pGaitGenerator =
		ControlLayer::loadClass<GaitGenerator>(name);
	std::lock_guard<std::mutex> lock(this->gait_generators_mutex);
	TODO("define properly what this function does when a controller already exists")
	this->addGaitGenerator(pGaitGenerator);
}
