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
#include <iostream>

#include "dls2/application_framework/app_layer.phpp"
#include "dls2/util/debug/debug.hpp"

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
AppLayer::AppLayer
(
	const std::string &ID_
) :
	components_mutex(),
	components({}),
	ID(ID_),
	command_manager(),
	status_mutex(),
	status(Status::INITIALISING),
	scout(ID_),
	main(0)
{
	command_manager.addCommand<void, ARGVOID>
	(
		this->ID,
		"where",
		std::string("Prints the state of ") + this->ID,
		std::function<void(ARGVOID)>
		(
			[&](ARGVOID)
			{
				std::cout << where() << std::endl;
				scout << where() << std::endl;
			}
		)
	);
}

// AppLayer::AppLayer(const std::initializer_list<pComponent_t> &_components) :
// 	components_mutex(),
// 	components(_components),
// 	status_mutex(),
// 	status(Status::INITIALISING),
// 	main(0)
// { }

// =============================================================================
// Member Functions
// =============================================================================
AppLayer::Status AppLayer::getStatus() const
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	return this->status;
}

void AppLayer::setStatus(Status s)
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	this->status = s;
}

AppLayer::Status AppLayer::eStop()
{
	return this->shutdown();
	// {
	// 	std::lock_guard<std::mutex> lock(this->components_mutex);
	// 	for(const auto pComponent : this->components)
	// 	{
	// 		pComponent->eStop();
	// 	}
	// }

	// std::lock_guard<std::mutex> lock(this->status_mutex);
	// this->status = Status::E_STOP;
	// return this->status;
}
