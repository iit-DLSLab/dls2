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
#ifndef PERIODIC_APP_LAYER_COMPONENT_HPP_RY9LWBZG
#define PERIODIC_APP_LAYER_COMPONENT_HPP_RY9LWBZG

#include "application_framework/components/app_layer_component.hpp"
#include "msg/timePubSubTypes.h"
#include "util/messaging/subscriber_base.hpp"

#include <chrono>
#include <atomic>

namespace dls
{
class PeriodicAppLayerComponent : public AppLayerComponent
{
	friend class ClockSubscriber;
public:
	typedef std::chrono::duration<double, std::ratio<1, 1'000'000>> period_t;
public:
	PeriodicAppLayerComponent(const ID_t&,const period_t&);

	Status run() override;
	Status stop() override;
	virtual void run(const std::chrono::system_clock::time_point&) = 0;

private:
	const period_t period;
	std::atomic_bool should_run;

};
} // end namespace dls

#endif /* end of include guard: PERIODIC_APP_LAYER_COMPONENT_HPP_RY9LWBZG */
