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
#ifndef PUBLISHER_BASE_HPP
#define PUBLISHER_BASE_HPP

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include <string>


/// \cond doxygen_namespace_dls
namespace dls
{
	namespace version2
	{
		class PublisherBase
		{
		public:
			PublisherBase(
				eprosima::fastdds::dds::DomainParticipant *participant_
			);

			~PublisherBase(){};

			virtual bool addDataWriter(
				std::string 	topicName_
			);

		protected:

			eprosima::fastdds::dds::DomainParticipant *participant;
		};

	} /// \endcond namespace version2
} /// \endcond namespace dls

#endif /* end of include guard: PUBLISHER_BASE_HPP */
