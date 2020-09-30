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
#include <iostream>
#include "dls2/application_framework/components/app_layer_component.hpp"

using namespace dls;

// =============================================================================
// Constructors
// =============================================================================
AppLayerComponent::AppLayerComponent(const ID_t &ID_) :
	status(Status::UNCONSTRUCTED),
	status_mutex(),
	ID(ID_)//,
	// command_manager(),
	// scout(ID_)
{
	// this->command_manager.addCommand<void, ARGVOID>
	// (
	// 	this->getID(),
	// 	"where",
	// 	std::string("Prints the state of ") + this->getID(),

	// 	std::function<void(ARGVOID)>
	// 	(
	// 		[&](ARGVOID)
	// 		{
	// 			auto s = where();
	// 			std::cout << s << std::endl;
	// 			scout     << s << std::endl;
	// 		}
	// 	)
	// );
}

// =============================================================================
// Class Implementation
// =============================================================================
AppLayerComponent::Status AppLayerComponent::getStatus()
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	return this->status;
}

void AppLayerComponent::setStatus(Status s)
{
	this->status = s;
}

AppLayerComponent::ID_t AppLayerComponent::getID()
{
	return this->ID;
}
