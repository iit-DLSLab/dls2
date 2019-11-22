/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
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
#include "path_prefixes/path_prefixes.hpp"

#include <string.h>
#include <errno.h>

// =============================================================================
// Constructors
// =============================================================================
dls::ControlLayer::ControlLayer() :
	controllers_b(),
	controllers_mutex_b(),
	// generators(),
	// gait_generators_mutex(),
	// currentActiveGenerator(nullptr),
	pGait_generator_data(nullptr),
	gait_generators_mutex(),
	publisher(topics::desired_torques),
	// num_children(0),
	// num_children_cv(),
	// num_children_mutex(),
	should_quit(false),
	wait_on_controller_threads(),
	wait_on_controller_threads_mutex(),

	TODO("These are temporary until a proper console is developed")
	activate_gait_generator_listener(topics::activate_gait_generator, *this),
	deactivate_gait_generator_listener(topics::deactivate_gait_generator, *this),
	activate_controller_listener(topics::activate_controller, *this),
	deactivate_controller_listener(topics::deactivate_controller, *this)
{ }

dls::ControlLayer::~ControlLayer()
{
	// this->should_quit = true;
	// this->num_children_cv.notify_one();
	// DMSG("notified");
	// this->wait_on_controllers_thread.join();
	// DMSG("joined");
}

// =============================================================================
// Interface Override Functions
// =============================================================================
dls::ControlLayer::Status dls::ControlLayer::run()
{

	TODO("spawn nonrealtime thread for user interaction")

	TODO("Check status of all components in the control layer, take corrective actions if requred")
	setStatus(Status::RUNNING);
	TODO("correct looping condition")
	while(getStatus() == Status::RUNNING)
	{
		// Read the control signals
		Eigen::VectorXd desired_torques = Eigen::VectorXd::Zero(Robot::getDimension());
		{
			std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
			for(const auto &pair : this->controllers_b)
			{
				if(!pair.second->pSubscriber) continue;
				auto pControl_signal = pair.second->pSubscriber->getLastPublishedControlSignal();
				if(pControl_signal)
				{
					desired_torques += pControl_signal->torques;
				}
			}
		}

		// Send the desired torques to HAL
		publishDesiredTorques(saturateTorques(desired_torques));

		TODO("sleep at correct frequency here")
		std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(500));
	}

	return getStatus();
}

dls::ControlLayer::Status dls::ControlLayer::shutdown()
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
bool dls::ControlLayer::activateController(const Controller::ID_t &ID)
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
			execl(CHILD_PROCESS_PATH "dls_controller_process", ID.c_str(), ID.c_str(), (char *)NULL);
			logging::cfatal << "Controller process failed to launch" << logging::endl;
			DMSG(strerror(errno));
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

		this->controllers_b.emplace
		(
			std::pair<Controller::ID_t, std::shared_ptr<ControllerData>>
			(
				ID,
				pData
			)
		);
	}

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
}

void dls::ControlLayer::deactivateController(std::shared_ptr<ControllerData> pData)
{
	kill(pData->controller_pid, SIGTERM);
}

bool dls::ControlLayer::deactivateController(const Controller::ID_t &ID)
{
	std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
	auto pair_it = this->controllers_b.find(ID);

	TODO("Inform user that the controller does not exist")
	if(pair_it == this->controllers_b.end()) return false;

	// tell the process to stop -- waitOnChildControllers will
	// wait on it and join it as required
	kill(pair_it->second->controller_pid, SIGTERM);

	return true;
}

TODO("remove this function")
void dls::ControlLayer::loadController(const Controller::ID_t &name)
{
	DMSG("Deprecated function call does nothing");
	name.c_str();
}

// -----------------------------------------------------------------------------
// Gait Generators
// -----------------------------------------------------------------------------
bool dls::ControlLayer::activateGaitGenerator(const GaitGenerator::ID_t &ID)
{
	std::lock_guard<std::mutex> lock(this->gait_generators_mutex);

	// if a gait generator is already running
	if(this->pGait_generator_data && this->pGait_generator_data->gait_generator_pid != 0)
	{
		logging::cout << "A gait generator is already running" << logging::endl;
		return false;
	}
	// launch a gait generator and register it with the architecture
	this->pGait_generator_data = std::make_shared<GaitGeneratorData>();
	this->pGait_generator_data->gait_generator_pid = fork();
	if(this->pGait_generator_data->gait_generator_pid == 0)
	{
		TODO("error checking")
		execl(CHILD_PROCESS_PATH "dls_gait_generator_process", ID.c_str(), ID.c_str(), (char *)NULL);
		logging::cfatal << "Failed to launch gait generator" << logging::endl;
		DMSG(strerror(errno));
	}

	// start the wait thread -- it will remove the gait generator data when done
	std::thread t(&ControlLayer::waitOnChildGaitGenerator, this, this->pGait_generator_data);
	t.detach();

	return true;
}

void dls::ControlLayer::deactivateGaitGenerators()
{
	std::lock_guard<std::mutex> lock(this->gait_generators_mutex);
	if(this->pGait_generator_data || this->pGait_generator_data->gait_generator_pid != 0)
	{
		kill(this->pGait_generator_data->gait_generator_pid, SIGTERM);
	}
}

TODO("remove this function")
void dls::ControlLayer::loadGaitGenerator(const std::string &name)
{
	DMSG("Deprecated function call does nothing");
	name.c_str();
	// std::shared_ptr<GaitGenerator> pGaitGenerator =
	// 	ClassLoader::loadClass<GaitGenerator>(name);
	// TODO("define properly what this function does when a gait generator already exists")
	// this->addGaitGenerator(pGaitGenerator);
}

Eigen::MatrixXd dls::ControlLayer::saturateTorques(const Eigen::MatrixXd &req) const
{
	TODO("This is not implemented yet")
	TODO("Move this to the robot class")
	return req;
}

void dls::ControlLayer::publishDesiredTorques(const Eigen::VectorXd &torques) const
{

	DesiredTorquesMsg msg;
	msg.desired_torques().resize(torques.size());
	Eigen::VectorXd::Map(&msg.desired_torques()[0], torques.size()) = torques;
	publisher.publish(msg);
}

// =============================================================================
// FastRTPS
// =============================================================================
dls::ControlLayer::ControlSubListener::ControlSubListener(const std::string &topic):
	SubscriberBase<ControlSignalMsgPubSubType>(topic),
	control_signal(nullptr),
	control_signal_mutex(),
	info()
{ }

std::shared_ptr<dls::ControlSignal>
	dls::ControlLayer::ControlSubListener::getLastPublishedControlSignal()
{
	std::lock_guard<std::mutex> lock(this->control_signal_mutex);
	return this->control_signal;
}

void dls::ControlLayer::ControlSubListener::onNewDataMessage
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
void dls::ControlLayer::waitOnChildController(std::shared_ptr<ControllerData> pData)
{
	int status;

	TODO("Handle errors, relaunching etc")
	/*pid_t child_pid = */waitpid(pData->controller_pid, &status, 0);
	// DMSG("CHILD : " << pData->controller_pid << " pid " << child_pid << " exited");
	if(WIFSIGNALED(status))
	{
		std::cout << "child controller exited by signal" << std::endl;
		if(WCOREDUMP(status))
		{
			std::cout << "child controller had a core dump" << std::endl;
		}
	}
	{
		std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
		auto it = this->controllers_b.find(pData->ID);
		if(it != this->controllers_b.end())
		{
			this->controllers_b.erase(it);
		}
	}
}

void dls::ControlLayer::waitOnChildGaitGenerator(std::shared_ptr<GaitGeneratorData> pData)
{
	int status;
	/*pid_t child_pid =*/ waitpid(pData->gait_generator_pid, &status, 0);
	if(WIFSIGNALED(status))
	{
		std::cout << "child gait generator exited by signal" << std::endl;
		if(WCOREDUMP(status))
		{
			std::cout << "child gait generator had a core dump" << std::endl;
		}
	}
	// DMSG("child gait generator " << child_pid << " exited");
	{
		std::lock_guard<std::mutex> lock(this->gait_generators_mutex);
		pGait_generator_data = nullptr;
	}
}
