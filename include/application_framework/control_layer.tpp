#ifndef CONTROL_LAYER_TPP_COCUF9QG
#define CONTROL_LAYER_TPP_COCUF9QG

#include "application_framework/control_layer.hpp"
#include "util/debug/debug.hpp"

#include <type_traits>
#include <dlfcn.h>
#include <sstream>
#include <stdexcept>

// =============================================================================
// Controlles
// =============================================================================
template <typename controller_t>
void ControlLayer::addController(const std::shared_ptr<controller_t> &pController)
{
	DMSG("adding controller");
	static_assert
	(
		std::is_base_of<Controller, controller_t>::value,
		"Error: controller_t must inherit from Controller"
	);

	TODO("this is the wrong mutex")
	// std::lock_guard<std::mutex> lock(this->components_mutex);
	std::lock_guard<std::mutex> lock(this->components_mutex);
	this->controllers.insert
		(
			std::pair
				<
					Controller::ID_t,
					std::shared_ptr<Controller>
				>
				(
					pController->getID(),
					std::static_pointer_cast<Controller>(pController)
				)
		);
}
// =============================================================================
// Gait Generators
// =============================================================================
template <typename generator_t>
void ControlLayer::addGaitGenerator(const std::shared_ptr<generator_t> &pGen)
{
	static_assert
	(
		std::is_base_of<GaitGenerator, generator_t>::value,
		"Error: generator_t must inherit from Gait Generator"
	);

	std::lock_guard<std::mutex> lock(this->gait_generators_mutex);
	this->generators.insert
		(
			std::pair
				<
					GaitGenerator::ID_t,
					std::shared_ptr<GaitGenerator>
				>
				(
					pGen->getID(),
					std::static_pointer_cast<GaitGenerator>(pGen)
				)
		);
}

// =============================================================================
// Utility Implementation
// =============================================================================
template <class T>
std::shared_ptr<T> ControlLayer::loadClass(const std::string &name)
{
	// std::cout << name << std::endl;
	void *T_lib = dlopen(name.c_str(), RTLD_NOW);
	// void *T_lib = dlopen(name.c_str(), RTLD_LAZY);
	if(!T_lib)
	{
		std::stringstream ss;
		ss << "Error: could not load object " << name << ": " << dlerror();
		throw std::runtime_error(ss.str());
	}

	// clear errors
	dlerror();

	typename T::create_t *create_T =
		(typename T::create_t*)dlsym(T_lib, "create");

	if(!create_T)
	{
		std::stringstream ss;
		ss	<< "Error: could not find instantiation code in " << name
			<< ". Did the module export the class?" << dlerror();
		throw std::runtime_error(ss.str());
	}

	// clear errors
	dlerror();

	typename T::destroy_t *destroy_T =
		(typename T::destroy_t*)dlsym(T_lib, "destroy");

	if(!destroy_T)
	{
		std::stringstream ss;
		ss	<< "Error: could not find destruction code in " << name
			<< ". Did the module export the class?" << dlerror();
		throw std::runtime_error(ss.str());
	}

	// clear errors
	dlerror();

	std::shared_ptr<T> pT
		(
			create_T(),		// create a pointer to be managed by the shared_ptr
			destroy_T		// use this as the deleter of the shared_ptr
		);

	return pT;
}


#endif /* end of include guard: CONTROL_LAYER_TPP_COCUF9QG */
