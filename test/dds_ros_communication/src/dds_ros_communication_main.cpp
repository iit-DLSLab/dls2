#include "dds_ros_publisher.hpp"
#include "dds_ros_subscriber.hpp"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastrtps/log/Log.h>

#include <stdexcept>

#include <unistd.h>
#include <getopt.h>

using eprosima::fastdds::dds::Log;

static struct option options[] =
{
    { "mode",       required_argument, 0, 'm' },
    { "domain",     required_argument, 0, 'd' },
    { "count",      required_argument, 0, 'c' },
    { "topic_name", required_argument, 0, 'n'},
    { "help",       no_argument, 0, 'h' }
};

enum class OperationMode
{
    INVALID,
    PUBLISH,
    SUBSCRIBE
};

const std::string usage()
{
    std::ostringstream help;

    help << "Usage: DDS-ROS Communication ";
    help << "-m/--mode <publisher/subscriber> ";
    help << "-d/--domain <UNSIGNED_INTEGER> ";
    help << "-c/--count <UNSIGNED_INTEGER> ";
    help << "-n/--topic_name <STRING>";
    return help.str();
}

int main(
        int argc,
        char** argv)
{
    if (argc < 2)
    {
        std::cout << usage() << std::endl;
        return 1;
    }

    OperationMode mode(OperationMode::INVALID);
    uint32_t count = 10;
    std::string topic_name("DDSROSTopic");
    const uint32_t sleep = 100;
    eprosima::fastdds::dds::DomainId_t domain_id(0);

    while (true)
    {
        int option_index = 0;
        auto opt = getopt_long(argc, argv, "m:d:c:n:h", options, &option_index);

        if (-1 == opt)
        {
            // Reached last argument. Finish loop
            break;
        }

        switch (opt)
        {
            case 'm':
            {
                if (0 == strcmp("publisher", optarg))
                {
                    mode = OperationMode::PUBLISH;
                }
                else if (0 == strcmp("subscriber", optarg))
                {
                    mode = OperationMode::SUBSCRIBE;
                }

                if (OperationMode::INVALID == mode)
                {
                    throw std::invalid_argument("Invalid mode: please choose between 'publisher' or 'subscriber'");
                }
                break;
            }
            case 'd':
            {
                int raw_domain = atoi(optarg);
                if (raw_domain < 0)
                {
                    throw std::invalid_argument("Error while parsing provided arguments: Domain ID must be >= 0");
                }

                domain_id = static_cast<eprosima::fastdds::dds::DomainId_t>(raw_domain);
                break;
            }
            case 'c':
            {
                int raw_count = atoi(optarg);
                if (raw_count <= 0)
                {
                    throw std::invalid_argument("Topic publish count parameter must be a positive value");
                }

                count = static_cast<uint32_t>(raw_count);
                break;
            }
            case 'n':
            {
                topic_name.assign(optarg);
                break;
            }
            case 'h':
            {
                std::cout << usage() << std::endl;
                std::cout << "\t-m/--mode\tChoose between 'publisher' or 'subscriber'" << std::endl;
                std::cout << "\t-d/--domain\t(optional) Set a custom Domain ID (default: 0)" << std::endl;
                std::cout << "\t-c/--count\t(optional) Publish a specific number of messages (default: 10)" << std::endl;
                std::cout << "\t-n/--topic_name\t(optional) Publish or subscribe to a specific topic (default: DDSROSTopic)" << std::endl;
                return 0;
            }
            default:
            {
                std::cout << usage() << std::endl;
                return 1;
            }
        }
    }

    switch (mode)
    {
        case OperationMode::INVALID:
        {
            std::cout << usage() << std::endl;
            return 1;
        }
        case OperationMode::PUBLISH:
        {
            DDSROSPublisher mypub;
            if (mypub.init(domain_id, topic_name))
            {
                mypub.run(count, sleep);
            }
            break;
        }
        case OperationMode::SUBSCRIBE:
        {
            DDSROSSubscriber mysub;
            if (mysub.init(domain_id, topic_name))
            {
                mysub.run();
            }
            break;
        }
    }
    Log::Reset();
    return 0;
}
