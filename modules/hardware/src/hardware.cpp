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
	const dls::topicType &blind_state_topic_
)
	: PeriodicAppLayerComponent(name_, period_)
    , should_run(false)
    , control_signal_topic(control_signal_topic_)
	, blind_state_topic(blind_state_topic_)
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
	ddslink.addWriter("blindState", this->blind_state_topic);

	ddslink.addReader(
		"desiredSignalListener",
		dls::topics::desired_torques,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				DesiredTorquesMsg  msg = *((DesiredTorquesMsg *) tuple);
				this->control_signal = msg;
                this->heart_beat = true;
			}
		}
	);

	for (int i = 0; i < 12; i++)
	{
		control_signal.desired_torques()[i] = 0;
	}

	std::cout << "### " << pRobot->getName() << " ROBOT HARDWARE INTERFACE IS RUNNING ###" << std::endl;
}

void Hardware::publishSignal()
{
	if (!this->ddslink.sendMessage("blindState", (void*) &(this->blind_state)))
        std::cout << "=== Problems sending ControlSignal ===" << std::endl;
}

void Hardware::executeCommand(std::string cmd)
{
	if(cmd == "shutdown"){
		this->stop();
	}
}

bool Hardware::readBeat()
{
    bool out = this->heart_beat;
    this->heart_beat = false;

    return out;
}

#endif /* end of include guard: HARDWARE_CPP */