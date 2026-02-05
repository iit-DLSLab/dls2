
#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <list>
#include <thread>
#include <queue>

#include "dls2/signal/writer.hpp"
#include "dls2/signal/reader.hpp"

namespace dls
{
	// Type-erased binding: "copy reader's msg into some object"
	class ReaderBindingBase {
	public:
		virtual ~ReaderBindingBase() = default;
		virtual void copyToField(void* target) = 0;
	};

	template <typename MsgT, typename TargetT>
	class ReaderBinding : public ReaderBindingBase {
	public:
		using ReaderPtrT = ReaderPtr<MsgT>;

		ReaderBinding(const ReaderPtrT& reader, MsgT TargetT::* field)
			: reader_(reader), field_(field) {}

		void copyToField(void* target) override {
			auto* typed = static_cast<TargetT*>(target);
			typed->*field_ = reader_->msg;
		}

	private:
		ReaderPtrT reader_;
		MsgT TargetT::* field_;
	};

	// Type-erased binding: "copy from some object into writer's msg"
	class WriterBindingBase {
	public:
		virtual ~WriterBindingBase() = default;
		virtual void copyFromField(const void* source) = 0;
	};

	template <typename MsgT, typename SourceT>
	class WriterBinding : public WriterBindingBase {
	public:
		using WriterPtrT = WriterPtr<MsgT>;

		WriterBinding(const WriterPtrT& writer, MsgT SourceT::* field)
			: writer_(writer), field_(field) {}

		void copyFromField(const void* source) override {
			auto* typed = static_cast<const SourceT*>(source);
			writer_->msg = typed->*field_;
		}

	private:
		WriterPtrT writer_;
		MsgT SourceT::* field_;
	};

	class TelemetryBase {

	public:
		
		explicit TelemetryBase(std::vector<std::shared_ptr<dls::ReaderBase>>& readers,
                  			   std::vector<std::shared_ptr<dls::WriterBase>>& writers);

		template <typename MsgT, typename TargetT>
		void bindReader(const ReaderPtr<MsgT>& reader, MsgT TargetT::* field);

		template <typename MsgT, typename SourceT>
		void bindWriter(const WriterPtr<MsgT>& writer, MsgT SourceT::* field);

		template <typename InputT, typename OutputT>
    	void tick(InputT& input, OutputT& output);

	private:

		// References to OrchestratorBase-owned containers
		std::vector<std::shared_ptr<dls::ReaderBase>>& readers_;
		std::vector<std::shared_ptr<dls::WriterBase>>& writers_;

		std::vector<std::unique_ptr<ReaderBindingBase>> reader_bindings_;
		std::vector<std::unique_ptr<WriterBindingBase>> writer_bindings_;
	};
	
} // end namespace dls

#include "dls2/supervisor/telemetry_base.tpp"
