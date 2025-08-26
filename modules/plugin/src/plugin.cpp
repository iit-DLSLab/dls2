#include "dls2/plugin/plugin.hpp"

namespace dls
{
	Plugin::Plugin(const std::string &ID, const domainType &domain)
		: dds_participant_(std::make_shared<dls::DDSParticipant>(ID, domain))
		, missing_inputs("")
		, common_outputs("") 
	{}

	Plugin::~Plugin()
	{
	}

	void Plugin::read()
	{
		for (long unsigned int i = 0; i < inputs.size(); i++)
		{
			inputs[i]->read();
		}
	}

	void Plugin::read(const std::string& name)
	{
		try {
			inputs[inputs_map.at(name)]->read();
		}
		catch (const std::out_of_range& e)
		{
			throw std::runtime_error("Input name '" + name + "' not found in the plugin inputs.");
		}
	}

	void Plugin::write()
	{
		for (long unsigned int i = 0; i < outputs.size(); i++)
		{
			if (outputs[i]->hasTimestamp())
			{
				outputs[i]->setTimestamp(std::chrono::system_clock::now().time_since_epoch().count());
			}
			outputs[i]->publish();
		}
	}

	void Plugin::write(const std::string &name)
	{
		try {
			if (outputs[outputs_map.at(name)]->hasTimestamp())
			{
				outputs[outputs_map.at(name)]->setTimestamp(std::chrono::system_clock::now().time_since_epoch().count());
			}
			outputs[outputs_map.at(name)]->publish();
		}
		catch (const std::out_of_range& e)
		{
			throw std::runtime_error("Input name '" + name + "' not found in the plugin inputs.");
		}
	}

	bool Plugin::areOutputsUnique()
	{
		common_outputs.str("");
		// iterate over the writers, getting their topic type
		for(auto out : outputs)
		{
			// for each writer, check if there is another writer publishing on its same topic
			// -- create reader
			const std::string reader_name = out->getID() + "::" + "check_output_reader";
			bool is_writer_active = false;
			auto ddslink = std::make_shared<dls::DDSReader>(
			reader_name,
			dls::domains::signals,
			out->getTopic(),
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
				common_outputs << out->getTopic().first << " ";
				return false;
			}
		}
		return true;
	}

	bool Plugin::areInputsReceivingData()
	{
		bool are_inputs_receiving_data = true;
		missing_inputs.str("");
		for (long unsigned int i = 0; i < inputs.size(); i++)
		{
			// check data availability if: all the readers needs to be checked or only the ones required on activation
			if (are_inputs_required_on_activation[i])
			{
				if(!inputs[i]->is_receiving_data())
				{
					missing_inputs << inputs[i]->getTopic().first << " ";
					if(are_inputs_receiving_data)
						are_inputs_receiving_data = false;
				}
			}
		}
		return are_inputs_receiving_data;
	}
}