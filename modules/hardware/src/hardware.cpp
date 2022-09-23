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
	const std::string &name_,
	const std::shared_ptr<robotlib::RobotBase> &robot_,
	const period_t &period_,
    const dls::topicType &control_signal_topic_,
	const dls::topicType &raw_signal_topic_
)
	: PeriodicAppLayerComponent(name_, period_)
	, pRobot(robot_)
    , should_run(false)
    , control_signal_topic(control_signal_topic_)
	, raw_signal_topic(raw_signal_topic_)
	, ddslink(
		"Hardware::" + name_,
		dls::domains::hardware
	)
	, ddsMonitor(
		name_,
		dls::domains::hardware,
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
	std::cout << "### " << pRobot->getName() << " ROBOT HARDWARE INTERFACE IS RUNNING ###" << std::endl;
}


void Hardware::publishSignal()
{
	// ControlSignalMsg p = signal;

	// if (!this->ddslink.sendMessage("signalout", (void *) &p))
    //     std::cout << "=== Problems sending ControlSignal ===" << std::endl;

    std::cout << "=== PUBLISH THE SIGNAL ===" << std::endl;
}

void Hardware::executeCommand(std::string cmd)
{
	if(cmd == "shutdown"){
		this->stop();
	}
}

#endif /* end of include guard: HARDWARE_CPP */