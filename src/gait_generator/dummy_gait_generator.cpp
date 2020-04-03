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
#include <iostream>
#include "dls2/gait_generator/dummy_gait_generator.hpp"
#include "dls2/util/debug/debug.hpp"
#include "dls2/geometry/pose.hpp"
#include "dls2/util/log/log.hpp"
#include <doglib/factory/robot_factory.hpp>

using namespace dls;
using dls::dog::Dog;

DummyGaitGenerator::DummyGaitGenerator(const std::shared_ptr<Dog> &pDog) :
	GaitGenerator
	(
		pDog,
		"dummy_gait_generator",
		std::chrono::duration<double>(1)
	),
	scout(getID()),
	sclog(getID()),
	service_client
	(
		"dls_dummy_controller_service"
	)
{
	scout << "dummy gait generator launched" << std::endl;
}

// DummyGaitGenerator::DummyGaitGenerator()
// 	: DummyGaitGenerator(std::make_shared<Dog>())
// {
// 	scout << "dummy gait generator destroyed" << std::endl;
// }

void DummyGaitGenerator::run(const std::chrono::system_clock::time_point &time)
{
	// sclog << "Dummy Gait Generator Epoch" << std::endl;
	std::cout << "Dummy Gait Generator Epoch" << std::endl;
	GaitSignal data;

	Eigen::Vector3d com_position; com_position << 10, 2, 33;
	data.desired_com_pose_world = Pose(com_position);

	std::cout << "Dummy gait generator sending message" << std::endl;
	StringMsg msg; msg.msg() = "Hello from dummy gait generator";
	StringMsg response;
	if(service_client.call(msg, &response))
	{
		std::cout << "Dummy gait generator got response: " << response.msg()
		          << std::endl;
	}
	else
	{
		std::cout << "service call failed" << std::endl;
	}

	publishData(data);
	time.time_since_epoch();
}

std::string DummyGaitGenerator::where()
{
	return "You really want to know the status of this?";
}

extern "C" GaitGenerator *create()
{
	using dls::dog::RobotFactory;
	auto p = new DummyGaitGenerator(RobotFactory::buildRobot(RobotFactory::RobotType::HyQReal));
	return p;
}

extern "C" void destroy(GaitGenerator *p)
{
	delete p;
}
