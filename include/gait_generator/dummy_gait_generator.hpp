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
#ifndef DUMMY_GAIT_GENERATOR_HPP_BVC8KPN1
#define DUMMY_GAIT_GENERATOR_HPP_BVC8KPN1

#include "gait_generator/gait_generator.hpp"
#include "util/debug/debug.hpp"
#include "util/log/log.hpp"

namespace dls
{
TODO("this is a temporary class. Remove this file, the cpp file, and the CMake entry")
class DummyGaitGenerator : public GaitGenerator
{
public:
	DummyGaitGenerator();
	DummyGaitGenerator(const std::shared_ptr<Dog> &pDog);
	void run(const std::chrono::system_clock::time_point&) override;
	AppLayerComponent::Status eStop() override {return getStatus();}

private:
	logging::coutstream scout;
	logging::clogstream sclog;
};
} // end namespace dls

#endif /* end of include guard: DUMMY_GAIT_GENERATOR_HPP_BVC8KPN1 */
