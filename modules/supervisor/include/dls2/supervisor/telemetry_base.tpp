#pragma once

#include "dls2/supervisor/telemetry_base.hpp"

#include <algorithm>
#include <iostream>

using namespace dls;

inline TelemetryBase::TelemetryBase(
                            std::vector<std::shared_ptr<dls::ReaderBase>>& readers,
                            std::vector<std::shared_ptr<dls::WriterBase>>& writers)
: readers_(readers)
, writers_(writers)
{}

template <typename MsgT, typename TargetT>
void TelemetryBase::bindReader(const ReaderPtr<MsgT>& reader, MsgT TargetT::* field) {
    reader_bindings_.push_back(std::make_unique<ReaderBinding<MsgT, TargetT>>(reader, field));
    readers_.push_back(reader);
}

template <typename MsgT, typename SourceT>
void TelemetryBase::bindWriter(const WriterPtr<MsgT>& writer, MsgT SourceT::* field) {
    writer_bindings_.push_back(std::make_unique<WriterBinding<MsgT, SourceT>>(writer, field));
    writers_.push_back(writer);
}

template <typename MsgT, typename TargetT>
ReaderPtr<MsgT> TelemetryBase::bindReader(const std::shared_ptr<dls::DDSParticipant>& participant,
                                          const dls::topicType& topic,
                                          MsgT TargetT::* field,
                                          eprosima::fastdds::dds::DataReaderQos qos)
{
    addTypeHash(qos, topic);
    auto reader = std::make_shared<dls::Reader<MsgT>>(participant, topic, [](){}, qos);
    bindReader<MsgT, TargetT>(reader, field);
    return reader;
}

template <typename QosT>
void TelemetryBase::addTypeHash(QosT& qos, const dls::topicType& topic) const
{
    const std::string type_name = rosTypeName(topic.second.get_type_name());
    const std::string hash = typeHash(type_name);
    if (hash.empty()) {
        return;
    }
    const std::string type_hash = "typehash=" + hash + ";";
    std::vector<uint8_t> user_data = qos.user_data().data_vec();
    user_data.insert(user_data.end(), type_hash.begin(), type_hash.end());
    qos.user_data().data_vec(user_data);
    std::cout << "[telemetry] " << topic.first << " (" << type_name << ") advertises " << hash << std::endl;
}

template <typename MsgT, typename SourceT>
WriterPtr<MsgT> TelemetryBase::bindWriter(const std::shared_ptr<dls::DDSParticipant>& participant,
                                          const dls::topicType& topic,
                                          MsgT SourceT::* field,
                                          eprosima::fastdds::dds::DataWriterQos qos)
{
    addTypeHash(qos, topic);
    auto writer = std::make_shared<dls::Writer<MsgT>>(participant, topic, qos);
    bindWriter<MsgT, SourceT>(writer, field);
    return writer;
}

inline void TelemetryBase::loadTypeHashes(const YAML::Node& type_hashes)
{
    if (!type_hashes || type_hashes.IsNull()) {
        return;
    }
    if (!type_hashes.IsMap()) {
        std::cerr << "[telemetry] type hashes: expected a map ROS type name -> hash, ignored" << std::endl;
        return;
    }

    static const std::string prefix = "RIHS01_";
    const auto is_valid = [](const std::string& hash) {
        return hash.size() == prefix.size() + 64 &&
               hash.compare(0, prefix.size(), prefix) == 0 &&
               std::all_of(hash.begin() + prefix.size(), hash.end(), [](char c) {
                   return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
               });
    };

    for (const auto& entry : type_hashes) {
        const auto type_name = entry.first.as<std::string>();
        const auto hash = entry.second.IsNull() ? std::string{} : entry.second.as<std::string>();
        if (hash.empty()) {
            continue;
        }
        if (!is_valid(hash)) {
            std::cerr << "[telemetry] type hash of " << type_name << ": invalid '" << hash
                      << "' (expected RIHS01_ followed by 64 lowercase hex digits), not advertised" << std::endl;
            continue;
        }
        type_hashes_[type_name] = hash;
    }
}

inline std::string TelemetryBase::typeHash(const std::string& ros_type_name) const
{
    const auto it = type_hashes_.find(ros_type_name);
    return it != type_hashes_.end() ? it->second : std::string{};
}

inline std::string TelemetryBase::rosTypeName(const std::string& dds_type_name)
{
    std::string name = dds_type_name;
    // The ROS 2 IDL mapping adds a "dds_" namespace and a trailing '_' to the type name
    static const std::string dds_namespace = "::dds_::";
    const auto dds_pos = name.find(dds_namespace);
    if (dds_pos != std::string::npos) {
        name.replace(dds_pos, dds_namespace.size(), "::");
        if (!name.empty() && name.back() == '_') {
            name.pop_back();
        }
    }
    for (auto pos = name.find("::"); pos != std::string::npos; pos = name.find("::", pos + 1)) {
        name.replace(pos, 2, "/");
    }
    return name;
}

template <typename InputT, typename OutputT>
void TelemetryBase::tick(InputT& input, OutputT& output)
{
    {
        std::lock_guard<std::mutex> lock(input.mutex);
        if (reader_bindings_.size() != readers_.size()) {
            std::cerr << "[telemetry tick] reader bindings/readers size mismatch: bindings="
                      << reader_bindings_.size() << " readers=" << readers_.size() << std::endl;
        }

        for (size_t i = 0; i < reader_bindings_.size(); ++i) {
            auto& b = reader_bindings_[i];
            if (!b || !b->isValid() || i >= readers_.size() || !readers_[i]) {
                std::cerr << "[telemetry tick] invalid reader binding at index " << i << std::endl;
                continue;
            }

            b->copyToField(&input);
        }
    }
    {
        std::lock_guard<std::mutex> lock(output.mutex);
        if (writer_bindings_.size() != writers_.size()) {
            std::cerr << "[telemetry tick] writer bindings/writers size mismatch: bindings="
                      << writer_bindings_.size() << " writers=" << writers_.size() << std::endl;
        }

        for (size_t i = 0; i < writer_bindings_.size(); ++i) {
            auto& b = writer_bindings_[i];
            if (!b || !b->isValid() || i >= writers_.size() || !writers_[i]) {
                std::cerr << "[telemetry tick] invalid writer binding at index " << i << std::endl;
                continue;
            }

            b->copyFromField(&output);
        }
    }
}