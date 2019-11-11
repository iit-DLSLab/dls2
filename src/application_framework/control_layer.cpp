#include "application_framework/control_layer.hpp"
#include <Eigen/Dense>
#include <tuple>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "util/debug/debug.hpp"
#include "robot/robot.hpp"
#include "util/class_loader.hpp"
#include "msg/control_signalPubSubTypes.h"
#include "topics/desired_torques.hpp"
#include "util/log/log.hpp"

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
	publisher(topics::desired_torques),
	// num_children(0),
	// num_children_cv(),
	// num_children_mutex(),
	should_quit(false),
	wait_on_controller_threads()
{ }

ControlLayer::~ControlLayer()
{
	DMSG("Destructor");

	// this->should_quit = true;
	// this->num_children_cv.notify_one();
	// DMSG("notified");
	// this->wait_on_controllers_thread.join();
	// DMSG("joined");
}

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
		logging::cdbg << "Control Layer loop" << logging::endl;

		DMSG("CL loop");
		// Read the control signals
		Eigen::VectorXd desired_torques = Eigen::VectorXd::Zero(Robot::getDimension());
		{
			std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
			for(const auto &pair : this->controllers_b)
			{
				// if(!pair.second.pController) continue;
				// if(pair.second.pController->getStatus() == Controller::Status::RUNNING)
				// {
				if(!pair.second->pSubscriber) continue;
				auto pControl_signal = pair.second->pSubscriber->getLastPublishedControlSignal();
				if(pControl_signal)
				{
					DMSG("got a control signal");
					desired_torques += pControl_signal->torques;
				}
				// }
			}
		}

		// Send the desired torques to HAL
		publishDesiredTorques(saturateTorques(desired_torques));

		TODO("sleep at correct frequency here")
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(500));
	}

	return getStatus();
}

ControlLayer::Status ControlLayer::shutdown()
{
	setStatus(Status::STOP);
	deactivateGaitGenerators();

	// Stop all controllers
	{
		std::unique_lock<std::mutex> lock(this->controllers_mutex_b);
		// while(controllers_b.size() != 0)
		// {
		// 	Controller::ID_t ID = controllers_b.begin()->second->ID;
		// 	lock.unlock(); // TODO fix this
		// 	deactivateController(ID);
		// 	lock.lock();
		// }
		for(auto &pair : this->controllers_b)
		{
			deactivateController(pair.second);
		}
	}

	// Join all dangling threads
	{
		std::lock_guard<std::mutex> lock(this->wait_on_controller_threads_mutex);
		for(auto &thread : this->wait_on_controller_threads)
		{
			thread.join();
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
	pid_t controller_pid;
	auto pData = std::make_shared<ControllerData>();
	{
		std::lock_guard<std::mutex> lock(this->controllers_mutex_b);

		// find the controller in the list of controllers
		auto pair_it = this->controllers_b.find(ID);
		TODO("Inform the user that the controller already exists")
		if(pair_it != this->controllers_b.end()) return false;

		if((controller_pid = fork()) == 0)
		{
			execl("controller_process", ID.c_str(), ID.c_str(), (char *)NULL);
			TODO("handle errors");
			_exit(0);
		}

		pData->controller_pid = controller_pid;
		pData->ID = ID;
		pData->pSubscriber =
			std::make_shared<ControlSubListener>
			(
				TODO("This should be done by a static function in Controller")
				std::string("control_signal_") + ID
			);

		DMSG("emplace");
		this->controllers_b.emplace
		(
			std::pair<Controller::ID_t, std::shared_ptr<ControllerData>>
			(
				ID,
				pData
			)
		);
		DMSG("Done emplace");
	}

	// {
	// 	std::lock_guard<std::mutex> lock(this->num_children_mutex);
	// 	++(this->num_children);
	// 	this->num_children_cv.notify_one();
	// }

	{
		std::lock_guard<std::mutex> lock(this->wait_on_controller_threads_mutex);
		this->wait_on_controller_threads.emplace_back
		(
			&ControlLayer::waitOnChildController,
			this,
			pData
		);
	}

	return true;
	// std::lock_guard<std::mutex> lock(this->controllers_mutex_b);

	// // find the controller in the list of controllers
	// auto pair_it = this->controllers_b.find(ID);
	// TODO("Inform the user that the controller does not exist")
	// if(pair_it == this->controllers_b.end()) return false;

	// // check that the controller is not already running
	// TODO("inform the user that the controlelr is already running")
	// if(pair_it->second.pExecution_thread) return false;

	// // Start the controller in a new thread
	// AppLayerComponent::Status (Controller::*run_p)() = &Controller::run;
	// pair_it->second.pExecution_thread = std::make_shared<std::thread>(run_p, pair_it->second.pController.get());

	// // start the subscriber
	// pair_it->second.pSubscriber = std::make_shared<ControlSubListener>(pair_it->second.pController->getControlSignalTopic());

	// return true;
}

void ControlLayer::deactivateController(std::shared_ptr<ControllerData> pData)
{
	kill(pData->controller_pid, SIGTERM);
}

bool ControlLayer::deactivateController(const Controller::ID_t &ID)
{
	std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
	auto pair_it = this->controllers_b.find(ID);

	TODO("Inform user that the controller does not exist")
	if(pair_it == this->controllers_b.end()) return false;

	// tell the process to stop -- waitOnChildControllers will
	// wait on it and join it as required
	kill(pair_it->second->controller_pid, SIGTERM);

	// TODO wait until process is joined

	// remove the process data
	// DMSG("removing");
	// this->controllers_b.erase(pair_it);

	return true;
}

TODO("remove this function")
void ControlLayer::loadController(const Controller::ID_t &name)
{
	DMSG("Deprecated function call does nothing");
	// std::shared_ptr<Controller> pController = ClassLoader::loadClass<Controller>(name);
	// // std::lock_guard<std::mutex> lock(this->controllers_mutex_b);

	// TODO("Define properly what this function does when a controller already exists")
	// this->addController(pController);
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
	}
}

// =============================================================================
// Fork
// =============================================================================
void ControlLayer::waitOnChildController(std::shared_ptr<ControllerData> pData)
{
	DMSG("wait Thread launched");
	int status;

	TODO("Handle errors, relaunching etc")
	pid_t child_pid = waitpid(pData->controller_pid, &status, 0);
	DMSG("CHILD : " << pData->controller_pid << " pid " << child_pid << " exited");
	{
		std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
		auto it = this->controllers_b.find(pData->ID);
		if(it != this->controllers_b.end())
		{
			DMSG("remove controller data");
			this->controllers_b.erase(it);
		}
	}
	DMSG("wait thread exit");
}
