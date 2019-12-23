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
		// std::chrono::duration<double, std::ratio<1, 1'000'000'000>>(1),
		std::chrono::duration<double, std::ratio<1, 1>>(1),
		ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
	),
	command_manager(),
	outstream(getID()),
	logstream(getID())
{
	outstream << "dummy controller launched" << std::endl;
	outstream << "Constructed a dummy controller. Hello from my new fancy stream" << std::endl;

	command_manager.addCommand<double, double, double>
	(
		getID(),
		"addTwoDoubles",
		"adds two doubles",
		std::function<double(double,double)>
		{
			[](double a, double b) ->double
			{
				std::cout << a + b << std::endl;
				return a + b;
			}
		}
	);

	// std::cout << "creating free standing command" << std::endl;
	// Command<double, double, double>
	// (
	// 	"test",
	// 	"test",
	// 	"test",
	// 	std::function<double(double,double)>
	// 	{
	// 		[](double a, double b) ->double
	// 		{
	// 			std::cout << a + b << std::endl;
	// 			return a + b;
	// 		}
	// 	}
	// );
	// std::cout << "removing free-standing command" << std::endl;
}

DummyController::DummyController() :
	DummyController(std::make_shared<Dog>())
{
	outstream << "dummy controller destroyed" << std::endl;
	outstream << "Destroyed a dummy controller. Hello from my new fancy stream" << std::endl;
}

DummyController::~DummyController()
{
	std::cout << "Dummy controller destructor" << std::endl;
}

void DummyController::run(const std::chrono::system_clock::time_point &time)
{
	// Command<int, int> command("dummy", "help", foo);
	logstream << "Dummy Controller Epoch" << std::endl;
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
