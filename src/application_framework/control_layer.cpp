#include "application_framework/control_layer.hpp"
#include <Eigen/Dense>
#include <tuple>

#include "util/debug/debug.hpp"
#include "robot/robot.hpp"
#include "util/class_loader.hpp"

// eprosima
#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/Domain.h>
#include "msg/control_signalPubSubTypes.h"


// =============================================================================
// Constructors
// =============================================================================
ControlLayer::ControlLayer() :
	controllers(),
	active_controller_threads(),
	controllers_mutex(),
	generators(),
	gait_generators_mutex(),
	currentActiveGenerator(nullptr),
	active_generator_thread()
{ }

ControlLayer::~ControlLayer()
{ }

// =============================================================================
// Interface Override Functions
// =============================================================================
ControlLayer::Status ControlLayer::run()
{
	DMSG("Running control layer");
	// {
		TODO("remove this. also from applayer base class")
	// 	std::lock_guard<std::mutex> lock(this->components_mutex);
	// }

	TODO("spawn nonrealtime thread for user interaction")

	TODO("Check status of all components in the control layer, take corrective actions if requred")
	setStatus(Status::RUNNING);
	TODO("correct looping condition")
	while(getStatus() == Status::RUNNING)
	{
		DMSG("==============Control layer loop==========");
		// Read the gait signal from the current active gait generator
		decltype(currentActiveGenerator->readSignal()) pSignal;
		{
			// Read the reference signal from the gait generator
			std::lock_guard<std::mutex> lock(this->gait_generators_mutex);
			if(this->currentActiveGenerator)
			{
				DMSG("Reading Signal from Gait Generator");
				pSignal = this->currentActiveGenerator->readSignal();
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
						TODO("handle Impulse here")
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
		std::lock_guard<std::mutex> lock(this->controllers_mutex);
		for(auto it = this->controllers.begin(); it != this->controllers.end(); ++it)
		{
			it->second->stop();
		}

		// Join each controller's thread
		for(auto it = this->active_controller_threads.begin(); it != this->active_controller_threads.end(); ++it)
		{
			it->second.join();
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
	DMSG("about to start new thread for controller");
	AppLayerComponent::Status (Controller::*run_p)() = &Controller::run;
	this->active_controller_threads.emplace
	(
		std::piecewise_construct,
		std::forward_as_tuple(ID),
		std::forward_as_tuple(run_p, &*controller_it->second)
	);

	// start the controller subscriber
	{
		std::lock_guard<std::mutex> lock(this->control_subscribers_mutex);
		this->control_subscribers.emplace
		(
			std::piecewise_construct,
			std::forward_as_tuple(ID),
			std::forward_as_tuple(ID)
		);
	}

	DMSG("finished calling run on controller from control layer");
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

	// stop the controller subscriber
	{
		std::lock_guard<std::mutex> lock(this->control_subscribers_mutex);
		this->control_subscribers.erase(ID);
	}
	return true;
}

void ControlLayer::loadController(const Controller::ID_t &name)
{
	std::shared_ptr<Controller> pController = ClassLoader::loadClass<Controller>(name);
	std::lock_guard<std::mutex> lock(this->controllers_mutex);

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

void ControlLayer::publishDesiredTorques(const Eigen::MatrixXd &) const
{
	TODO("This is not yet implemented")
}

// =============================================================================
// FastRTPS
// =============================================================================
// ControlSignalMsgPubSubType ControlLayer::ControlSubListener::rtps_type;
HelloWorldPubSubType ControlLayer::ControlSubListener::rtps_type;

ControlLayer::ControlSubListener::ControlSubListener(const Controller::ID_t &id):
	control_signal(nullptr),
	control_signal_mutex(),
	pParticipant(nullptr),
	pSubscriber(nullptr)
{
	eprosima::fastrtps::ParticipantAttributes participant_attr;
	participant_attr.rtps.setName("Participant_subscriber");

	TODO("figure out how to remove participant properly")
	pParticipant.reset
	(
		eprosima::fastrtps::Domain::createParticipant(participant_attr),
		[](eprosima::fastrtps::Participant*){}
	);
	eprosima::fastrtps::Domain::registerType
	(
		pParticipant.get(),
		static_cast<eprosima::fastrtps::TopicDataType*>(&rtps_type)
	);

	eprosima::fastrtps::SubscriberAttributes sub_attr;
	sub_attr.topic.topicKind = eprosima::fastrtps::rtps::NO_KEY;
	sub_attr.topic.topicDataType = rtps_type.getName();

	TODO("do this better")
	// build topic name
	sub_attr.topic.topicName = "HelloWorldPubSubTopic";
	// sub_attr.topic.topicName = id;
	TODO("figure out how to remove this subscriber")
	pSubscriber.reset
	(
		eprosima::fastrtps::Domain::createSubscriber
		(
			pParticipant.get(),
			sub_attr,
			static_cast<eprosima::fastrtps::SubscriberListener*>(this)
		),
		[](eprosima::fastrtps::Subscriber*){}
	);

	DMSG("#$%^&^%$%^&^%$%^&%$# CREATED A SUBSCRIBER I HOPE #$%^&^%$%^&^%$%^&%$#");
}

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
	DMSG("#$%^&^%$%^&^%$%^&%$# GOT A MESSAGE #$%^&^%$%^&^%$%^&%$#");

	TODO("this is wrong, temporary code")
	std::shared_ptr<ControlSignal> p = std::make_shared<ControlSignal>();
	std::lock_guard<std::mutex> lock(this->control_signal_mutex);
	control_signal = p;
}
