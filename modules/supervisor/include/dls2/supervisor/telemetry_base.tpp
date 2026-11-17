#pragma once

#include "dls2/supervisor/telemetry_base.hpp"

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