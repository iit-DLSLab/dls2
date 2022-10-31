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
#include "dls2/gait_generator/gait_generator.hpp"
#include "dls2/topics/topics.hpp"

#include <chrono>
#include <thread>
#include <iostream>

using namespace dls;

GaitGenerator::GaitGenerator
(
	const std::string &ID,
	const std::shared_ptr<robotlib::RobotBase> &pRobot_,
	const period_t &period_,
	const dls::topicType& gateTopic_,
	const dls::topicType& blindStateTopic_
) 
	: PeriodicAppLayerComponent(ID, period_)
	, pRobot(pRobot_)
	// , pData(nullptr)
	// , data_mutex()
	, blind_state_signal(pRobot_)
	// , blind_state_signal_mutex()
    , heart_beat(false)
	, ddslink("GaitGen::" + ID, dls::domains::signals)
	, ddsMonitor(
		ID,
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
	this->ddslink.addWriter("signalout", gateTopic_);

	this->ddslink.addReader("blindStateListener",
		blindStateTopic_,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				BlindStateMsg bs = *((BlindStateMsg*) tuple);

				std::lock_guard<std::mutex> lock(this->blind_state_signal_mutex);
				
				this->blind_state_signal = bs;
                this->heart_beat = true;
			}
		}
	);

}

// =============================================================================
// Implementation
// =============================================================================
void GaitGenerator::publishData(const GaitSignal &signal)
{
	GaitSignalMsg p = signal;
	this->ddslink.sendMessage("signalout", (void *) &p);
}

BlindState GaitGenerator::readBlindStateSignal()
{
	std::lock_guard<std::mutex> lock(this->blind_state_signal_mutex);
    this->heart_beat = false;
	return blind_state_signal;
}


void GaitGenerator::executeCommand(std::string cmd)
{
	if(cmd == "shutdown"){
		this->stop();
	}
	else if(cmd == "activate"){
		this->run();
	}
}

bool GaitGenerator::readBeat()
{
    bool out = this->heart_beat;
    this->heart_beat = false;

    return out;
}