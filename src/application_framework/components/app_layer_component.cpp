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
#include "application_framework/components/app_layer_component.hpp"

// =============================================================================
// Constructors
// =============================================================================
dls::AppLayerComponent::AppLayerComponent() :
	status(Status::UNCONSTRUCTED),
	status_mutex()
{ }

// =============================================================================
// Class Implementation
// =============================================================================
dls::AppLayerComponent::Status dls::AppLayerComponent::getStatus()
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	return this->status;
}

void dls::AppLayerComponent::setStatus(Status s)
{
	this->status = s;
}
