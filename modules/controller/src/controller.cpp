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
using dls::dog::Dog;

// =============================================================================
// Constructors
// =============================================================================
Controller::Controller
(
	const std::string &name_,
	const std::shared_ptr<Dog> &dog_,
	const period_t &period_,
	const ControlSignal::SignalReconstructionMethod &reconst_meth_,
	const dls::topicType &gait_topic_,
	const dls::topicType &blind_state_topic_
):
	PeriodicAppLayerComponent(name_, period_),
	pDog(dog_),
	signal_reconstruction_method(reconst_meth_),
	pGait_signal(nullptr),
	gait_signal_mutex(),
	pControl_signal(nullptr),
	control_signal_mutex(),
	pBlind_state_signal(nullptr),
	blind_state_signal_mutex(),
	should_run(false),
	control_signal_topic(dls::topicType(name_, new StringMsgPubSubType())),
	gait_topic(gait_topic_),
	blind_state_topic(blind_state_topic),
	ddslink(name_, dls::domains::control)
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
				// TODO do not reassign memory, just reset it
				// std::cout << "received a gait signal" << std::endl;
				this->pGait_signal = std::make_shared<const GaitSignal>(st);
			}
		}
	);

	this->ddslink.addReader("blindStateListener",
		this->blind_state_topic,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				BlindStateMsg bs = *((BlindStateMsg*) tuple);
				std::lock_guard<std::mutex> lock(this->blind_state_signal_mutex);
				// TODO do not reassign memory, just reset it
				this->pBlind_state_signal = std::make_shared<BlindState>(bs);
			}
		}
	);

	std::cout << "controller is publishing on topic: '"	<< name_ << "'" << std::endl;
}

// =============================================================================
// Implementation
// =============================================================================
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

void Controller::publishSignal(const ControlSignal &signal)
{
	ControlSignalMsg p = signal;
	this->ddslink.sendMessage("signalout", (void *) &p);
}

std::string Controller::getControlSignalTopic() const
{
	return this->control_signal_topic.first;
}