#ifndef CONTROL_LAYER_HPP_YCHFNYBM
#define CONTROL_LAYER_HPP_YCHFNYBM

#include "application_framework/app_layer.hpp"
#include "controller/controller.hpp"

#include <vector>
#include <memory>
#include <mutex>

class ControlLayer : public AppLayer
{
public:
	ControlLayer();

	// TODO these are not implemented
	Status run() override;
	Status shutdown() override;

	template <typename controller_t>
	void addController(std::shared_ptr<controller_t>&);

private:
	// BEGIN critical section
		std::vector<std::shared_ptr<Controller>> controllers;
		std::mutex controllers_mutex;
	// END critical section
};

#include "application_framework/control_layer.tpp"

#endif /* end of include guard: CONTROL_LAYER_HPP_YCHFNYBM */
