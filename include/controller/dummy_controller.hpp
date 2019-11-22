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
#ifndef DUMMY_CONTROLLER_HPP_YGHMJHQN
#define DUMMY_CONTROLLER_HPP_YGHMJHQN

#include "controller/controller.hpp"
#include <iostream>

TODO("This is a dummy class. Remove it, its cpp file, as well as its entry in the CMakeLists file")
namespace dls
{
class DummyController : public Controller
{
public:
	DummyController
	(
		const std::shared_ptr<Dog> &dog
		// const std::string &name,
		// const period_t &period
	);

	DummyController();

	void run(const std::chrono::system_clock::time_point &time) override;

	// Status stop() override { return getStatus(); }
	Status eStop() override { return getStatus(); }
};
} // end namespace dls

#endif /* end of include guard: DUMMY_CONTROLLER_HPP_YGHMJHQN */
