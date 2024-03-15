
#include "dls2/plugin_base/periodic_plugin_base.hpp"

namespace dls
{
	PeriodicPluginBase::PeriodicPluginBase(const std::string &ID, const domainType &domain)
		: PeriodicApp(ID), active(false), dds_participant_(std::make_shared<dls::DDSParticipant>(ID, domain))
	{
		// Define console commands
		command_manager.addCommand("activate", "Activate " + ID, &PeriodicPluginBase::activateCommand, this, {{0, 1}}, true);
		command_manager.addCommand("deactivate", "Deactivate " + ID, &PeriodicPluginBase::deactivateCommand, this, {{1, 0}}, true);
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

	void PeriodicPluginBase::read(const std::string& name)
	{
		readers_map_[name].second->setDataFromWrapperBase(readers_map_[name].first->getWrapperBasePtr());
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

	void PeriodicPluginBase::write(const std::string &name)
	{
		writers_map_[name].first->setDataFromWrapperBase(writers_map_[name].second);
		if (writers_map_[name].first->hasTimestamp())
		{
			writers_map_[name].first->setTimestamp(std::chrono::system_clock::now().time_since_epoch().count());
		}
		writers_map_[name].first->publish();
	}

	bool PeriodicPluginBase::areOutputsUnique()
	{
		// iterate over the writers, getting their topic type
		for(auto writer : writers_)
		{
			// for each writer, check if there is another writer publishing on its same topic
			// -- create reader
			const std::string reader_name = "reader_of_"+writer->getID();
			bool is_writer_active = false;
			auto ddslink = std::make_shared<dls::DDSReader>(
			reader_name,
			dls::domains::signals,
			writer->getTopic(),
			std::function<void(void *)>
			{
				[&](void *)
				{
					std::unique_lock<std::mutex> lock(this->unique_outputs_mutex);
					is_writer_active=true;
					this->unique_outputs_cv.notify_all();
				}
			});

			// -- wait for the notification of the reader: if the time expires, it is assumed that no other writer is publishing on that topic
			std::unique_lock<std::mutex> lock(this->unique_outputs_mutex);
			this->unique_outputs_cv.wait_for(lock, ddslink->getSubListener(reader_name)->is_receiving_data_th);
			if(is_writer_active)
			{
				scout_warn << "There is at list another writer publishing on the topic "<< writer->getTopic().first << std::endl;
				return false;
			}
		}
		return true;
	}

	bool PeriodicPluginBase::areInputsReceivingData(bool check_required_on_activation)
	{
		bool are_inputs_receiving_data = true;
		std::stringstream missing_inputs("");
		for (long unsigned int i = 0; i < readers_.size(); i++)
		{
			// check data availability if: all the readers needs to be checked or only the ones required on activation
			if ((!check_required_on_activation || (check_required_on_activation && are_inputs_required_on_activation[i])))
			{
				if(!readers_[i]->is_receiving_data())
				{
					missing_inputs << readers_[i]->getTopic().first << " ";
					if(are_inputs_receiving_data)
						are_inputs_receiving_data = false;
				}
			}
		}
		if(!are_inputs_receiving_data)
		{
			scout_warn << "Missing inputs: " << missing_inputs.str() << std::endl;
		}
		return are_inputs_receiving_data;
	}

	bool PeriodicPluginBase::waitForInputs()
	{
		// Wait for timeout seconds the input readyness
		double timeout = 15;//seconds
        auto start = std::chrono::high_resolution_clock::now();
		std::stringstream missing_inputs("");
		scout_sys << "Waiting for inputs... " << std::endl;
        while(!areInputsReceivingData(true))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            auto end = std::chrono::high_resolution_clock::now();
            if(std::chrono::duration_cast<std::chrono::seconds>(end - start).count() > timeout)
            {
                scout_err << "Timeout waiting for inputs" << std::endl;
                return false;
            }
        }
		return true;
	}

	bool PeriodicPluginBase::checkActivation()
	{
		return waitForInputs();
	}

	bool PeriodicPluginBase::activateCommand()
	{   
		return activate();
	}

	bool PeriodicPluginBase::activate()
	{
		active = checkActivation();
		return active;
	}

	bool PeriodicPluginBase::deactivateCommand()
	{
		return deactivate();
	}

	bool PeriodicPluginBase::deactivate()
	{
		active = false;
		return !active;
	}
}