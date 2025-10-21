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
		for (long unsigned int i = 0; i < readers_.size(); i++)
		{
			inputs_[i]->setDataFromWrapperBase(readers_[i]->getWrapperBasePtr());
		}
	}

	void Plugin::read(const std::string& name)
	{
		readers_map_[name].second->setDataFromWrapperBase(readers_map_[name].first->getWrapperBasePtr());
	}

	void Plugin::write()
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

	void Plugin::write(const std::string &name)
	{
		writers_map_[name].first->setDataFromWrapperBase(writers_map_[name].second);
		if (writers_map_[name].first->hasTimestamp())
		{
			writers_map_[name].first->setTimestamp(std::chrono::system_clock::now().time_since_epoch().count());
		}
		writers_map_[name].first->publish();
	}

	bool Plugin::areOutputsUnique()
	{
		common_outputs.str("");
		// iterate over the writers, getting their topic type
		for(auto writer : writers_)
		{
			// for each writer, check if there is another writer publishing on its same topic
			// -- create reader
			const std::string reader_name = writer->getID() + "::" + "check_output_reader";
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
				common_outputs << writer->getTopic().first << " ";
				return false;
			}
		}
		return true;
	}

	bool Plugin::areInputsReceivingData()
	{
		bool are_inputs_receiving_data = true;
		missing_inputs.str("");
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
		return are_inputs_receiving_data;
	}

	std::shared_ptr<SignalReaderBase> Plugin::getReader(const std::string &name)
	{
		return readers_map_[name].first;
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

  void Plugin::updateServices()
  {
  }

  void* Plugin::getData(const std::string &serviceName)
  {
    std::shared_ptr<RpcService> srvcPtr = _rpc_srvc_map[serviceName];
    if (srvcPtr && srvcPtr->_service)
    {
      if (srvcPtr->_replier && srvcPtr->_replier->is_enabled())
      {
        dds_rpc::RequestInfo info;
        int32_t ret = srvcPtr->_replier->take_request(srvcPtr->_data, info);
        if (ret == 0) return srvcPtr->_data;
      }
      else if (srvcPtr->_requester)
      {
        dds_rpc::RequestInfo info;
        int32_t ret = srvcPtr->_requester->take_reply(srvcPtr->_data, info);
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
}
