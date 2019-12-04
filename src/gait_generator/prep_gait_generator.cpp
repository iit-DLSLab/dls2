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
#include "gait_generator/prep_gait_generator.hpp"
#include "util/debug/debug.hpp"
#include "geometry/pose.hpp"
#include "util/log/log.hpp"

namespace dls
{

PrepGaitGenerator::PrepGaitGenerator(const std::shared_ptr<Dog> &pDog) : GaitGenerator(pDog, "prep_gait_generator", std::chrono::milliseconds(1)), data()
{
	logging::clog << "prep gait generator launched" << std::endl;
	data.desired_joint_state.position.resize(12);
	data.desired_joint_state.velocity.resize(12);
	data.desired_joint_state.acceleration.resize(12); // TODO this field causes a segfault!!!
	data.desired_joint_state.effort.resize(12);
	data.desired_joint_state.position << -0.2, 0.7, -1.4, -0.2, 0.7, -1.4, -0.2, -0.7, 1.4, -0.2, -0.7, 1.4;
	data.desired_joint_state.velocity << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
	data.desired_joint_state.acceleration << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
	data.desired_joint_state.effort << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
}

PrepGaitGenerator::PrepGaitGenerator() : PrepGaitGenerator(std::make_shared<Dog>())
{
	logging::clog << "prep gait generator destroyed" << std::endl;
}

void PrepGaitGenerator::run(const std::chrono::system_clock::time_point &time)
{
	// logging::clog << "Prep Gait Generator Epoch" << std::endl;
	publishData(data);
	time.time_since_epoch();
}

extern "C" GaitGenerator *create()
{
	auto p = new PrepGaitGenerator;
	return p;
}

extern "C" void destroy(GaitGenerator *p)
{
	delete p;
}

} //namespace dls
