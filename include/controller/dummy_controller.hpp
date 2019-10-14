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
		const std::shared_ptr<Dog> &dog,
		const std::string &name,
		const period_t &period
	) :
		Controller(dog, name, period)
	{}

	void run(const std::chrono::system_clock::time_point &time) override
	{
		static int cnt = 0;
		std::cout << "Running " << name << " " << std::endl;
		time.time_since_epoch();
		if(++cnt == 5)
		{
			setStatus(Status::SUCCESS);
		}
	}

	Status shutdown() override { return getStatus(); }
	Status eStop() override { return getStatus(); }
};

#endif /* end of include guard: DUMMY_CONTROLLER_HPP_YGHMJHQN */
