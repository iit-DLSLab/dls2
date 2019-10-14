#ifndef PROGRAM_LAYER_HPP_YCHFNYBM
#define PROGRAM_LAYER_HPP_YCHFNYBM

#include "application_framework/app_layer.hpp"
#include "controller/controller.hpp"

#include <vector>
#include <memory>
#include <mutex>

class ProgramLayer : public AppLayer
{
public:
	ProgramLayer();

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

#include "application_framework/program_layer.tpp"

#endif /* end of include guard: PROGRAM_LAYER_HPP_YCHFNYBM */
