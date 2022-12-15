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
#ifndef SERVICE_BASE_CPP
#define SERVICE_BASE_CPP

#include "dls2/service/service_base.hpp"

namespace dls
{
	// =========================================================================
	// Service Implementation
	// =========================================================================
	ServiceBase::ServiceBase(std::string& ID_)
	    : AppLayerComponent(ID_)
		, should_quit(false)
	{ }

    AppLayerComponent::Status ServiceBase::run()
	{
		while(!this->should_quit)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		return this->getStatus();
	}

	AppLayerComponent::Status ServiceBase::stop()
	{
		this->should_quit = true;

		return this->getStatus();
	}
}

#endif /* end of include guard: SERVICE_BASE_CPP */