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

namespace dls
{

PidController::PidController (const std::shared_ptr<Dog> &dog) : Controller
	(
		dog,
		"pid_controller",
		std::chrono::milliseconds(1),
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
		
		Eigen::Matrix<double,1,12> kp; kp << 300,300,200,300,300,200,300,300,200,300,300,200;
		Eigen::Matrix<double,1,12> kd; kd << 10,10,6,10,10,6,10,10,6,10,10,6;		
		Eigen::VectorXd tau = pGait_signal->desired_joint_state.position;

		for (int i=0;i<12;i++)
		{
			tau[i] = kp[i]*(pGait_signal->desired_joint_state.position[i]-pBlind_state_signal->joint_state.position[i]) + kd[i]*(pGait_signal->desired_joint_state.velocity[i]-pBlind_state_signal->joint_state.velocity[i]);
		}
		
		ControlSignal s;
		s.torques.resize(12);
		s.torques << tau;
		publishSignal(s);
		logging::clog << "N=" << std::to_string(tau.size()) << logging::endl;
		time.time_since_epoch();
	}
	else
	{
		if (!pBlind_state_signal) logging::clog << 	"NO blind state signal" << logging::endl;
		if (!pGait_signal) logging::clog << "NO gait signal" << logging::endl;
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

} //namespace dls
