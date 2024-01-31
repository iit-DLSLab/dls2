
#include "LookupListener.hpp"


namespace dls
{
	LookupListener::LookupListener(
		std::string     partName_,
		dls::domainType domain_,
		dls::topicType  topic_,
		std::function<void(void *)> callback_,
		eprosima::fastdds::dds::DataReaderQos qos_
	)
		:  DDSParticipant(partName_, domain_)
	{
		if (callback_ != nullptr)
			this->reader = this->addReader("unicReader", topic_, callback_, qos_);
	}

	LookupListener::~LookupListener(){
		this->reader = nullptr;
	}



    void LookupListener::on_participant_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastrtps::rtps::ParticipantDiscoveryInfo&& info)
    {
        static_cast<void>(participant);
        if (info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DISCOVERY_STATUS::DISCOVERED_PARTICIPANT)
        {
            // discovered_participants_info.insert({static_cast<std::string>(info.info.m_participantName), info.info.m_guid});
            std::cout << "Discovered participant " <<  static_cast<std::string>(info.info.m_participantName) << std::endl;
        }
        else if (info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DISCOVERY_STATUS::REMOVED_PARTICIPANT)
        {
            std::cout << " Deleted participant " << static_cast<std::string>(info.info.m_participantName) << std::endl;
            // discovered_participants_info.erase(static_cast<std::string>(info.info.m_participantName));
        }
    }

    void LookupListener::on_publisher_discovery(
        eprosima::fastdds::dds::DomainParticipant* participant,
        eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info)
    {
        // warning suppress
        (void)participant;

        // Only set as new topic discovered if it is ALIVE
        if (info.status == eprosima::fastrtps::rtps::WriterDiscoveryInfo::DISCOVERY_STATUS::DISCOVERED_WRITER)
        {
            // Get Topic of DataWriter discovered and set it as discovered
            std::string topic_name = info.info.topicName().to_string();
            std::string type_name = info.info.typeName().to_string();

            std::cout << "discovered topic: " << topic_name << std::endl;
            // Set Topic as discovered. If it is not new nothing happen
            // if(LookupSubscriber::is_type_registered_in_participant_(type_name))
            //     LookupSubscriber::on_topic_discovery_(topic_name, type_name);
        }
    }


    void LookupListener::on_type_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            const eprosima::fastrtps::rtps::SampleIdentity& request_sample_id,
            const eprosima::fastrtps::string_255& topic,
            const eprosima::fastrtps::types::TypeIdentifier* identifier,
            const eprosima::fastrtps::types::TypeObject* object,
            eprosima::fastrtps::types::DynamicType_ptr dyn_type)
    {
        static_cast<void>(participant); // remove compilation warnings
        static_cast<void>(request_sample_id); // remove compilation warnings
        static_cast<void>(identifier); // remove compilation warnings
        static_cast<void>(object); // remove compilation warnings
        // Create TypeSupport and register it
        eprosima::fastdds::dds::TypeSupport(
            new eprosima::fastrtps::types::DynamicPubSubType(dyn_type)).register_type(participant);

        std::cout << "Topic name is: " << topic.to_string() << "dynamic type name: " << dyn_type->get_name() << std::endl;

        // In case this callback is sent, it means that the type is already registered, so notify
        // TODO in future it would be better to update every topic in this type name, and not just the one calling here
        // on_topic_discovery_(topic.to_string(), dyn_type->get_name());
    }

    // void LookupSubscriber::on_topic_discovery_(const std::string& topic_name, const std::string& type_name)
    // {
    //     // Check if this topic has already been discovered
    //     if (discovery_database.find(topic_name) != discovery_database.end())
    //         return;

    //     discovery_database[topic_name] = type_name;

    //     // Call listener callback to notify new topic
    //     if (this->topicListener)
    //     {
    //         this->topicListener->on_topic_discovery(topic_name, type_name);
    //     }
    // }

} // end namespace dls





// // Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
// //
// // Licensed under the Apache License, Version 2.0 (the "License");
// // you may not use this file except in compliance with the License.
// // You may obtain a copy of the License at
// //
// //     http://www.apache.org/licenses/LICENSE-2.0
// //
// // Unless required by applicable law or agreed to in writing, software
// // distributed under the License is distributed on an "AS IS" BASIS,
// // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// // See the License for the specific language governing permissions and
// // limitations under the License.

// /**
//  * @file LookupSubscriber.cpp
//  *
//  */

// #include "LookupSubscriber.hpp"

// #include <chrono>
// #include <thread>

// #include <fastdds/dds/domain/DomainParticipantFactory.hpp>
// #include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
// #include <fastdds/dds/subscriber/SampleInfo.hpp>
// #include <fastdds/dds/subscriber/Subscriber.hpp>
// #include <fastrtps/attributes/ParticipantAttributes.h>
// #include <fastrtps/attributes/SubscriberAttributes.h>
// #include <fastrtps/types/DynamicDataFactory.h>
// #include <fastrtps/types/DynamicDataHelper.hpp>
// #include <fastrtps/types/TypeObjectFactory.h>
// // 
// using namespace eprosima::fastdds::dds;
// using namespace eprosima::fastrtps;
// using namespace eprosima::fastrtps::rtps;

// LookupSubscriber::LookupSubscriber()
//     : mp_participant(nullptr)
//     , mp_subscriber(nullptr)
//     , m_listener(this)
// {
// }

// bool LookupSubscriber::init()
// {
//     DomainParticipantQos pqos;
//     pqos.wire_protocol().builtin.discovery_config.discoveryProtocol = SIMPLE;
//     pqos.wire_protocol().builtin.discovery_config.use_SIMPLE_EndpointDiscoveryProtocol = true;
//     pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationReaderANDSubscriptionWriter = true;
//     pqos.wire_protocol().builtin.discovery_config.m_simpleEDP.use_PublicationWriterANDSubscriptionReader = true;
//     pqos.wire_protocol().builtin.typelookup_config.use_client = true;
//     pqos.wire_protocol().builtin.use_WriterLivelinessProtocol = false;
//     pqos.wire_protocol().builtin.discovery_config.leaseDuration = c_TimeInfinite;
//     pqos.name("Participant_sub");

//     //Do not enable entities on creation
//     DomainParticipantFactoryQos factory_qos;
//     factory_qos.entity_factory().autoenable_created_entities = false;
//     DomainParticipantFactory::get_instance()->set_qos(factory_qos);

//     StatusMask par_mask = StatusMask::subscription_matched() << StatusMask::data_available();
//     mp_participant = DomainParticipantFactory::get_instance()->create_participant(0, pqos, &m_listener, par_mask);
//     if (mp_participant == nullptr)
//     {
//         return false;
//     }
//     if (mp_participant->enable() != ReturnCode_t::RETCODE_OK)
//     {
//         DomainParticipantFactory::get_instance()->delete_participant(mp_participant);
//         return false;
//     }

//     // CREATE THE COMMON READER ATTRIBUTES
//     qos_ = DATAREADER_QOS_DEFAULT;
//     // qos_.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;
//     // qos_.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
//     // qos_.history().kind = eprosima::fastdds::dds::KEEP_LAST_HISTORY_QOS;
//     // qos_.history().depth = 30;
//     // qos_.resource_limits().max_samples = 50;
//     // qos_.resource_limits().allocated_samples = 20;

//     return true;
// }

// LookupSubscriber::~LookupSubscriber()
// {
//     for (const auto& it : topics_)
//     {
//         mp_subscriber->delete_datareader(it.first);
//         mp_participant->delete_topic(it.second);
//     }
//     if (mp_subscriber != nullptr)
//     {
//         mp_participant->delete_subscriber(mp_subscriber);
//     }

//     DomainParticipantFactory::get_instance()->delete_participant(mp_participant);
//     topics_.clear();
//     readers_.clear();
//     datas_.clear();
// }

// void LookupSubscriber::SubListener::on_data_available(
//         eprosima::fastdds::dds::DataReader* reader)
// {
//     std::cout << "data available" << std::endl;
//     auto dit = subscriber_->datas_.find(reader);

//     if (dit != subscriber_->datas_.end())
//     {
//         types::DynamicData_ptr data = dit->second;
//         SampleInfo info;
//         if (reader->take_next_sample(data.get(), &info) == ReturnCode_t::RETCODE_OK)
//         {
//             if (info.valid_data)
//             {
//                 types::DynamicType_ptr type = subscriber_->readers_[reader];
//                 this->n_samples++;
//                 std::cout << "Received data of type " << type->get_name() << std::endl;
//                 types::DynamicDataHelper::print(data);
//             }
//         }
//     }
// }

// void LookupSubscriber::SubListener::on_type_information_received(
//         eprosima::fastdds::dds::DomainParticipant*,
//         const eprosima::fastrtps::string_255 topic_name,
//         const eprosima::fastrtps::string_255 type_name,
//         const eprosima::fastrtps::types::TypeInformation& type_information)
// {   
//     std::cout << "type information discovered" << std::endl;
//     std::function<void(const std::string&, const types::DynamicType_ptr)> callback =
//             [this, topic_name](const std::string& name, const types::DynamicType_ptr type)
//             {
//                 std::cout << "Discovered type: " << name << " from topic " << topic_name << std::endl;

//                 if (subscriber_->mp_subscriber == nullptr)
//                 {
//                     //SubscriberAttributes Rparam;
//                     //Rparam = subscriber_->att_;
//                     //Rparam.topic = subscriber_->topic_;
//                     //Rparam.topic.topicName = topic_name;
//                     //Rparam.qos = subscriber_->qos_;
//                     subscriber_->mp_subscriber = subscriber_->mp_participant->create_subscriber(
//                         SUBSCRIBER_QOS_DEFAULT, nullptr);

//                     if (subscriber_->mp_subscriber == nullptr)
//                     {
//                         return;
//                     }
//                 }

//                 //CREATE THE TOPIC
//                 eprosima::fastdds::dds::Topic* topic = subscriber_->mp_participant->create_topic(
//                     "TypeLookupTopic",
//                     name,
//                     TOPIC_QOS_DEFAULT);

//                 if (topic == nullptr)
//                 {
//                     return;
//                 }

//                 StatusMask sub_mask = StatusMask::subscription_matched() << StatusMask::data_available();
//                 DataReader* reader = subscriber_->mp_subscriber->create_datareader(
//                     topic,
//                     subscriber_->qos_,
//                     &subscriber_->m_listener,
//                     sub_mask);

//                 if (type == nullptr)
//                 {
//                     const types::TypeIdentifier* ident =
//                             types::TypeObjectFactory::get_instance()->get_type_identifier_trying_complete(name);

//                     if (nullptr != ident)
//                     {
//                         const types::TypeObject* obj =
//                                 types::TypeObjectFactory::get_instance()->get_type_object(ident);

//                         types::DynamicType_ptr dyn_type =
//                                 types::TypeObjectFactory::get_instance()->build_dynamic_type(name, ident, obj);

//                         if (nullptr != dyn_type)
//                         {
//                             subscriber_->readers_[reader] = dyn_type;
//                             types::DynamicData_ptr data(
//                                 types::DynamicDataFactory::get_instance()->create_data(dyn_type));
//                             subscriber_->datas_[reader] = data;
//                         }
//                         else
//                         {
//                             std::cout << "ERROR: DynamicType cannot be created for type: " << name << std::endl;
//                         }
//                     }
//                     else
//                     {
//                         std::cout << "ERROR: TypeIdentifier cannot be retrieved for type: " << name << std::endl;
//                     }
//                 }
//                 else
//                 {
//                     subscriber_->topics_[reader] = topic;
//                     subscriber_->readers_[reader] = type;
//                     types::DynamicData_ptr data(types::DynamicDataFactory::get_instance()->create_data(type));
//                     subscriber_->datas_[reader] = data;
//                 }
//             };

//     subscriber_->mp_participant->register_remote_type(
//         type_information,
//         type_name.to_string(),
//         callback);
// }

// void LookupSubscriber::run()
// {
//     std::cout << "Subscriber running. Please press enter to stop the Subscriber" << std::endl;
//     std::cin.ignore();
// }

// void LookupSubscriber::run(
//         uint32_t number)
// {
//     std::cout << "Subscriber running until " << number << " samples have been received" << std::endl;
//     while (number > this->m_listener.n_samples)
//     {
//         std::this_thread::sleep_for(std::chrono::milliseconds(500));
//     }
// }
