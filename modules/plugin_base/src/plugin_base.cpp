#include "dls2/plugin_base/plugin_base.hpp"

namespace dls
{
	PluginBase::PluginBase(const std::string &ID, const domainType &domain)
		: dds_participant_(std::make_shared<dls::DDSParticipant>(ID, domain))
		, warner(ID)
	{}

	PluginBase::~PluginBase()
	{
	}

	void PluginBase::read()
	{
		for (long unsigned int i = 0; i < readers_.size(); i++)
		{
			inputs_[i]->setDataFromWrapperBase(readers_[i]->getWrapperBasePtr());
		}
	}

	void PluginBase::read(const std::string& name)
	{
		readers_map_[name].second->setDataFromWrapperBase(readers_map_[name].first->getWrapperBasePtr());
	}

	void PluginBase::write()
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

	void PluginBase::write(const std::string &name)
	{
		writers_map_[name].first->setDataFromWrapperBase(writers_map_[name].second);
		if (writers_map_[name].first->hasTimestamp())
		{
			writers_map_[name].first->setTimestamp(std::chrono::system_clock::now().time_since_epoch().count());
		}
		writers_map_[name].first->publish();
	}

	bool PluginBase::areOutputsUnique()
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
				warner << "There is at list another writer publishing on the topic "<< writer->getTopic().first << std::endl;
				return false;
			}
		}
		return true;
	}

	bool PluginBase::areInputsReceivingData()
	{
		bool are_inputs_receiving_data = true;
		std::stringstream missing_inputs("");
		for (long unsigned int i = 0; i < readers_.size(); i++)
		{
			// check data availability if: all the readers needs to be checked or only the ones required on activation
			if (are_inputs_required_on_activation[i])
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
			warner << "Missing inputs: " << missing_inputs.str() << std::endl;
		}
		return are_inputs_receiving_data;
	}

	bool PluginBase::basicActivationChecks(){
		return areInputsReceivingData() && areOutputsUnique();
	}

	std::shared_ptr<SignalReaderBase> PluginBase::getReader(const std::string &name)
	{
		return readers_map_[name].first;
	}
}