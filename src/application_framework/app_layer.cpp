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
#include "application_framework/app_layer.hpp"
#include "util/debug/debug.hpp"

using namespace dls;
// =============================================================================
// Constructors
// =============================================================================
dls::AppLayer::AppLayer() :
	components_mutex(),
	components({}),
	status_mutex(),
	status(Status::INITIALISING),
	main(0)
{ }

dls::AppLayer::AppLayer(const std::initializer_list<pComponent_t> &_components) :
	components_mutex(),
	components(_components),
	status_mutex(),
	status(Status::INITIALISING),
	main(0)
{ }

// =============================================================================
// Member Functions
// =============================================================================
dls::AppLayer::Status dls::AppLayer::getStatus() const
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	return this->status;
}

void dls::AppLayer::setStatus(Status s)
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	this->status = s;
}

dls::AppLayer::Status dls::AppLayer::eStop()
{
	{
		std::lock_guard<std::mutex> lock(this->components_mutex);
		for(const auto pComponent : this->components)
		{
			pComponent->eStop();
		}
	}

	std::lock_guard<std::mutex> lock(this->status_mutex);
	this->status = Status::E_STOP;
	return this->status;
}
