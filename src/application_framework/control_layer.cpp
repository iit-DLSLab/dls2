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
#include "path_prefixes/path_prefixes.hpp"
#include "util/time/time.hpp"

#include <string.h>
#include <errno.h>
// TODO  temp include
#include <sstream>

#ifndef NDEBUG
#include <sstream>
#include <chrono>
#include "util/log/log.hpp"
#endif

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
ControlLayer::ControlLayer() :
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

	// TODO these 2s should only be put once, and they should be made dynamic
	pDefault_spline_in
	(
		// ramp for two seconds from 0 to 1
		std::make_shared<spline::Ramp<double>> (0, 2, 0, 1)
	),
	pDefault_spline_out
	(
		// ramp for two seconds from 1 to 0
		std::make_shared<spline::Ramp<double>> (0, 2, 1, 0)
	),
	default_duration_seconds(2),

	command_manager(),
	TODO("These are temporary until a proper console is developed")
	// activate_gait_generator_listener(topics::activate_gait_generator, *this),
	// deactivate_gait_generator_listener(topics::deactivate_gait_generator, *this),
	// activate_controller_listener(topics::activate_controller, *this),
	// deactivate_controller_listener(topics::deactivate_controller, *this),
	scout("control_layer"),
	clog("control_layer"),
	cfatal("control_layer")
{
	command_manager.addCommand<bool, std::string>
	(
		"control_layer",
		"activateGaitGenerator",
		"activates a gait generator",
		std::function<bool(std::string)>
		(
			[&](std::string s)->bool
			{
				return this->activateGaitGenerator(s);
			}
		)
	);

	command_manager.addCommand<bool, std::string>
	(
		"control_layer",
		"activateController",
		"activates a controller",
		std::function<bool(std::string s)>
		(
			[&](const std::string &s)->bool
			{
				return this->activateController(s);
			}
		)
	);

	// TODO template system can't currently handle no arguments -- this is just
	// a temporary workaround by passing an unnecessary string
	command_manager.addCommand<void, std::string>
	(
		"control_layer",
		"deactivateGaitGenerators",
		"stops all running gait generators",
		std::function<void(std::string)>
		(
			[&](std::string)
			{
				this->deactivateGaitGenerators();
			}
		)
	);

	command_manager.addCommand<void, std::string>
	(
		"control_layer",
		"deactivateController",
		"stops a controller",
		std::function<void(std::string)>
		(
			[&](std::string s)
			{
				this->deactivateController(s);
			}
		)
	);

	command_manager.addCommand<void, std::string>
	(
		"control_layer",
		"ls",

		"List components\n"
		"argument:\n"
		"\tcont - list controllers\n"
		"\tgait - list gait generator",

		std::function<void(std::string)>
		(
			[&](std::string s)
			{
				if(s == "cont")
				{
					std::stringstream ss;
					std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
					size_t i = 0;
					for
					(
						auto it = this->controllers_b.cbegin();
						it != this->controllers_b.cend();
						++it
					)
					{
						ss << it->first << "\n";
						++i;
					}
					if(i) this->scout << ss.str() << std::endl;
				}
				else if(s == "gait")
				{
					std::lock_guard<std::mutex> lock(this->gait_generators_mutex);
					if(this->pGait_generator_data)
					{
						scout << this->pGait_generator_data->gait_generator_pid
							<< std::endl;
					}
				}
				else
				{
					scout << "Error: unrecognised argument" << std::endl;
				}
			}
		)
	);
}

ControlLayer::~ControlLayer()
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
ControlLayer::Status ControlLayer::run()
{
	// TODO THIS NEEDS to be moved to the library initialisation of hyq_app.
	// However, there is something going wrong with the fastrtps subscriber if
	// it is done there. For now, this call has been placed here
	DMSG("using simulated time");
	Time::set_use_simulated_time(true);

	TODO("spawn nonrealtime thread for user interaction")

	TODO("Check status of all components in the control layer, take corrective actions if requred")
	setStatus(Status::RUNNING);
	TODO("correct looping condition")
	double time;
	while(getStatus() == Status::RUNNING)
	{
		#ifndef NDEBUG
			auto begin_epoch = std::chrono::system_clock::now();
		#endif
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
					desired_torques +=
						pair.second->premultiplier*pControl_signal->torques;
					time = pControl_signal->time; // TODO How to pick which time
				}
			}
		}

		// Send the desired torques to HAL
		publishDesiredTorques(saturateTorques(desired_torques),time);

		// std::stringstream ss;
		// ss << "Control layer published torques: " << desired_torques.transpose() << std::endl;
		// logging::cout << ss.str() << std::endl;

		TODO("sleep at correct frequency here")
		// std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(500));
		// DMSG(Time::now().time_since_epoch().count());
		// std::this_thread::sleep_for(std::chrono::duration<double, std::micro>(100));
		Time::sleep_until(Time::now() + std::chrono::duration<double, std::micro>(100));

		#ifndef NDEBUG
		{
			auto end_epoch = std::chrono::system_clock::now();
			double useconds =
				std::chrono::duration<double, std::ratio<1, 1'000'000>>
					(end_epoch - begin_epoch).count();

			std::stringstream ss;
			ss << "Control Layer has period "
				<< std::chrono::duration<double, std::ratio<1, 1'000'000>>(100).count() << " useconds. epoch ran in: " << useconds << " useconds "
				<< std::endl;
			clog << ss.str() << std::endl;
			clog << "Control layer published torques " << desired_torques.transpose() << std::endl;

		}
		#endif
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
	auto pData = std::make_shared<ControllerData>
	(
		this->pDefault_spline_in,
		this->pDefault_spline_out,
		default_duration_seconds,
		default_duration_seconds
	);
	decltype(this->controllers_b.find(ID)) pair_it;
	{
		std::lock_guard<std::mutex> lock(this->controllers_mutex_b);

		// find the controller in the list of controllers
		pair_it = this->controllers_b.find(ID);
		TODO("Inform the user that the controller already exists")
		if(pair_it != this->controllers_b.end()) return false;

		if((controller_pid = fork()) == 0)
		{
			// launch the controller
			//
			// since its premultiplier is initially set to zero, this will have
			// no effect on the robot until the spline in is run later
			execl(CHILD_PROCESS_PATH "dls_controller_process", ID.c_str(), ID.c_str(), (char *)NULL);
			cfatal << "Controller process failed to launch" << std::endl;
			DMSG(strerror(errno));
			TODO("handle errors");
			_exit(0);
		}

		pData->controller_pid = controller_pid;
		pData->ID = ID;
		pData->pSubscriber =
			std::make_shared<ControlSubListener>
			(
				// TODO("This should be done by a static function in Controller")
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

	// spline in the controller
	//
	// TODO this is copied from the spline out code. Refactor so that it is not
	// repeated
	if(pData->pSpline_in)
	{
		auto start_time = std::chrono::system_clock::now();
		auto end_time = start_time + pData->spline_in_duration;
		auto now = start_time;

		while(now < end_time)
		{
			double running_time =
				(pData->spline_in_duration - std::chrono::duration<double>(end_time - now)).count();

			pData->premultiplier = pData->pSpline_in->eval(running_time);
			now = std::chrono::system_clock::now();

		}
	}

	return true;
}

void ControlLayer::deactivateController(std::shared_ptr<ControllerData> pData)
{
	// spline down the controller
	if(pData->pSpline_out)
	{
		auto start_time = std::chrono::system_clock::now();
		auto end_time = start_time + pData->spline_out_duration;
		auto now = start_time;

		while(now < end_time)
		{

			double running_time =
				(pData->spline_out_duration - std::chrono::duration<double>(end_time - now)).count();

			pData->premultiplier = pData->pSpline_out->eval(running_time);
			now = std::chrono::system_clock::now();
		}
	}

	// tell the process to stop -- waitOnChildControllers will
	// wait on it and join it as required
	kill(pData->controller_pid, SIGTERM);
}

bool ControlLayer::deactivateController(const Controller::ID_t &ID)
{
	decltype(this->controllers_b.find(ID)) pair_it;
	{
		std::lock_guard<std::mutex> lock(this->controllers_mutex_b);
		pair_it = this->controllers_b.find(ID);

		TODO("Inform user that the controller does not exist")
		if(pair_it == this->controllers_b.end()) return false;
	}

	this->deactivateController(pair_it->second);

	return true;
}

TODO("remove this function")
void ControlLayer::loadController(const Controller::ID_t &name)
{
	DMSG("Deprecated function call does nothing");
	name.c_str();
}

// -----------------------------------------------------------------------------
// Gait Generators
// -----------------------------------------------------------------------------
bool ControlLayer::activateGaitGenerator(const GaitGenerator::ID_t &ID)
{
	std::lock_guard<std::mutex> lock(this->gait_generators_mutex);

	// if a gait generator is already running
	if(this->pGait_generator_data && this->pGait_generator_data->gait_generator_pid != 0)
	{
		std::cout << "A gait generator is already running" << std::endl;
		return false;
	}
	// launch a gait generator and register it with the architecture
	this->pGait_generator_data = std::make_shared<GaitGeneratorData>();
	this->pGait_generator_data->gait_generator_pid = fork();
	if(this->pGait_generator_data->gait_generator_pid == 0)
	{
		TODO("error checking")
		execl(CHILD_PROCESS_PATH "dls_gait_generator_process", ID.c_str(), ID.c_str(), (char *)NULL);
		cfatal << "Failed to launch gait generator" << std::endl;
		DMSG(strerror(errno));
	}

	// start the wait thread -- it will remove the gait generator data when done
	std::thread t(&ControlLayer::waitOnChildGaitGenerator, this, this->pGait_generator_data);
	t.detach();

	return true;
}

void ControlLayer::deactivateGaitGenerators()
{
	std::lock_guard<std::mutex> lock(this->gait_generators_mutex);
	if(this->pGait_generator_data || this->pGait_generator_data->gait_generator_pid != 0)
	{
		kill(this->pGait_generator_data->gait_generator_pid, SIGTERM);
	}
}

TODO("remove this function")
void ControlLayer::loadGaitGenerator(const std::string &name)
{
	DMSG("Deprecated function call does nothing");
	name.c_str();
	// std::shared_ptr<GaitGenerator> pGaitGenerator =
	// 	ClassLoader::loadClass<GaitGenerator>(name);
	// TODO("define properly what this function does when a gait generator already exists")
	// this->addGaitGenerator(pGaitGenerator);
}

Eigen::MatrixXd ControlLayer::saturateTorques(const Eigen::MatrixXd &req) const
{
	TODO("This is not implemented yet")
	TODO("Move this to the robot class")
	return req;
}

void ControlLayer::publishDesiredTorques(const Eigen::VectorXd &torques, double time) const // TODO Change type of time
{

	DesiredTorquesMsg msg;
	msg.desired_torques().resize(torques.size());
	msg.header().time().seconds()=time;
	Eigen::VectorXd::Map(&msg.desired_torques()[0], torques.size()) = torques;
	publisher.publish(msg);
}

// =============================================================================
// FastRTPS
// =============================================================================
ControlLayer::ControlSubListener::ControlSubListener(const std::string &topic):
	SubscriberBase<ControlSignalMsgPubSubType>(topic),
	control_signal(nullptr),
	control_signal_mutex(),
	info()
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
// TODO spline out controller here too if necessary
void ControlLayer::waitOnChildController(std::shared_ptr<ControllerData> pData)
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

void ControlLayer::waitOnChildGaitGenerator(std::shared_ptr<GaitGeneratorData> pData)
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

// =============================================================================
// Helper Classes
// =============================================================================
ControlLayer::ControllerData::ControllerData
(
	std::shared_ptr<spline::SplineBase<double>> spline_in_,
	std::shared_ptr<spline::SplineBase<double>> spline_out_,
	const std::chrono::duration<double> &duration_in,
	const std::chrono::duration<double> &duration_out
) :
	controller_pid(0),
	pSubscriber(nullptr),
	ID(),
	premultiplier(0),
	spline_in_duration(duration_in),
	spline_out_duration(duration_out),
	pSpline_in(spline_in_),
	pSpline_out(spline_out_)
{ }
