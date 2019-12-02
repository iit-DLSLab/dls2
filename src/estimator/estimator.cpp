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
#include "estimator/estimator.hpp"

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
Estimator::Estimator(const ID_t &inID, const period_t &period) :
	PeriodicAppLayerComponent(inID, period)
	// ID(inID)
{ }

// =============================================================================
// Interface Override Functions
// =============================================================================
// AppLayerComponent::Status Estimator::run()
// {
// 	TODO("Unimplemented")
// 	return getStatus();
// }

// AppLayerComponent::Status Estimator::eStop()
// {
// 	TODO("Unimplemented")
// 	return getStatus();
// }

// AppLayerComponent::Status Estimator::stop()
// {
// 	TODO("Unimplemented")
// 	return getStatus();
// }

// =============================================================================
// Implementation
// =============================================================================
// Estimator::ID_t Estimator::getID() const
// {
// 	return this->ID;
// }
