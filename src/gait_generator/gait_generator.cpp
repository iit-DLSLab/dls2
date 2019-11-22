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
#include "gait_generator/gait_generator.hpp"
#include "util/debug/debug.hpp"
#include "topics/gait_signal.hpp"

#include <chrono>
#include <thread>

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
GaitGenerator::GaitGenerator
(
	const std::shared_ptr<Dog> &pRobot_,
	const ID_t &ID_,
	const period_t &period_
) :
	PeriodicAppLayerComponent(period_),
	pRobot(pRobot_),
	ID(ID_),
	pData(nullptr),
	data_mutex(),
	publisher(topics::gait_signal)
{ }

// =============================================================================
// Implementation
// =============================================================================
GaitGenerator::ID_t GaitGenerator::getID()
{
	return this->ID;
}
void GaitGenerator::publishData(const GaitSignal &signal)
{
	GaitSignalMsg p = signal;
	publisher.publish(p);
}
