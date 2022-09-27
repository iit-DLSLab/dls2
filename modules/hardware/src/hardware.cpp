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
#ifndef HARDWARE_CPP
#define HARDWARE_CPP

#include "dls2/hardware/hardware.hpp"

using namespace dls;
using robotlib::RobotBase;

Hardware::Hardware
(
	const ID_t& name_,
	const std::shared_ptr<robotlib::RobotBase> &pRobot_,
	const period_t &period_,
    const dls::topicType &control_signal_topic_,
	const dls::topicType &raw_signal_topic_
)
	: PeriodicAppLayerComponent(name_, period_)
    , should_run(false)
    , control_signal_topic(control_signal_topic_)
	, raw_signal_topic(raw_signal_topic_)
    , pRobot(pRobot_)
	, ddslink(
		"Hardware::" + name_,
		dls::domains::control
	)
	, ddsMonitor(
		name_,
		dls::domains::hardwares,
		dls::topics::command_send,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				CommandSendMsg msg = *((CommandSendMsg*) tuple);

				if (msg.name() == this->getID()){
					this->executeCommand(msg.command());			
                }
			}
		}

	)
{
	ddslink.addWriter(
		"blindState", 
		raw_signal_topic
	);

	ddslink.addReader(
		"desiredTorqueListener",
		control_signal_topic,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				DesiredTorquesMsg msg = *((DesiredTorquesMsg*) tuple);
				this->desired_torques = msg;
			}
		}
	);

	std::cout << "### " << pRobot->getName() << " ROBOT HARDWARE INTERFACE IS RUNNING ###" << std::endl;
}

void Hardware::publishSignal()
{
	if (!this->ddslink.sendMessage("signalout", (void *) &blind_state))
        std::cout << "=== Problems sending ControlSignal ===" << std::endl;
}

void Hardware::executeCommand(std::string cmd)
{
	if(cmd == "shutdown"){
		this->stop();
	}
}

#endif /* end of include guard: HARDWARE_CPP */