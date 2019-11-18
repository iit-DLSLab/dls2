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
#include "controller/pid_controller.hpp"
#include "util/debug/debug.hpp"
#include "util/log/log.hpp"

PidController::PidController (const std::shared_ptr<Dog> &dog) : Controller
	(
		dog,
		"pid_controller",
		std::chrono::duration<double>(1),
		ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
	)
{
	logging::clog << "pid controller launched" << logging::endl;
}

PidController::PidController() : PidController(std::make_shared<Dog>())
{
	logging::clog << "pid controller destroyed" << logging::endl;
}

void PidController::run(const std::chrono::system_clock::time_point &time)
{
	logging::clog << "PID Controller Epoch" << logging::endl;
	auto pGait_signal = this->readGaitSignal();
	auto pBlind_state_signal = this->readBlindStateSignal();

	if(pBlind_state_signal && pGait_signal)
	{

		double kp = 300.0;
		//double ki = 0.0;
		double kd = 10.0;

		Eigen::VectorXd tau = kp*(pGait_signal->desired_joint_position-pBlind_state_signal->joint_position) + kd*(pGait_signal->desired_joint_velocity-pBlind_state_signal->joint_velocity);
		ControlSignal s;
		s.torques=tau;
		publishSignal(s);

		time.time_since_epoch();
	}
}

extern "C" Controller *create()
{
	auto p = new PidController;
	return p;
}

extern "C" void destroy(Controller *p)
{
	delete p;
}
