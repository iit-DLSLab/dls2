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
// =============================================================================
// Constructors
// =============================================================================
GaitGenerator::GaitGenerator
(
	const ID_t &ID,
	const std::shared_ptr<dog::Dog> &pRobot_,
	const period_t &period_,
	const dls::topicType& gateTopic_,
	const dls::topicType& blindStateTopic_
) 
	: PeriodicAppLayerComponent(ID, period_)
	// , pRobot(pRobot_)
	// , pData(nullptr)
	// , data_mutex()
	// , pBlind_state_signal(nullptr)
	// , blind_state_signal_mutex()
	, ddslink("GaitGen::" + ID, dls::domains::control)
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
				// TODO do not reassign memory, just reset it
				this->pBlind_state_signal = std::make_shared<BlindState>(bs);
				// std::cout << "received a blind state message of size: "
				//           << bs.joint_state().position().size() << std::endl;
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

std::shared_ptr<BlindState> GaitGenerator::readBlindStateSignal() const
{
	std::lock_guard<std::mutex> lock(this->blind_state_signal_mutex);
	return this->pBlind_state_signal;
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
