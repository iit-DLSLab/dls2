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
		std::lock_guard<std::mutex> lock(input_info_mutex_);
		for (long unsigned int i = 0; i < input_info_.size(); i++)
		{
			input_info_[i].reader->read();
			updateInputInfo(input_info_[i]);
		}
	}

	void Plugin::read(const std::string& name)
	{
		try {
			const auto& input_idx = inputs_map.at(name);
			
			std::lock_guard<std::mutex> lock(input_info_mutex_);

			input_info_[input_idx].reader->read();
			updateInputInfo(input_info_[input_idx]);
		}
		catch (const std::out_of_range& e)
		{
			throw std::runtime_error("Input name '" + name + "' not found in the plugin inputs.");
		}
	}

	void Plugin::write()
	{
		std::lock_guard<std::mutex> lock(output_info_mutex_);
		for (long unsigned int i = 0; i < output_info_.size(); i++)
		{
			auto& output_info = output_info_[i];
			if (output_info.writer->hasTimestamp())
			{
				output_info.writer->setTimestamp(std::chrono::system_clock::now().time_since_epoch().count());
			}

			if (output_info.writer->hasSequenceId())
			{
				auto seq = output_info.nextSequenceId();
				output_info.writer->setSequenceId(seq);
			}

			output_info.writer->publish();
		}
	}

	void Plugin::write(const std::string &name)
	{
		try {
			std::lock_guard<std::mutex> lock(output_info_mutex_);
			const auto& writer = output_info_[outputs_map.at(name)].writer;
			if (writer->hasTimestamp())
			{
				writer->setTimestamp(std::chrono::system_clock::now().time_since_epoch().count());
			}
			writer->publish();
		}
		catch (const std::out_of_range& e)
		{
			throw std::runtime_error("Input name '" + name + "' not found in the plugin inputs.");
		}
	}

	bool Plugin::areOutputsUnique()
	{
		common_outputs.str("");
		std::lock_guard<std::mutex> lock(output_info_mutex_);

		// iterate over the writers, getting their topic type
		for(const auto& out_info : output_info_)
		{
			const auto& out = out_info.writer;
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

		std::lock_guard<std::mutex> lock(input_info_mutex_);

		for (long unsigned int i = 0; i < input_info_.size(); i++)
		{
			// check data availability if: all the readers needs to be checked or only the ones required on activation
			if (input_info_[i].are_inputs_required_on_activation)
			{
				if(!input_info_[i].reader->is_receiving_data())
				{
					missing_inputs << input_info_[i].reader->getTopic().first << " ";
					if(are_inputs_receiving_data){
						are_inputs_receiving_data = false;
					}
				}
			}
		}
		return are_inputs_receiving_data;
	}

  void Plugin::createReplier(const std::string &serviceName,
                             const dls::topicType &requestTopic,
                             const dls::topicType &replyTopic,
                             void* data)
  {
    _rpc_srvc_map[serviceName] = dds_participant_->createReplier(serviceName,
                                                                 requestTopic,
                                                                 replyTopic,
                                                                 data);
  }

  void Plugin::createRequester(const std::string &serviceName,
                               const dls::topicType &requestTopic,
                               const dls::topicType &replyTopic,
                               void* data)
  {
    _rpc_srvc_map[serviceName] = dds_participant_->createRequester(serviceName,
                                                                   requestTopic,
                                                                   replyTopic,
                                                                   data);
  }

  void Plugin::sendRequest(const std::string& serviceName, void* data)
  {
    _rpc_srvc_map[serviceName]->sendRequest(data);
  }

  void Plugin::sendReply(const std::string &serviceName, void* data)
  {
    _rpc_srvc_map[serviceName]->sendReply(data);
  }

  void* Plugin::getData(const std::string &serviceName)
  {
    std::shared_ptr<RpcService> srvcPtr = _rpc_srvc_map[serviceName];
    if (srvcPtr && srvcPtr->_service)
    {
      if (srvcPtr->_replier && srvcPtr->_replier->is_enabled())
      {
        int32_t ret = srvcPtr->_replier->take_request(srvcPtr->_data, srvcPtr->_info);
        if (ret == 0) return srvcPtr->_data;
      }
      else if (srvcPtr->_requester)
      {
        int32_t ret = srvcPtr->_requester->take_reply(srvcPtr->_data, srvcPtr->_info);
        if (ret == 0) return srvcPtr->_data;
      }
    }

    return NULL;
  }

  void Plugin::deleteServices()
  {
    for (auto& [key, srvcPtr] : _rpc_srvc_map)
    {
      dds_participant_->deleteService(key);
    }
    _rpc_srvc_map.clear();
  }
  
	void Plugin::updateInputInfo(InputInfo& input_info)
	{
		auto& reader = input_info.reader;
		input_info.latest_period_ms = reader->get_latest_period_ms();
		input_info.latest_timestamp = reader->get_latest_timestamp();

		bool is_sequence_id_sane = true;
		if(reader->hasSequenceId() && reader->hasStartedReceivingData())
		{
			const auto delta_sample_count = reader->getRelativeSampleCount();
			const auto reader_latest_sequence_id = reader->getLatestSequenceId();
			if(input_info.got_first_sequence_id){
				is_sequence_id_sane = checkSequenceId(input_info.latest_sequence_id, reader_latest_sequence_id, delta_sample_count);
			}else{
				input_info.got_first_sequence_id = true;
			}
			
			input_info.latest_sequence_id = reader_latest_sequence_id;
		}

		input_info.sequence_id_sane = is_sequence_id_sane;
	}

	bool Plugin::checkSequenceId(uint32_t prev_sequence_id, uint32_t received_sequence_id, int delta_sample_count)
	{
		const auto expected_sequence_id = (prev_sequence_id + delta_sample_count) % MAX_SEQUENCE_ID;
		auto sane = received_sequence_id == expected_sequence_id;
		
		return sane;
	}
}
