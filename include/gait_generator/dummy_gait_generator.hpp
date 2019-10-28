#ifndef DUMMY_GAIT_GENERATOR_HPP_BVC8KPN1
#define DUMMY_GAIT_GENERATOR_HPP_BVC8KPN1

#include "gait_generator/gait_generator.hpp"
#include "util/debug/debug.hpp"

TODO("this is a temporary class. Remove this file, the cpp file, and the CMake entry")
class DummyGaitGenerator : public GaitGenerator
{
public:
	DummyGaitGenerator();
	DummyGaitGenerator(const std::shared_ptr<Dog> &pDog);
	void run(const std::chrono::system_clock::time_point&);
	AppLayerComponent::Status eStop() override {return getStatus();}
};


#endif /* end of include guard: DUMMY_GAIT_GENERATOR_HPP_BVC8KPN1 */
