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
#include "gait_generator/dummy_gait_generator.hpp"
#include "util/debug/debug.hpp"
#include "geometry/pose.hpp"
#include "util/log/log.hpp"

using namespace dls;
DummyGaitGenerator::DummyGaitGenerator(const std::shared_ptr<Dog> &pDog) :
	GaitGenerator
	(
		pDog,
		"dummy_gait_generator",
		std::chrono::duration<double>(1)
	)
{
	logging::clog << "dummy gait generator launched" << logging::endl;
}

DummyGaitGenerator::DummyGaitGenerator()
	: DummyGaitGenerator(std::make_shared<Dog>())
{
	logging::clog << "dummy gait generator destroyed" << logging::endl;
}

void DummyGaitGenerator::run(const std::chrono::system_clock::time_point &time)
{
	logging::clog << "Dummy Gait Generator Epoch" << logging::endl;
	GaitSignal data;

	Eigen::Vector3d com_position; com_position << 10, 2, 33;
	data.desired_com_pose = Pose(com_position);

	publishData(data);
	time.time_since_epoch();
}

extern "C" GaitGenerator *create()
{
	auto p = new DummyGaitGenerator;
	return p;
}

extern "C" void destroy(GaitGenerator *p)
{
	delete p;
}
