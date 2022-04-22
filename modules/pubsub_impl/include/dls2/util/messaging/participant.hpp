/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef PARTICIPANT_HPP_4d198a8c_fdc4_4c7d_8be9_6d4b7e6bd7d2
#define PARTICIPANT_HPP_4d198a8c_fdc4_4c7d_8be9_6d4b7e6bd7d2

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>

#include "dls2/util/messaging/subscriber.hpp"
#include "dls2/util/messaging/publisher.hpp"

#include "dls2/topics/topics.hpp"
#include "dls2/domains/domains.hpp"


#include <map>
#include <string>

/// \cond doxygen_namespace_dls
namespace dls
{
	class DDSParticipant{

	public:
		DDSParticipant(
			std::string 	partName_,
			dls::domainType domain_
		);

		~DDSParticipant();

		std::vector<std::string> getParticipants();

		bool addWriter(dls::topicType topic_);

		eprosima::fastdds::dds::DataReader *addReader(
			dls::topicType				topicData_,
			std::function<void(void *)>	callback
		);

		void sendMessage(void *msg);

	private:
		eprosima::fastdds::dds::DomainParticipant  				*participant;
		std::map<std::string, eprosima::fastdds::dds::Topic *>  topics;	

		dls::version2::Subscriber	*subscriber;
		dls::version2::Publisher	*publisher;

		eprosima::fastdds::dds::Topic* addTopic(dls::topicType topicData_);
	};
	
} // namespace dls
/// \endcond

#include "dls2/util/messaging/participant.tpp"

#endif // PARTICIPANT_HPP_4d198a8c_fdc4_4c7d_8be9_6d4b7e6bd7d2