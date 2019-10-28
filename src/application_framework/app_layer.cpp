#include "application_framework/app_layer.hpp"
#include "util/debug/debug.hpp"

// =============================================================================
// Constructors
// =============================================================================
AppLayer::AppLayer()
	:
	components_mutex(),
	components({}),
	status_mutex(),
	status(Status::INITIALISING),
	main(0)
{ }

AppLayer::AppLayer(const std::initializer_list<pComponent_t> &_components)
	: components_mutex(),
	components(_components),
	status_mutex(),
	status(Status::INITIALISING),
	main(0)
{ }

// =============================================================================
// Member Functions
// =============================================================================
AppLayer::Status AppLayer::getStatus() const
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	return this->status;
}

void AppLayer::setStatus(Status s)
{
	std::lock_guard<std::mutex> lock(this->status_mutex);
	this->status = s;
}

AppLayer::Status AppLayer::eStop()
{
	DMSG("Application layer eStop");
	{
		std::lock_guard<std::mutex> lock(this->components_mutex);
		for(const auto pComponent : this->components)
		{
			pComponent->eStop();
		}
	}

	std::lock_guard<std::mutex> lock(this->status_mutex);
	this->status = Status::E_STOP;
	return this->status;
}
