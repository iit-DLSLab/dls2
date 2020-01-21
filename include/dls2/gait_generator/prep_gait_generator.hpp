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
#ifndef PREP_GAIT_GENERATOR_HPP_BVC8KPN1
#define PREP_GAIT_GENERATOR_HPP_BVC8KPN1

#include "dls2/gait_generator/gait_generator.hpp"
#include "dls2/util/debug/debug.hpp"
#include "dls2/util/log/log.hpp"

#include <doglib/base/dog.hpp>

namespace dls
{
class PrepGaitGenerator : public GaitGenerator
{
public:
	// PrepGaitGenerator();
	PrepGaitGenerator(const std::shared_ptr<dog::Dog> &pDog);
	void run(const std::chrono::system_clock::time_point&);
	AppLayerComponent::Status eStop() override {return getStatus();}
private:
	GaitSignal data;
	logging::coutstream scout;
};
} // end namespace dls

#endif /* end of include guard: PREP_GAIT_GENERATOR_HPP_BVC8KPN1 */
