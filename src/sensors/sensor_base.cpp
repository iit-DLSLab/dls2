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
#include "sensors/sensor_base.hpp"

// =============================================================================
// Constructors
// =============================================================================
dls::SensorBase::SensorBase(Criticality c) :
	criticality(c),
	status(Status::UNINITIALISED),
	status_mutex()
{ }

// =============================================================================
// Member Functions
// =============================================================================
dls::SensorBase::Criticality dls::SensorBase::getCriticality() const
{
	return this->criticality;
}

dls::SensorBase::Status dls::SensorBase::getStatus() const
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	return this->status;
}

void dls::SensorBase::setStatus(Status s)
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	this->status = s;
}
