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
#include "dls2/controller/dummy_controller.hpp"
#include "dls2/util/debug/debug.hpp"
#include "dls2/util/log/log.hpp"
#include <doglib/factory/robot_factory.hpp>

using namespace dls;
using dls::dog::Dog;

DummyController::DummyController
(
	const std::shared_ptr<Dog> &dog
) :
	Controller
	(
		dog,
		"dls_dummy_controller",
		// std::chrono::duration<double, std::ratio<1, 1'000'000'000>>(1),
		std::chrono::duration<double, std::ratio<1, 1>>(2),
		ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
	)//,
	// command_manager(),
	// outstream(getID()),
	// logstream(getID()),
	// service
	// (
	// 	"dls_dummy_controller_service",
	// 	[](StringMsg msg)
	// 	{
	// 		std::cout << "Dummy controller got service call with msg: "
                      // << msg.msg() << std::endl;
	// 		std::cout << "Dummy controller responding with the message modified"
                      // << std::endl;
	// 		msg.msg() = std::string("returned from DummyController: ") + msg.msg();
	// 		std::cout << "Dummy controller exiting callback" << std::endl;
	// 		return msg;
	// 	}
	// )
{
	// outstream << "dummy controller launched" << std::endl;

	// command_manager.addCommand<double, double, double>
	// (
	// 	getID(),
	// 	"addTwoDoubles",
	// 	"adds two doubles",
	// 	std::function<double(double,double)>
	// 	{
	// 		[](double a, double b) ->double
	// 		{
	// 			std::cout << a + b << std::endl;
	// 			return a + b;
	// 		}
	// 	}
	// );

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

// DummyController::DummyController() :
// 	DummyController
// 	(
// 		// std::make_shared<Dog>()
// 		dls::dog::RobotFactory::buildRobot(dls::dog::RobotFactory::RobotType::HyQ)
// 	)
// { }

DummyController::~DummyController()
{ }

std::string DummyController::where()
{
	return "why are you asking me that?";
}

void DummyController::run(const std::chrono::system_clock::time_point &time)
{
	// Command<int, int> command("dummy", "help", foo);
	// logstream << "Dummy Controller Epoch" << std::endl;
	// std::cout << "Dummy Controller Epoch" << std::endl;
	auto pGait_signal = this->readGaitSignal();
	auto pBlind_state = this->readBlindStateSignal();
	if(!pBlind_state)
	{
		std::cout << "dummy controller 2 did not get blind state" << std::endl;
	}
	else
	{
		std::cout << "DUMMY CONTROLLER 2 GOT BLIND STATE" << std::endl;
	}

	ControlSignal s;
	s.torques.resize(12);

	publishSignal(s);

	time.time_since_epoch();
}

extern "C" Controller *create(std::shared_ptr<dls::dog::Dog> pDog)
{
	auto p = new DummyController(pDog);
	std::cout << "loaded dummy controller on: " << pDog->getName() << std::endl;
	return p;
}

extern "C" void destroy(Controller *p)
{
	delete p;
}
