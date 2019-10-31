#ifndef PERIODIC_APP_LAYER_COMPONENT_HPP_RY9LWBZG
#define PERIODIC_APP_LAYER_COMPONENT_HPP_RY9LWBZG

#include "application_framework/components/app_layer_component.hpp"

#include <chrono>
#include <atomic>

class PeriodicAppLayerComponent : public AppLayerComponent
{
public:
	typedef std::chrono::duration<double, std::ratio<1, 1'000'000>> period_t;
public:
	PeriodicAppLayerComponent(const period_t&);

	Status run() override;
	Status stop() override;
	virtual void run(const std::chrono::system_clock::time_point&) = 0;

private:
	const period_t period;
	std::atomic_bool should_run;
};

#endif /* end of include guard: PERIODIC_APP_LAYER_COMPONENT_HPP_RY9LWBZG */
