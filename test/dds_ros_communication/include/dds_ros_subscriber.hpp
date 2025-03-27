#ifndef DDS_ROS_SUBSCRIBER
#define DDS_ROS_SUBSCRIBER

#include <dls_messages/dds/dds_rosPubSubTypes.hpp>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastrtps/subscriber/SampleInfo.h>
#include <fastdds/dds/core/status/SubscriptionMatchedStatus.hpp>

class DDSROSSubscriber
{
public:
    DDSROSSubscriber();
    virtual ~DDSROSSubscriber();

    bool init(const eprosima::fastdds::dds::DomainId_t domain_id, const std::string& topic_name);
    void run();
    void run(uint32_t number);
private:
    eprosima::fastdds::dds::DomainParticipant* participant_;
    eprosima::fastdds::dds::Subscriber* subscriber_;
    eprosima::fastdds::dds::Topic* topic_;
    eprosima::fastdds::dds::DataReader* reader_;
    eprosima::fastdds::dds::TypeSupport type_;

    class SubListener : public eprosima::fastdds::dds::DataReaderListener
    {
    public:
        SubListener() : matched_(0), samples_(0){}
        ~SubListener() override{}

        void on_data_available(eprosima::fastdds::dds::DataReader* reader) override;
        void on_subscription_matched(eprosima::fastdds::dds::DataReader* reader,
                                     const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

        DDSROSMsg dds_ros_message_;
        int matched_;
        uint32_t samples_;
    } listener_;
};

#endif