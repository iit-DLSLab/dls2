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
*******************************************************************************/
#include "dls2/controller/controller.hpp"
#include "dls2/topics/topics.hpp"
#include "dls2/log/log.hpp"

#include <thread>

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls;
using robotlib::RobotBase;

// =============================================================================
// Constructors
// =============================================================================
Controller::Controller
(
	const std::string &name_,
	const std::shared_ptr<robotlib::RobotBase> &robot_,
	const period_t &period_,
	const ControlSignal::SignalReconstructionMethod &reconst_meth_,
	const dls::topicType &gait_topic_,
	const dls::topicType &blind_state_topic_
)
	: PeriodicAppLayerComponent(name_, period_)
	, pRobot(robot_)
	, signal_reconstruction_method(reconst_meth_)
	// , pGait_signal(nullptr),
	// gait_signal_mutex(),
	// pControl_signal(nullptr),
	// control_signal_mutex(),
	// pBlind_state_signal(nullptr),
	// blind_state_signal_mutex()
	, should_run(false)
	, control_signal_topic(dls::topicType("Listener::" + name_, new ControlSignalMsgPubSubType()))
	, gait_topic(gait_topic_)
	, blind_state_topic(blind_state_topic)
	, ddslink(
		"Controller::" + name_,
		dls::domains::control
	)
	, ddsMonitor(
		name_,
		dls::domains::controllers,
		dls::topics::command_send,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				CommandSendMsg msg = *((CommandSendMsg*) tuple);

				if (msg.name() == this->getID())
					this->executeCommand(msg.command());			
			}
		}

	)
{

	this->ddslink.addWriter("signalout", this->control_signal_topic);

	this->ddslink.addReader("gaitListener",
		this->gait_topic,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				GaitSignalMsg st = *((GaitSignalMsg*) tuple);

				std::lock_guard<std::mutex> lock(this->gait_signal_mutex);
				this->gait_signal = st;
			}
		}
	);

	this->ddslink.addReader("blindStateListener",
		//this->blind_state_topic,
		dls::topics::low_level_estimation::blind_state,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				BlindStateMsg bs = *((BlindStateMsg*) tuple);

				std::lock_guard<std::mutex> lock(this->blind_state_signal_mutex);
				this->blind_state_signal = bs;
			}
		}
	);

	std::cout << "### CONTROLLER IS PUBLISHING ON TOPIC: '"	<< name_ << "' ###" << std::endl;
}

// =============================================================================
// Implementation
// =============================================================================
GaitSignal Controller::readGaitSignal()
{
	std::lock_guard<std::mutex> lock(this->gait_signal_mutex);
	return gait_signal;
}

BlindState Controller::readBlindStateSignal()
{
	std::lock_guard<std::mutex> lock(this->blind_state_signal_mutex);
	return this->blind_state_signal;
}

void Controller::publishSignal(const ControlSignal &signal)
{
	ControlSignalMsg p = signal;
	
	this->ddslink.sendMessage("signalout", (void *) &p);
}

std::string Controller::getControlSignalTopic() const
{
	return this->control_signal_topic.first;
}

void Controller::executeCommand(std::string cmd)
{
	if(cmd == "shutdown"){
		this->stop();
	}
	else if(cmd == "activate"){
		this->run();
	}
}