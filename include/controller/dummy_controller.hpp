#ifndef DUMMY_CONTROLLER_HPP_YGHMJHQN
#define DUMMY_CONTROLLER_HPP_YGHMJHQN

#include "controller/controller.hpp"
#include <iostream>

// TODO remove this, its cpp file, as well as the CMakeLists entry
class DummyController : public Controller
{
public:
	DummyController
	(
		const std::shared_ptr<Dog> &dog
		// const std::string &name,
		// const period_t &period
	);

	DummyController();

	void run(const std::chrono::system_clock::time_point &time) override;

	Status shutdown() override { return getStatus(); }
	Status eStop() override { return getStatus(); }
};

#endif /* end of include guard: DUMMY_CONTROLLER_HPP_YGHMJHQN */
