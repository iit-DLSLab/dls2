
#include <iostream>
#include "dls2/motion_generator/dummy_motion_generator.hpp"
#include "dls2/geometry/pose.hpp"
#include "dls2/log/log.hpp"
#include "robotlib/robot_factory.hpp"

using namespace dls;
using robotlib::RobotBase;

DummyMotionGenerator::DummyMotionGenerator(const std::shared_ptr<Dog> &pDog) :
	MotionGenerator
	(
		pDog,
		"dummy_motion_generator",
		std::chrono::duration<double>(1)
	),
	scout(getID()),
	sclog(getID()),
	service_client
	(
		"dls_dummy_controller_service"
	)
{
	scout << "dummy motion generator launched" << std::endl;
}

// DummyMotionGenerator::DummyMotionGenerator()
// 	: DummyMotionGenerator(std::make_shared<Dog>())
// {
// 	scout << "dummy motion generator destroyed" << std::endl;
// }

void DummyMotionGenerator::run(const std::chrono::system_clock::time_point &time)
{
	// sclog << "Dummy Motion Generator Epoch" << std::endl;
	std::cout << "Dummy Motion Generator Epoch" << std::endl;
	GaitSignal data;

	Eigen::Vector3d com_position; com_position << 10, 2, 33;
	data.desired_com_pose_world_ = Pose(com_position);

	std::cout << "Dummy motion generator sending message" << std::endl;
	StringMsg msg; msg.msg() = "Hello from dummy motion generator";
	StringMsg response;
	if(service_client.call(msg, &response))
	{
		std::cout << "Dummy motion generator got response: " << response.msg()
		          << std::endl;
	}
	else
	{
		std::cout << "service call failed" << std::endl;
	}

	publishData(data);
	time.time_since_epoch();
}

std::string DummyMotionGenerator::where()
{
	return "You really want to know the status of this?";
}

extern "C" MotionGenerator *create()
{
	using dls::dog::RobotFactory;
	auto p = new DummyMotionGenerator(RobotFactory::buildRobot(RobotFactory::RobotType::HyQReal));
	return p;
}

extern "C" void destroy(MotionGenerator *p)
{
	delete p;
}
