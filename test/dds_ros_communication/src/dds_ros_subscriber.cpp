#include "dds_ros_subscriber.hpp"

#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <thread>

using namespace eprosima::fastdds::dds;

DDSROSSubscriber::DDSROSSubscriber()
    : participant_(nullptr)
    , subscriber_(nullptr)
    , topic_(nullptr)
    , reader_(nullptr)
    , type_(new DDSROSMsgPubSubType())
{}

bool DDSROSSubscriber::init(const eprosima::fastdds::dds::DomainId_t domain_id, const std::string& topic_name)
{
    DomainParticipantQos pqos;
    pqos.name("DDSROSSubscriber");
    participant_ = DomainParticipantFactory::get_instance()->create_participant(domain_id, pqos);

    if (participant_ == nullptr)
    {
        return false;
    }

    //REGISTER THE TYPE
    type_.register_type(participant_);

    //CREATE THE SUBSCRIBER
    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);

    if (subscriber_ == nullptr)
    {
        return false;
    }

    //CREATE THE TOPIC
    topic_ = participant_->create_topic(
        topic_name,
        type_.get_type_name(),
        TOPIC_QOS_DEFAULT);

    if (topic_ == nullptr)
    {
        return false;
    }

    // CREATE THE READER
    DataReaderQos rqos = DATAREADER_QOS_DEFAULT;
    rqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    reader_ = subscriber_->create_datareader(topic_, rqos, &listener_);

    if (reader_ == nullptr)
    {
        return false;
    }

    std::cout << "DDSROSSubscriber running under DDS Domain ID: " << domain_id << std::endl;
    return true;
}

DDSROSSubscriber::~DDSROSSubscriber()
{
    if (reader_ != nullptr)
    {
        subscriber_->delete_datareader(reader_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }
    if (subscriber_ != nullptr)
    {
        participant_->delete_subscriber(subscriber_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

void DDSROSSubscriber::SubListener::on_subscription_matched(DataReader*, const SubscriptionMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        matched_ = info.total_count;
        std::cout << "Subscriber matched." << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        matched_ = info.total_count;
        std::cout << "Subscriber unmatched." << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
}

void DDSROSSubscriber::SubListener::on_data_available(DataReader* reader)
{
    SampleInfo info;
    if (reader->take_next_sample(&dds_ros_message_, &info) == ReturnCode_t::RETCODE_OK)
    {
#if (FASTRTPS_VERSION_MAJOR == 2 && FASTRTPS_VERSION_MINOR < 2)
        if (info.instance_state == ALIVE)
#else
        if (info.instance_state == ALIVE_INSTANCE_STATE)
#endif //  if (FASTRTPS_VERSION_MAJOR == 2 && FASTRTPS_VERSION_MINOR < 2)
        {
            samples_++;
            // Print your structure data here.
            std::cout << "Message received from ROS: " << dds_ros_message_.data() << std::endl;
        }
    }
}

void DDSROSSubscriber::run()
{
    std::cout << "Subscriber running. Please press enter to stop the Subscriber" << std::endl;
    std::cin.ignore();
}

void DDSROSSubscriber::run(uint32_t number)
{
    std::cout << "Subscriber running until " << number << "samples have been received" << std::endl;
    while (number > listener_.samples_)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}