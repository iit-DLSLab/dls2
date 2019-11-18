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

#include "gait_generator/gait_generator.hpp"
#include "util/debug/debug.hpp"

TODO("this is a temporary class. Remove this file, the cpp file, and the CMake entry")
class PrepGaitGenerator : public GaitGenerator
{
public:
	PrepGaitGenerator();
	PrepGaitGenerator(const std::shared_ptr<Dog> &pDog);
	void run(const std::chrono::system_clock::time_point&);
	AppLayerComponent::Status eStop() override {return getStatus();}
};


#endif /* end of include guard: PREP_GAIT_GENERATOR_HPP_BVC8KPN1 */
