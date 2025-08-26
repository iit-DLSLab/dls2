#include "dls2/signal/writer_base.hpp"

namespace dls
{
	WriterBase::WriterBase(std::shared_ptr<dls::DDSParticipant> dds_participant, const dls::topicType& topic)
		: SignalBase(dds_participant, topic)
	{ }

	WriterBase::~WriterBase(){}

	std::vector<std::string> WriterBase::getMatchedReaders()
	{
		// Get discovered participant info
		auto discovered_participants_info = dds_participant_->getDiscoveredParticipantsInfo();
		// Get writer listener
		auto command_publisher_listener = dds_participant_->getPubListener(ID_);
		if(command_publisher_listener == nullptr)
			return {};
		// Get matched datareaders instances
		auto matched_datareaders_instances = command_publisher_listener->matched_datareaders_instances;
		// Find the domain participant name associated to each matched data reader, and save the name (corresponding to the command name)
		std::vector<std::string> data_readers;
		for(auto datareader_instance : matched_datareaders_instances)
		{
			for(auto participant_info : discovered_participants_info)
			{
				if(participant_info.second.guidPrefix == eprosima::fastdds::rtps::iHandle2GUID(datareader_instance).guidPrefix)
				{
					data_readers.push_back(participant_info.first);
				}
			}
		}
		return data_readers;
	}
}