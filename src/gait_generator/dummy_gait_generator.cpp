#include "gait_generator/dummy_gait_generator.hpp"
#include "util/debug/debug.hpp"
#include "geometry/pose.hpp"

DummyGaitGenerator::DummyGaitGenerator(const std::shared_ptr<Dog> &pDog) :
	GaitGenerator
	(
		pDog,
		"dummy_gait_generator",
		std::chrono::duration<double>(1)
	)
{ }

DummyGaitGenerator::DummyGaitGenerator()
	: DummyGaitGenerator(std::make_shared<Dog>())
{ }

void DummyGaitGenerator::run(const std::chrono::system_clock::time_point &time)
{
	GaitSignal data;

	Eigen::Vector3d com_position; com_position << 10, 2, 33;
	DLOG(com_position);
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
