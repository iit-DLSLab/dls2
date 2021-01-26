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
#include "dls2/topics/gait_signal.hpp"
#include "dls2/topics/low_level_estimation/blind_state.hpp"

#include <chrono>
#include <thread>
#include <iostream>

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
GaitGenerator::GaitGenerator
(
	const std::shared_ptr<dog::Dog> &pRobot_,
	const ID_t &ID,
	const period_t &period_
) :
	PeriodicAppLayerComponent(ID, period_),
	pRobot(pRobot_),
	// ID(ID_),
	pData(nullptr),
	data_mutex(),pBlind_state_signal(nullptr),
	blind_state_signal_mutex(),
	publisher(topics::gait_signal),
	blind_state_listener(std::shared_ptr<GaitGenerator>(this, [](GaitGenerator*){}))
{ }

// =============================================================================
// Implementation
// =============================================================================
void GaitGenerator::publishData(const GaitSignal &signal)
{
	GaitSignalMsg p = signal;
	publisher.publish(p);
}

std::shared_ptr<BlindState> GaitGenerator::readBlindStateSignal() const
{
	std::lock_guard<std::mutex> lock(this->blind_state_signal_mutex);
	return this->pBlind_state_signal;
}

// =============================================================================
// FastRTPS
// =============================================================================
GaitGenerator::BlindStateListener::BlindStateListener(std::shared_ptr<GaitGenerator> p) :
	SubscriberBase<BlindStateMsgPubSubType>(topics::low_level_estimation::blind_state),
	pOwner(p),
	info()
{ }

void GaitGenerator::BlindStateListener::onNewDataMessage(eprosima::fastrtps::Subscriber *pSub)
{
	BlindStateMsg bs;
	if(pSub->takeNextData(&bs, &info))
	{
		std::lock_guard<std::mutex> lock(pOwner->blind_state_signal_mutex);
		// TODO do not reassign memory, just reset it
		pOwner->pBlind_state_signal = std::make_shared<BlindState>(bs);
		// std::cout << "received a blind state message of size: "
		//           << bs.joint_state().position().size() << std::endl;
	}
}
