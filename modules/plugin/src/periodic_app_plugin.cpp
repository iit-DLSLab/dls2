#include "dls2/plugin/periodic_app_plugin.hpp"

namespace dls
{
	PeriodicAppPlugin::PeriodicAppPlugin(const std::string &ID, const domainType &domain)
		: PeriodicApp(ID), Plugin(ID, domain)
	{}

	PeriodicAppPlugin::~PeriodicAppPlugin()
	{}

	bool PeriodicAppPlugin::checkActivation()
	{
		return basicActivationChecks();
	}

}