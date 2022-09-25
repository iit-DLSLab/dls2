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

template<class DataIn, class DataOut>
Hardware<DataIn, DataOut>::Hardware
(
	const std::string &name_,
	const std::string &robotName_,
	const period_t &period_,
    const dls::topicType &control_signal_topic_,
	const dls::topicType &raw_signal_topic_
)
	: PeriodicAppLayerComponent(name_, period_)
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
	try
    {
        this->pRobot = robotlib::RobotFactory::openRobot(robotName_);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

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
				DataIn msg = *((DataIn*) tuple);
				this->desired_torques = msg;
			}
		}
	);

	std::cout << "### " << pRobot->getName() << " ROBOT HARDWARE INTERFACE IS RUNNING ###" << std::endl;
}

template<class DataIn, class DataOut>
void Hardware<DataIn, DataOut>::publishSignal()
{
	if (!this->ddslink.sendMessage("signalout", (void *) &blind_state))
        std::cout << "=== Problems sending ControlSignal ===" << std::endl;
}

template<class DataIn, class DataOut>
void Hardware<DataIn, DataOut>::executeCommand(std::string cmd)
{
	if(cmd == "shutdown"){
		this->stop();
	}
}

#endif /* end of include guard: HARDWARE_CPP */