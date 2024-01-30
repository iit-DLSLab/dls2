
#include "dls2/plugin_base/periodic_plugin_base.hpp"

namespace dls
{
	PeriodicPluginBase::PeriodicPluginBase(const std::string &ID, const domainType &domain)
		: PeriodicApp(ID), active(false), dds_participant_(std::make_shared<dls::DDSParticipant>(ID, domain))
	{
		// Define console commands
		command_manager.addCommand("activate", "Activate " + ID, &PeriodicPluginBase::activate, this, {{0, 1}}, true);
		command_manager.addCommand("deactivate", "Deactivate " + ID, &PeriodicPluginBase::deactivate, this, {{1, 0}}, true);
	}

	PeriodicPluginBase::~PeriodicPluginBase()
	{
	}

	void PeriodicPluginBase::read()
	{
		for (long unsigned int i = 0; i < readers_.size(); i++)
		{
			inputs_[i]->setDataFromWrapperBase(readers_[i]->getWrapperBasePtr());
		}
	}

	void PeriodicPluginBase::write()
	{
		for (long unsigned int i = 0; i < writers_.size(); i++)
		{
			writers_[i]->setDataFromWrapperBase(outputs_[i]);
			if (writers_[i]->hasTimestamp())
			{
				writers_[i]->setTimestamp(std::chrono::system_clock::now().time_since_epoch().count());
			}
			writers_[i]->publish();
		}
	}

	void PeriodicPluginBase::write(const std::string &output_name)
	{
		writers_map_[output_name].first->setDataFromWrapperBase(writers_map_[output_name].second);
		if (writers_map_[output_name].first->hasTimestamp())
		{
			writers_map_[output_name].first->setTimestamp(std::chrono::system_clock::now().time_since_epoch().count());
		}
		writers_map_[output_name].first->publish();
	}

	bool PeriodicPluginBase::activate()
	{
		active = true;
		return true;
	}

	bool PeriodicPluginBase::deactivate()
	{
		active = false;
		return true;
	}
}