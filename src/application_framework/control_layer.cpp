#include "application_framework/control_layer.hpp"
#include <Eigen/Dense>
#include <tuple>

#include "util/debug/debug.hpp"
#include "robot/robot.hpp"
#include "util/class_loader.hpp"
#include "msg/control_signalPubSubTypes.h"


// =============================================================================
// Constructors
// =============================================================================
ControlLayer::ControlLayer() :
	controllers_b(),
	controllers_mutex_b(),
	generators(),
	gait_generators_mutex(),
	currentActiveGenerator(nullptr),
	active_generator_thread(),
	publisher("desired_torques")
{ }

ControlLayer::~ControlLayer()
{ }

// =============================================================================
// Interface Override Functions
// =============================================================================
ControlLayer::Status ControlLayer::run()
{

	TODO("spawn nonrealtime thread for user interaction")

	TODO("Check status of all components in the control layer, take corrective actions if requred")
	setStatus(Status::RUNNING);
	TODO("correct looping condition")
	while(getStatus() == Status::RUNNING)
	{
		DMSG("==============Control layer loop==========");
		// Read the control signals
		Eigen::VectorXd desired_torques = Eigen::VectorXd::Zero(Robot::getDimension());
		{
			DMSG("ABOUT TO GRAB MUTEX");
			std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
			DMSG("GOT MUTEX");
			for(const auto &pair : this->controllers_b)
			{
				if(!pair.second.pController) continue;
				if(pair.second.pController->getStatus() == Controller::Status::RUNNING)
				{
					if(!pair.second.pSubscriber) continue;
					auto pControl_signal = pair.second.pSubscriber->getLastPublishedControlSignal();
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
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(500));
	}

	DMSG("Fell out of control loop!!!!!!!!!!!!!!!!!!!!!!!!!!");

	return getStatus();
}

ControlLayer::Status ControlLayer::shutdown()
{
	DMSG("shutdown control layer");
	setStatus(Status::STOP);
	deactivateGaitGenerators();

	{
		// Tell each controller to stop
		std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
		for(auto &pair : this->controllers_b)
		{
			pair.second.pController->stop();
			if(pair.second.pExecution_thread) pair.second.pExecution_thread->join();
		}
	}

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
	DMSG("ACTIVATE CONTROLLER");
	std::lock_guard<std::mutex> lock(this->controllers_mutex_b);

	// find the controller in the list of controllers
	auto pair_it = this->controllers_b.find(ID);
	TODO("Inform the user that the controller does not exist")
	if(pair_it == this->controllers_b.end()) return false;

	// check that the controller is not already running
	TODO("inform the user that the controlelr is already running")
	if(pair_it->second.pExecution_thread) return false;

	// Start the controller in a new thread
	DMSG("about to start new thread for controller");
	AppLayerComponent::Status (Controller::*run_p)() = &Controller::run;
	pair_it->second.pExecution_thread = std::make_shared<std::thread>(run_p, pair_it->second.pController.get());

	// start the subscriber
	pair_it->second.pSubscriber = std::make_shared<ControlSubListener>(pair_it->second.pController->getControlSignalTopic());

	DMSG("finished calling run on controller from control layer");
	return true;
}

bool ControlLayer::deactivateController(const Controller::ID_t &ID)
{
	DMSG("DEACTIVATE CONTROLLER");
	std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
	auto pair_it = this->controllers_b.find(ID);

	TODO("Inform user that the controller does not exist")
	if(pair_it == this->controllers_b.end()) return false;

	// check whether the controller is not already running
	TODO("inform the user that the controller is not running")
	if(pair_it->second.pExecution_thread == nullptr) return false;

	TODO("shutdown might need to be called stop or pause or whatever. Maybe add another virtual function in AppLayerComponent")
	pair_it->second.pController->stop();
	pair_it->second.pExecution_thread->join();
	pair_it->second.pExecution_thread = nullptr;
	pair_it->second.pSubscriber = nullptr;

	return true;
}

void ControlLayer::loadController(const Controller::ID_t &name)
{
	DMSG("LOAD CONTROLLER");
	std::shared_ptr<Controller> pController = ClassLoader::loadClass<Controller>(name);
	// std::lock_guard<std::mutex> lock(this->controllers_mutex_b);

	TODO("Define properly what this function does when a controller already exists")
	this->addController(pController);
}

// -----------------------------------------------------------------------------
// Gait Generators
// -----------------------------------------------------------------------------
TODO("this is a bit copy-pasty from the activate controller version")
bool ControlLayer::activateGaitGenerator(const GaitGenerator::ID_t &ID)
{
	std::lock_guard<std::mutex> lock(this->gait_generators_mutex);

	auto it = this->generators.find(ID);

	if(it == this->generators.end()) return false;

	if(this->currentActiveGenerator)
	{
		this->currentActiveGenerator->stop();
		this->active_generator_thread.join();
		DMSG("STOPPING PREVIOUS GENERATOR");
	}

	this->currentActiveGenerator=it->second;
	AppLayerComponent::Status (GaitGenerator::*run_p)() = &GaitGenerator::run;
	std::thread t(run_p, &*it->second);
	this->active_generator_thread.swap(t);

	return true;
}

void ControlLayer::deactivateGaitGenerators()
{
	if(this->currentActiveGenerator)
	{
		this->currentActiveGenerator->stop();
		this->active_generator_thread.join();
	}
	this->currentActiveGenerator = nullptr;
}

void ControlLayer::loadGaitGenerator(const std::string &name)
{
	std::shared_ptr<GaitGenerator> pGaitGenerator =
		ClassLoader::loadClass<GaitGenerator>(name);
	TODO("define properly what this function does when a gait generator already exists")
	this->addGaitGenerator(pGaitGenerator);
	DMSG("EXIT");
}

Eigen::MatrixXd ControlLayer::saturateTorques(const Eigen::MatrixXd &req) const
{
	TODO("This is not implemented yet")
	TODO("Move this to the robot class")
	return req;
}

void ControlLayer::publishDesiredTorques(const Eigen::VectorXd &torques) const
{

	DesiredTorquesMsg msg;
	msg.desired_torques().resize(torques.size());
	Eigen::VectorXd::Map(&msg.desired_torques()[0], torques.size()) = torques;
	publisher.publish(msg);
}

// =============================================================================
// FastRTPS
// =============================================================================
ControlLayer::ControlSubListener::ControlSubListener(const std::string &topic):
	SubscriberBase<ControlSignalMsgPubSubType>(topic),
	// SubscriberBase<HelloWorldPubSubType>(topic),
	control_signal(nullptr),
	control_signal_mutex()
{ }

std::shared_ptr<ControlSignal>
	ControlLayer::ControlSubListener::getLastPublishedControlSignal()
{
	std::lock_guard<std::mutex> lock(this->control_signal_mutex);
	return this->control_signal;
}

void ControlLayer::ControlSubListener::onNewDataMessage
(
	eprosima::fastrtps::Subscriber *sub
)
{
	ControlSignalMsg msg;
	if(sub->takeNextData((void*)&msg, &info))
	{
		if(info.sampleKind == eprosima::fastrtps::rtps::ALIVE)
		{
			std::shared_ptr<ControlSignal> p = std::make_shared<ControlSignal>(msg);
			std::lock_guard<std::mutex> lock(this->control_signal_mutex);
			this->control_signal = p;
		}
		DMSG("@%&!#@%*#@!^#(! GOT A MESSAGE @!*#^!@^#*!");
	}
}
