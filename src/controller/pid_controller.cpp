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
#include "dls2/controller/pid_controller.hpp"
#include "dls2/util/debug/debug.hpp"
#include "dls2/util/log/log.hpp"

#include <doglib/factory/robot_factory.hpp>

int foo(float, std::string){return 1;}

using dls::dog::Dog;
namespace dls
{

PidController::PidController (const std::shared_ptr<Dog> &dog) : Controller
	(
		dog,
		"dls_pid_controller",
		std::chrono::milliseconds(1),
		ControlSignal::SignalReconstructionMethod::ZERO_ORDER_HOLD
	),
	scout(getID()),
	command_manager()
	// command("dummy_controller", "foo", "help", foo)
{
	scout << "pid controller launched" << std::endl;
	// command_manager.addCommand<int, float, std::string>
	// 	(
	// 		std::string("pid"),
	// 		std::string("foo"),
	// 		std::string("foohelp"),
	// 		std::function<int(float, std::string)>(foo)
	// 	);
}

PidController::PidController() : PidController(dls::dog::RobotFactory::buildRobot(dog::RobotFactory::RobotType::HyQReal))
{
	scout << "pid controller destroyed" << std::endl;
}

void PidController::run(const std::chrono::system_clock::time_point &time)
{
	// scout << "PID Controller Epoch" << std::endl;
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
		s.time = pBlind_state_signal->time;
		publishSignal(s);
		// scout << "N=" << std::to_string(tau.size()) << std::endl;
		time.time_since_epoch();
	}
	else
	{
		// if (!pBlind_state_signal) scout << 	"NO blind state signal" << std::endl;
		// if (!pGait_signal) scout << "NO gait signal" << std::endl;
	}

}

extern "C" Controller *create(std::shared_ptr<dls::dog::Dog> pDog)
{
	auto p = new PidController(pDog);
	return p;
}

extern "C" void destroy(Controller *p)
{
	delete p;
}

} //namespace dls
