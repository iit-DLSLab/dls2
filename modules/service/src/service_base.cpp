
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
		while(!sm.isRaised(sm.deactivation_request) &&  !sm.isRaised(sm.quit_request))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}

		if(sm.isRaised(sm.deactivation_request))
			sm.nextState(sm.deactivation_request);
		else if (sm.isRaised(sm.quit_request))
		{
			sm.nextState(sm.quit_request);
		}

		return this->getStatus();
	}

	void ServiceBase::close(){}
}

#endif /* end of include guard: SERVICE_BASE_CPP */