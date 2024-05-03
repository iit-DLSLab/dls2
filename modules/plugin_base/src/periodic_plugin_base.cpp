
#include "dls2/plugin_base/periodic_plugin_base.hpp"

namespace dls
{
	PeriodicPluginBase::PeriodicPluginBase(const std::string &ID, const domainType &domain)
		: PeriodicApp(ID), PluginBase(ID, domain)
	{}

	PeriodicPluginBase::~PeriodicPluginBase()
	{}

	bool PeriodicPluginBase::checkActivation()
	{
		return basicActivationChecks();
	}

}