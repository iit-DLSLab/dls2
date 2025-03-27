#ifndef DDS_ROS_PUBLISHER
#define DDS_ROS_PUBLISHER

#include <dls_messages/dds/dds_rosPubSubTypes.hpp>

#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>

class DDSROSPublisher
{
public:

    DDSROSPublisher();

    virtual ~DDSROSPublisher();

    bool init(const eprosima::fastdds::dds::DomainId_t domain_id, const std::string& topic_name);
    bool publish(bool waitForListener = true);
    void run(const uint32_t number, const uint32_t sleep);

private:
    void runThread(const uint32_t number, const uint32_t sleep);

    eprosima::fastdds::dds::DomainParticipant* participant_;
    eprosima::fastdds::dds::Publisher* publisher_;
    eprosima::fastdds::dds::Topic* topic_;
    eprosima::fastdds::dds::DataWriter* writer_;
    eprosima::fastdds::dds::TypeSupport type_;

    DDSROSMsg hello_;
    bool stop_;

    class PubListener : public eprosima::fastdds::dds::DataWriterListener
    {
    public:
        PubListener() : matched_(0), firstConnected_(false){}
        ~PubListener() override{}

        void on_publication_matched(eprosima::fastdds::dds::DataWriter* writer,
                                    const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;

        int matched_;
        bool firstConnected_;
    } listener_;
};



#endif /* DDSROSPUBLISHER_H_ */
