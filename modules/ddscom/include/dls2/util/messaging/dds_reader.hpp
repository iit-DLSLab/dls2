
#ifndef DDSREADER_HPP
#define DDSREADER_HPP

#include "dls2/util/messaging/dds_participant.hpp"


namespace dls
{
	class DDSReader : public DDSParticipant
	{
	public:
		DDSReader(
			std::string     			partName_,
			dls::domainType 			domain_,
			dls::topicType  			topic_,
			std::function<void(void *)> callback_,
			eprosima::fastdds::dds::DataReaderQos qos_ = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT
		);

		virtual ~DDSReader();

		// Check wheter the data reader has matched with at least one data writer
		bool hasMatched();

		// Get the number of matches
		int getNumOfMatches();
	private:

		eprosima::fastdds::dds::DataReader *reader;

	};
}
#endif /* end of include guard: DDSREADER_HPP */
