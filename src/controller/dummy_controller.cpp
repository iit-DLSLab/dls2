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

dls::DummyController::DummyController
(
	const std::shared_ptr<Dog> &dog
) :
	Controller
	(
		dog,
		"dummy_controller",
		std::chrono::duration<double>(1),
		ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
	)
{
	logging::clog << "dummy controller launched" << logging::endl;
}

dls::DummyController::DummyController() :
	DummyController(std::make_shared<Dog>())
{
	logging::clog << "dummy controller destroyed" << logging::endl;
}

void dls::DummyController::run(const std::chrono::system_clock::time_point &time)
{
	logging::clog << "Dummy Controller Epoch" << logging::endl;
	auto pGait_signal = this->readGaitSignal();

	ControlSignal s;
	s.torques.resize(12);

	publishSignal(s);

	time.time_since_epoch();
}

extern "C" dls::Controller *create()
{
	auto p = new dls::DummyController;
	return p;
}

extern "C" void destroy(dls::Controller *p)
{
	delete p;
}
