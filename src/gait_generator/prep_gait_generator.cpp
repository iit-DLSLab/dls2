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

PrepGaitGenerator::PrepGaitGenerator(const std::shared_ptr<Dog> &pDog) : GaitGenerator
	(
		pDog,
		"prep_gait_generator",
		std::chrono::milliseconds(4)
	)
{
	logging::clog << "prep gait generator launched" << logging::endl;
}

PrepGaitGenerator::PrepGaitGenerator() : PrepGaitGenerator(std::make_shared<Dog>())
{
	logging::clog << "prep gait generator destroyed" << logging::endl;
}

void PrepGaitGenerator::run(const std::chrono::system_clock::time_point &time)
{
	logging::clog << "Prep Gait Generator Epoch" << logging::endl;
	GaitSignal data;

	Eigen::VectorXd q; q.resize(12); q << -0.2, 0.7, -1.4, -0.2, 0.7, -1.4, -0.2, -0.7, 1.4, -0.2, -0.7, 1.4;
	Eigen::VectorXd qd;	qd.resize(12); qd << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
	Eigen::VectorXd qdd; qdd.resize(12);qdd << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
	data.desired_joint_position = q;
	data.desired_joint_velocity = qd;
	data.desired_joint_acceleration = qdd;

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
