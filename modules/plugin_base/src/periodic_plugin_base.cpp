
#include "dls2/plugin_base/periodic_plugin_base.hpp"

namespace dls
{
	PeriodicPluginBase::PeriodicPluginBase(const std::string& ID, const domainType& domain)
	: PeriodicApp(ID)
	, active(false)
	, dds_participant_(std::make_shared<dls::DDSParticipant>(ID, domain))
	{}

	PeriodicPluginBase::~PeriodicPluginBase()
	{ }

	void PeriodicPluginBase::read()
	{
		for(long unsigned int i=0; i<readers_.size(); i++)
		{
			inputs_[i]->setDataFromWrapperBase(readers_[i]->getWrapperBasePtr());
		}
	}

	void PeriodicPluginBase::write()
	{
		for(long unsigned int i=0; i<writers_.size(); i++)
		{
			writers_[i]->setDataFromWrapperBase(outputs_[i]);
			if(writers_[i]->hasTimestamp())
			{
				writers_[i]->setTimestamp(std::chrono::system_clock::now().time_since_epoch().count());
			}
			writers_[i]->publish();
		}
	}
}