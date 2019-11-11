#ifndef LOG_LAYER_HPP_IAHZ5BZG
#define LOG_LAYER_HPP_IAHZ5BZG

#include "application_framework/app_layer.hpp"
#include "util/messaging/subscriber_base.hpp"

class LogLayer : public AppLayer
{
public:
	LogLayer();

	Status run() override;
	Status shutdown() override;
private:
};

#endif /* end of include guard: LOG_LAYER_HPP_IAHZ5BZG */
