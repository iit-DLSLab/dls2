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

#include "dls2/application_framework/components/app_layer_component.hpp"
#include "dls2/msg/timePubSubTypes.h"
#include "dls2/util/messaging/subscriber_base.hpp"

#include <chrono>
#include <atomic>

namespace dls
{
/// Periodic component
///
/// This class automatically calls its own run function at a given period
class PeriodicAppLayerComponent : public AppLayerComponent
{
	friend class ClockSubscriber;
public:
	typedef std::chrono::duration<double, std::ratio<1, 1'000'000>> period_t;
public:

	/// Constructor
	///
	/// @param ID the name of this component
	/// @parma period the period length of each run epoch
	PeriodicAppLayerComponent(const ID_t &ID,const period_t &period);

	/// Runs the component
	///
	/// Automatically calls the abstract run function at the correct frequency
	Status run() override;

	/// Stops this component
	///
	Status stop() override;

	/// Virtual run function
	///
	/// Overwrite this function with the function that needs to be called at the
	/// correct rate
	virtual void run(const std::chrono::system_clock::time_point&) = 0;

private:
	/// The period of this component
	///
	const period_t period;

	/// Used to determine whether the periodic run function should be called
	///
	std::atomic_bool should_run;

};
} // end namespace dls

#endif /* end of include guard: PERIODIC_APP_LAYER_COMPONENT_HPP_RY9LWBZG */
