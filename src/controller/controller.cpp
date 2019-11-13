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
#include "topics/control_signal_base.hpp"
// =============================================================================
// Constructors
// =============================================================================
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
	pControl_signal_mutex(),
	should_run(false),
	listener
	(
		std::shared_ptr<Controller>
		(
			this,
			[](Controller*){} // do not use the shared pointer to delete the object
		)
	),
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
Controller::SubListener::SubListener(std::shared_ptr<Controller> p) :
	SubscriberBase<GaitSignalMsgPubSubType>(topics::gait_signal),
	pOwner(p),
	info()
{ }

void Controller::SubListener::onNewDataMessage(eprosima::fastrtps::Subscriber *pSub)
{
	GaitSignalMsg st;
	if(pSub->takeNextData(&st, &info))
	{
		std::lock_guard<std::mutex> lock(pOwner->gait_signal_mutex);
		pOwner->pGait_signal = std::make_shared<const GaitSignal>(st);
	}
}
