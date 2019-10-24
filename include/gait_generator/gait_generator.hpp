#ifndef GAIT_GENERATOR_HPP_5MDX0BG2
#define GAIT_GENERATOR_HPP_5MDX0BG2

#include "application_framework/components/app_layer_component.hpp"
#include "geometry/pose.hpp"

#include <memory>

class Dog;
class GaitGenerator : AppLayerComponent
{
protected:
	// TODO this is repeated in Controller. Refactor this into one location
	typedef std::chrono::duration<double, std::ratio<1, 1'000'000>> period_t;

public:
	// Typedefs
	using ID_t = std::string;
	typedef GaitGenerator *create_t();
	typedef void destroy_t(GaitGenerator*);

	// Constructors
	GaitGenerator
	(
		const std::shared_ptr<Dog>&,	///< A pointer to the robot model
		const ID_t&,					///< The ID of the gait generator
		const period_t&					///< The period of the gait generator
	);
	virtual ~GaitGenerator() = default;

protected:
	std::shared_ptr<const Dog> pRobot;
};

#endif /* end of include guard: GAIT_GENERATOR_HPP_5MDX0BG2 */
