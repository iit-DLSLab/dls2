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
		Eigen::VectorXd tau = pGait_signal->desired_joint_position;

		// TODO VERY IMPORTANT - Middle legs are swapped
		for (int i=0;i<3;i++)
		{
			tau[i] = kp[i]*(pGait_signal->desired_joint_position[i]-pBlind_state_signal->joint_state.position[i]) + kd[i]*(pGait_signal->desired_joint_velocity[i]-pBlind_state_signal->joint_state.velocity[i]);
			tau[i+3] = kp[i+3]*(pGait_signal->desired_joint_position[i+3]-pBlind_state_signal->joint_state.position[i+6]) + kd[i+3]*(pGait_signal->desired_joint_velocity[i+3]-pBlind_state_signal->joint_state.velocity[i+6]);
			tau[i+6] = kp[i+6]*(pGait_signal->desired_joint_position[i+6]-pBlind_state_signal->joint_state.position[i+3]) + kd[i+6]*(pGait_signal->desired_joint_velocity[i+6]-pBlind_state_signal->joint_state.velocity[i+3]);
			tau[i+9] = kp[i+9]*(pGait_signal->desired_joint_position[i+9]-pBlind_state_signal->joint_state.position[i+9]) + kd[i+9]*(pGait_signal->desired_joint_velocity[i+9]-pBlind_state_signal->joint_state.velocity[i+9]);
		}
		
		ControlSignal s;
		s.torques.resize(12);
		s.torques << tau;
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
