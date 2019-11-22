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
#include <thread>
#include "controller/controller.hpp"
#include "todo.h"
#include "util/debug/debug.hpp"
#include "topics/gait_signal.hpp"
#include "fastrtps_wrappers/blind_state.hpp"
#include "topics/control_signal_base.hpp"
#include "topics/low_level_estimation/blind_state.hpp"

// =============================================================================
// Constructors
// =============================================================================
using namespace dls;
Controller::Controller
(
	const std::shared_ptr<Dog> &dog_,
	const std::string &name_,
	const period_t &period_,
	const ControlSignal::SignalReconstructionMethod &reconst_meth
):
	PeriodicAppLayerComponent(period_),
	pDog(dog_),
	name(name_),
	signal_reconstruction_method(reconst_meth),
	ID(name_),
	pGait_signal(nullptr),
	gait_signal_mutex(),
	pControl_signal(nullptr),
	control_signal_mutex(),
	pBlind_state_signal(nullptr),
	blind_state_signal_mutex(),
	should_run(false),
	gait_listener(std::shared_ptr<Controller>(this,[](Controller*){})),
	blind_state_listener(std::shared_ptr<Controller>(this,[](Controller*){})),
	control_signal_topic(std::string(topics::control_signal_base) + name_),
	publisher(control_signal_topic)
{ }

// =============================================================================
// Implementation
// =============================================================================
Controller::ID_t Controller::getID() const
{
	return this->ID;
}

std::shared_ptr<const GaitSignal> Controller::readGaitSignal() const
{
	std::lock_guard<std::mutex> lock(this->gait_signal_mutex);
	return this->pGait_signal;
}

std::shared_ptr<BlindState> Controller::readBlindStateSignal() const
{
	std::lock_guard<std::mutex> lock(this->blind_state_signal_mutex);
	return this->pBlind_state_signal;
}

void Controller::publishSignal(const ControlSignal &msg)
{
	ControlSignalMsg p = msg;
	publisher.publish(p);
}

std::string Controller::getControlSignalTopic() const
{
	return this->control_signal_topic;
}

// =============================================================================
// FastRTPS
// =============================================================================
Controller::GaitListener::GaitListener(std::shared_ptr<Controller> p) :
	SubscriberBase<GaitSignalMsgPubSubType>(topics::gait_signal),
	pOwner(p),
	info()
{ }

void Controller::GaitListener::onNewDataMessage(eprosima::fastrtps::Subscriber *pSub)
{
	GaitSignalMsg st;
	if(pSub->takeNextData(&st, &info))
	{
		std::lock_guard<std::mutex> lock(pOwner->gait_signal_mutex);
		// TODO do not reassign memory, just reset it
		pOwner->pGait_signal = std::make_shared<const GaitSignal>(st);
	}
}

Controller::BlindStateListener::BlindStateListener(std::shared_ptr<Controller> p) :
	SubscriberBase<BlindStateMsgPubSubType>(topics::low_level_estimation::blind_state),
	pOwner(p),
	info()
{ }

void Controller::BlindStateListener::onNewDataMessage(eprosima::fastrtps::Subscriber *pSub)
{
	BlindStateMsg bs;
	if(pSub->takeNextData(&bs, &info))
	{
		std::lock_guard<std::mutex> lock(pOwner->blind_state_signal_mutex);
		// TODO do not reassign memory, just reset it
		pOwner->pBlind_state_signal = std::make_shared<BlindState>(bs);
	}
}
