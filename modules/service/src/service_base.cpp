
#ifndef SERVICE_BASE_CPP
#define SERVICE_BASE_CPP

#include "dls2/service/service_base.hpp"

namespace dls
{
	// =========================================================================
	// Service Implementation
	// =========================================================================
	ServiceBase::ServiceBase(std::string& ID)
	    : App(ID)
		, should_quit(false)
	{ }

    AppStatus ServiceBase::run()
	{
		while(!this->should_quit)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		return this->getStatus();
	}

	AppStatus ServiceBase::stop()
	{
		this->should_quit = true;

		return this->getStatus();
	}
}

#endif /* end of include guard: SERVICE_BASE_CPP */