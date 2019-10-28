#include "application_framework/control_layer.hpp"
TODO("remove temp iostream include")
#include <iostream>
#include <Eigen/Dense>
#include <tuple>

#include "robot/robot.hpp"
// =============================================================================
// Constructors
// =============================================================================
ControlLayer::ControlLayer() :
	controllers(),
	active_controller_threads(),
	controllers_mutex(),
	generators(),
	gait_generators_mutex(),
	currentActiveGenerator(nullptr)
{ }

ControlLayer::~ControlLayer()
{
	std::lock_guard<std::mutex> lock(this->controllers_mutex);
	for(auto it = this->controllers.begin(); it != this->controllers.end(); ++it)
	{
		it->second->stop();
	}
	for(auto it = this->active_controller_threads.begin(); it != this->active_controller_threads.end(); ++it)
	{
		it->second.join();
	}
}

// =============================================================================
// Interface Override Functions
// =============================================================================
ControlLayer::Status ControlLayer::run()
{
	std::cout << "Running control layer" << std::endl;
	{
		TODO("remove this. also from applayer base class")
		std::lock_guard<std::mutex> lock(this->components_mutex);
	}

	TODO("spawn nonrealtime thread for user interaction")

	TODO("Check status of all components in the control layer, take corrective actions if requred")
	setStatus(Status::RUNNING);
	while(getStatus() == Status::RUNNING)
	{
		// Read the gait signal from the current active gait generator
		decltype(currentActiveGenerator->readSignal()) pSignal;
		{
			// Read the reference signal from the gait generator
			std::lock_guard<std::mutex> lock(this->gait_generators_mutex);
			if(currentActiveGenerator)
			{
				pSignal = currentActiveGenerator->readSignal();
			}
		}

		// Send the reference signal to all active controllers
		Eigen::MatrixXd desired_torques = Eigen::MatrixXd::Zero(Robot::getDimension(), 1);
		{
			std::lock_guard<std::mutex> lock(controllers_mutex);
			for(const auto &pair_id_pController : this->controllers)
			{
				TODO("make sure that this check is effective")
				if(!pair_id_pController.second) continue;
				if(pair_id_pController.second->getStatus() == Controller::Status::RUNNING)
				{
					// send the current signal of the gait generator
					if(pSignal)
					{
						// Signal may be nullptr on first run of the gait generator
						pair_id_pController.second->pushSignal(pSignal);
					}

					// read the last control command and sum it
					auto pControl_signal = pair_id_pController.second->readSignal();
					if(pControl_signal)
					{
						desired_torques += pControl_signal->torques;
					}
				}
			}
		}

		// Send the desired torques to HAL
		publishDesiredTorques(saturateTorques(desired_torques));

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
TODO("std::map already does this check for emplace, maybe for others. Double check and make this more efficient")
bool ControlLayer::activateController(const Controller::ID_t &ID)
{
	// Find the controller in the list of controllers
	std::lock_guard<std::mutex> lock(this->controllers_mutex);
	auto controller_it = this->controllers.find(ID);

	TODO("Inform user that the controller does not exist")
	if(controller_it == this->controllers.end()) return false;

	// Check that the controller is not already running
	auto controller_thread_it = this->active_controller_threads.find(ID);
	TODO("inform the user that the controlelr is already running")
	if(controller_thread_it != this->active_controller_threads.end()) return false;

	// Start the controller in a new thread
	std::cout << "about to start new thread for controller" << std::endl;
	AppLayerComponent::Status (Controller::*run_p)() = &Controller::run;
	this->active_controller_threads.emplace
	(
		std::piecewise_construct,
		std::forward_as_tuple(ID),
		std::forward_as_tuple(run_p, &*controller_it->second)
	);

	std::cout << "finished calling run on controller from control layer" << std::endl;
	std::cout << "started thread for controller" << std::endl;
	return true;
}

bool ControlLayer::deactivateController(const Controller::ID_t &ID)
{
	std::lock_guard<std::mutex> lock(this->controllers_mutex);
	auto controller_it = this->controllers.find(ID);

	TODO("Inform user that the controller does not exist")
	if(controller_it == this->controllers.end()) return false;

	// Check whether the controller is not already running
	auto controller_thread_it = this->active_controller_threads.find(ID);
	TODO("inform the user that the controller is not running")
	if(controller_thread_it != this->active_controller_threads.end()) return false;

	TODO("shutdown might need to be called stop or pause or whatever. Maybe add another virtual function in AppLayerComponent")
	controller_it->second->stop();
	controller_thread_it->second.join();
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

Eigen::MatrixXd ControlLayer::saturateTorques(const Eigen::MatrixXd &req) const
{
	TODO("This is not implemented yet")
	TODO("Move this to the robot class")
	return req;
}

void ControlLayer::publishDesiredTorques(const Eigen::MatrixXd &) const
{
	TODO("This is not yet implemented")
}
