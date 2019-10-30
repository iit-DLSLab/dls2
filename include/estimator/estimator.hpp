#ifndef ESTIMATOR_HPP_I8Z0QRSO
#define ESTIMATOR_HPP_I8Z0QRSO

#include "todo.h"
#include <string>

#include "application_framework/components/app_layer_component.hpp"

TODO("Document this class")
class Estimator : public AppLayerComponent
{
public:
	using ID_t = std::string;
	Estimator(const ID_t&);
	virtual ~Estimator() = default;

	typedef Estimator *create_t();
	typedef void destroy_t(Estimator*);

	ID_t getID() const;

	TODO("probably temps? Need to make periodic app layer component")
	Status run() override;
	Status eStop() override;
	Status stop() override;

private:
	const ID_t ID;
};


#endif /* end of include guard: ESTIMATOR_HPP_I8Z0QRSO */
