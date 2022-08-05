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
#include "dls2/components/app_layer_component.hpp"

using namespace dls;

// =============================================================================
// Constructors
// =============================================================================
AppLayerComponent::AppLayerComponent(const std::string &ID_) 
	: command_manager(ID_)
	, scout(ID_)
	, status(Status::UNCONSTRUCTED)
	, status_mutex()
    , ID(ID_)
{
	this->command_manager.addCommand<>
	(
		"where",
		std::string("Prints the state of ") + this->getID(),
		std::function<bool()>([&]()->bool
        {
			auto s = where();
			std::cout << s << std::endl;
			scout     << s << std::endl;
            return true;
		}),
		{{0,0}},
	 	true
	);
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

std::string AppLayerComponent::getID()
{
	return this->ID;
}
