#pragma once

#include "dls2/supervisor/telemetry_base.hpp"

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

template <typename InputT, typename OutputT>
void TelemetryBase::tick(InputT& input, OutputT& output)
{
    {
        std::lock_guard<std::mutex> lock(input.mutex);
        for (auto& b : reader_bindings_) {
            b->copyToField(&input);
        };
    }
    {
        std::lock_guard<std::mutex> lock(output.mutex);
        for (auto& b : writer_bindings_) {
            b->copyFromField(&output);
        };
    }
}