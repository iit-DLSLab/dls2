
#ifndef DDSREADER_CPP
#define DDSREADER_CPP

#include "dls2/util/messaging/dds_reader.hpp"
#include <chrono>

namespace dls
{
	DDSReader::DDSReader(
		std::string     partName_,
		dls::domainType domain_,
		dls::topicType  topic_,
		std::function<void(void *)> callback_,
		eprosima::fastdds::dds::DataReaderQos qos_
	)
		: DDSParticipant(partName_, domain_)
	{
		
		if (callback_ != nullptr)
			this->reader = this->addReader(partName_, topic_, callback_, qos_);
	}

	DDSReader::DDSReader(
		std::string     partName_,
		dls::domainType domain_,
		eprosima::fastrtps::rtps::DiscoveryProtocol_t partType_
	)
		: DDSParticipant(partName_, domain_, partType_){
			sleep(1);
		}


	DDSReader::~DDSReader(){
		this->reader = nullptr;
	}

	bool DDSReader::run( 
		std::string topicName,
		std::function<void(void *)> callback_,
		eprosima::fastdds::dds::DataReaderQos qos_
	){

		if (callback_ != nullptr){
			if (topicFound(topicName)){
				this->reader = this->addReader("unicReader", topicName, callback_, qos_);
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}


			
	}

	void DDSReader::printDiscoveredTopics(){
		std::vector<std::string> topicList = this->getDiscoveredTopics();

		for(auto& topic: topicList){
			std::cout << "\t" << topic << std::endl;
		}

	}
	void DDSReader::printParticipants(){
	
		auto participant_vect  = this->getParticipants();
		for(auto &t: participant_vect){
			std::cout << "\t" << t << std::endl;
		}
		
	}

	bool DDSReader::hasMatched(){
		if(this->getSubListener(this->getName())->matched_count.load()>0)
			return true;
		return false;
	}


} // end namespace dls

#endif /* end of include guard: DDSREADER_CPP */
