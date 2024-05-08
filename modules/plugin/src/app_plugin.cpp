#include "dls2/plugin/app_plugin.hpp"

namespace dls
{
	AppPlugin::AppPlugin(const std::string &ID, const domainType &domain)
		: App(ID), Plugin(ID, domain)
	{}

	AppPlugin::~AppPlugin()
	{}

	bool AppPlugin::checkActivation()
	{
		return basicActivationChecks();
	}

}