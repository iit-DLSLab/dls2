
#ifndef DUMMY_MOTION_GENERATOR_HPP_BVC8KPN1
#define DUMMY_MOTION_GENERATOR_HPP_BVC8KPN1

#include "dls2/motion_generator/motion_generator.hpp"
#include "dls2/log/log.hpp"
#include "dls_messages/dds/stringmsgPubSubTypes.h"
#include "dls2/util/service/service.hpp"

namespace dls
{
// TODO("this is a temporary class. Remove this file, the cpp file, and the CMake entry")
class DummyMotionGenerator : public MotionGenerator
{
public:
	// DummyMotionGenerator();
	DummyMotionGenerator(const std::shared_ptr<robotlib::RobotBase> &pDog);
	void run(const std::chrono::system_clock::time_point&) override;
	App::AppStatus eStop() override {return getStatus();}

	std::string where() override;

private:
	logging::clogstream scout;
	logging::clogstream sclog;
	ServiceClient<StringMsgPubSubType, StringMsgPubSubType> service_client;
};
} // end namespace dls

#endif /* end of include guard: DUMMY_MOTION_GENERATOR_HPP_BVC8KPN1 */
