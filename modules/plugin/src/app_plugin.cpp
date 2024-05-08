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

	bool AppPlugin::basicActivationChecks(){
		bool res = areInputsReceivingData() && areOutputsUnique();
		if(missing_inputs.str()!=""){
			activation_message << "Missing inputs: " << missing_inputs.str() << "\n";;
		}
		if(common_outputs.str()!=""){
			activation_message << "There is at list another writer publishing on the topics: " << common_outputs.str() << "\n";
		}
		return res;
	}
}