#pragma once

#include "dls2/util/messaging/dds_participant.hpp"

#include <functional>
#include <string>
#include <vector>

struct WriterSlot
{
    std::function<void()> write;
};

template <typename MessageT, typename PubSubTypeT>
WriterSlot make_writer(dls::DDSParticipant& participant, const std::string& writer_name, const std::string& topic_name)
{
    auto* writer = participant.addWriter(writer_name, dls::topicType(topic_name, new PubSubTypeT()));

    return WriterSlot{
        [writer]() {
            MessageT message{};
            writer->write(&message);
        }
    };
}
