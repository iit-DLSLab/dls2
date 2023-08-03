#include "dls2/profile/profile.hpp"
#include <yaml-cpp/yaml.h>

#include <iostream>

// =============================================================================
// Using Declarations
// =============================================================================
using namespace dls;

Profile::Profile(const std::string &yaml_config)
{
	YAML::Node config = YAML::Load(yaml_config);

	YAML::Node required_estimators = config["requires"]["estimators"];
	for
	(
		auto it = required_estimators.begin();
		it != required_estimators.end();
		++it
	)
	{
		std::cout << "inserting estimator" << std::endl;
		this->required_estimators.insert(it->first.as<std::string>());
	}

	this->required_gait_generator = config["requires"]["gait_generator"].as<std::string>();

	YAML::Node required_controllers = config["requires"]["controllers"];
	for
	(
		auto it = required_controllers.begin();
		it != required_controllers.end();
		++it
	)
	{
		std::cout << "INSERTING CONTROLLER" << std::endl;
		// this->required_controllers.insert(it->first.as<std::string>());
		this->required_controllers.insert(it->as<std::string>());
	}

	// TODO parse actions, states
}
