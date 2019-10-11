#ifndef PROGRAM_LAYER_HPP_YCHFNYBM
#define PROGRAM_LAYER_HPP_YCHFNYBM

#include "application_framework/app_layer.hpp"

class ProgramLayer : public AppLayer
{
public:
	Status run() override;
	Status shutdown() override;

private:
};

#endif /* end of include guard: PROGRAM_LAYER_HPP_YCHFNYBM */
