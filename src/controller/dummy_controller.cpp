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
#include "controller/dummy_controller.hpp"
#include "util/debug/debug.hpp"
#include "util/log/log.hpp"

using namespace dls;
DummyController::DummyController
(
	const std::shared_ptr<Dog> &dog
) :
	Controller
	(
		dog,
		"dls_dummy_controller",
		std::chrono::duration<double>(1),
		ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
	)
{
	logging::clog << "dummy controller launched" << std::endl;
}

DummyController::DummyController() :
	DummyController(std::make_shared<Dog>())
{
	logging::clog << "dummy controller destroyed" << std::endl;
}

void DummyController::run(const std::chrono::system_clock::time_point &time)
{
	logging::clog << "Dummy Controller Epoch" << std::endl;
	auto pGait_signal = this->readGaitSignal();

	ControlSignal s;
	s.torques.resize(12);

	publishSignal(s);

	time.time_since_epoch();
}

extern "C" Controller *create()
{
	auto p = new DummyController;
	return p;
}

extern "C" void destroy(Controller *p)
{
	delete p;
}
