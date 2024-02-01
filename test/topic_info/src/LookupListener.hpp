// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file TypeLookupSubscriber.h
 *
 */

#ifndef LookupSubscriber_H
#define LookupSubscriber_H

// #include <fastdds/dds/domain/DomainParticipant.hpp>
// #include <fastdds/dds/domain/DomainParticipantListener.hpp>
// #include <fastdds/dds/subscriber/DataReader.hpp>
// #include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
// #include <fastrtps/subscriber/SampleInfo.h>
// #include <fastrtps/rtps/common/Types.h>

// #include <fastrtps/types/DynamicPubSubType.h>
// #include <fastrtps/types/DynamicTypePtr.h>

// #include <fastrtps/attributes/SubscriberAttributes.h>

// #include <map>

#include <vector>
#include <string>

#include "dls2/util/messaging/dds_participant.hpp"


#include "dls2/util/messaging/dds_reader.hpp"
#include "dls_messages/dds/controller_commandPubSubTypes.h"
#include "dls_messages/dds/trunk_controller_debugPubSubTypes.h"
#include <dls_messages/dds/base_statePubSubTypes.h>
#include <dls_messages/dds/mpc_generator_outputPubSubTypes.h>
#include "Handler.hpp"
#include "utils/DataTypeConfiguration.hpp"

#include "utils/utils.hpp"
#include "utils/Exception.hpp"


namespace eprosima{
namespace fastdds{
    class LookupListener : public eprosima::fastdds::FastDdsListener
    {
        public:
            LookupListener();
            ~LookupListener();

            ////////////////////////////////////////////////////
            // FASTDDS LISTENER METHODS
            ////////////////////////////////////////////////////


            // virtual void on_string_data_read(
            //         const std::vector<types::TextDatum>& data_per_topic_value,
            //         double timestamp) override;

            bool start(unsigned int domain_id, std::vector<std::string>topic_list);

            void on_topic_discovery(
                    const std::string& topic_name,
                    const std::string& type_name,
                    bool type_registered) override;



        protected:

            ////////////////////////////////////////////////////
            // AUXILIAR METHODS
            ////////////////////////////////////////////////////

            void connect_to_domain_(
                    unsigned int domain_id);


            eprosima::fastdds::Handler fastdds_handler_;

        // private:
        

            void on_participant_discovery(
                eprosima::fastdds::dds::DomainParticipant* participant,
                eprosima::fastrtps::rtps::ParticipantDiscoveryInfo&& info);// override;

            void on_publisher_discovery(
                eprosima::fastdds::dds::DomainParticipant* participant,
                eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info); // override;


            // // void on_subscription_matched(
            // //         eprosima::fastdds::dds::DataReader* reader,
            // //         const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

            void on_type_discovery(
				eprosima::fastdds::dds::DomainParticipant* participant,
				const eprosima::fastrtps::rtps::SampleIdentity& request_sample_id,
				const eprosima::fastrtps::string_255& topic,
				const eprosima::fastrtps::types::TypeIdentifier* identifier,
				const eprosima::fastrtps::types::TypeObject* object,
				eprosima::fastrtps::types::DynamicType_ptr dyn_type); //override;


            // void on_type_information_received(
            //         eprosima::fastdds::dds::DomainParticipant* participant,
            //         const eprosima::fastrtps::string_255 topic_name,
            //         const eprosima::fastrtps::string_255 type_name,
            //         const eprosima::fastrtps::types::TypeInformation& type_information) override;

     

            eprosima::fastdds::dds::DataReader *reader;

    

    };
}
}
    // LookupSubscriber();

    // virtual ~LookupSubscriber();

    // //!Initialize the subscriber
    // bool init();

    // //!RUN the subscriber
    // void run();

    // //!Run the subscriber until number samples have been received.
    // void run(
    //         uint32_t number);

// private:

//     eprosima::fastdds::dds::DomainParticipant* mp_participant;

//     eprosima::fastdds::dds::Subscriber* mp_subscriber;

//     std::map<eprosima::fastdds::dds::DataReader*, eprosima::fastdds::dds::Topic*> topics_;

//     std::map<eprosima::fastdds::dds::DataReader*, eprosima::fastrtps::types::DynamicType_ptr> readers_;

//     std::map<eprosima::fastdds::dds::DataReader*, eprosima::fastrtps::types::DynamicData_ptr> datas_;

//     eprosima::fastrtps::SubscriberAttributes att_;

//     eprosima::fastdds::dds::DataReaderQos qos_;

// public:

//     class SubListener
//         : public eprosima::fastdds::dds::DomainParticipantListener
//     {
//     public:

 

//         // void on_data_available(
//         //         eprosima::fastdds::dds::DataReader* reader) override;

       

//         int n_matched;

//         uint32_t n_samples;

//         LookupSubscriber* subscriber_;

//         std::map<std::string, std::string> topic_type_map_;

//         eprosima::fastrtps::types::TypeInformation type_info_;
//     }
//     m_listener;

// private:

//     eprosima::fastrtps::types::DynamicPubSubType m_type;
// };

#endif /* HELLOWORLDSUBSCRIBER_H_ */
