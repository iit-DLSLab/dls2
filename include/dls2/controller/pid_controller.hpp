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
#ifndef PID_CONTROLLER_HPP_YGHMJHQN
#define PID_CONTROLLER_HPP_YGHMJHQN

#include "dls2/controller/controller.hpp"
#include <iostream>
#include "dls2/util/log/log.hpp"
#include "dls2/command/command.hpp"

#include <doglib/base/dog.hpp>

namespace dls
{
// TODO This is a dummy class. Remove it, its cpp file, as well as its entry in the CMakeLists file
class PidController : public Controller
{
public:
	PidController
	(
		const std::shared_ptr<dog::Dog> &dog
		// const std::string &name,
		// const period_t &period
	);

	PidController();

	void run(const std::chrono::system_clock::time_point &time) override;

	// Status stop() override { return getStatus(); }
	Status eStop() override { return getStatus(); }
private:
	logging::coutstream scout;
	// Command<int, int> command;
	CommandManager command_manager;
};
} // end namespace dls
#endif /* end of include guard: PID_CONTROLLER_HPP_YGHMJHQN */
