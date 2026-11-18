
#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <list>
#include <map>
#include <thread>
#include <queue>

#include <yaml-cpp/yaml.h>

#include "dls2/signal/writer.hpp"
#include "dls2/signal/reader.hpp"

namespace dls
{
	// Type-erased binding: "copy reader's msg into some object"
	class ReaderBindingBase {
	public:
		virtual ~ReaderBindingBase() = default;
		virtual void copyToField(void* target) = 0;
		virtual bool isValid() const = 0;
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

		bool isValid() const override {
			return static_cast<bool>(reader_);
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
		virtual bool isValid() const = 0;
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

		bool isValid() const override {
			return static_cast<bool>(writer_);
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

		/**
		 * @brief Create a reader on topic and bind it like bindReader(reader, field). If a type hash is
		 * configured for the topic type (see loadTypeHashes), the reader advertises it in its
		 * USER_DATA QoS as "typehash=<hash>;", the rmw_fastrtps_cpp format: without it, ROS 2 and
		 * zenoh-bridge peers see an invalid type hash and rmw_zenoh publishers do not match.
		 */
		template <typename MsgT, typename TargetT>
		ReaderPtr<MsgT> bindReader(const std::shared_ptr<dls::DDSParticipant>& participant,
		                           const dls::topicType& topic,
		                           MsgT TargetT::* field,
		                           eprosima::fastdds::dds::DataReaderQos qos = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);

		template <typename MsgT, typename SourceT>
		void bindWriter(const WriterPtr<MsgT>& writer, MsgT SourceT::* field);

		/**
		 * @brief Create a writer on topic and bind it like bindWriter(writer, field). If a type hash is
		 * configured for the topic type (see loadTypeHashes), the writer advertises it in its
		 * USER_DATA QoS as "typehash=<hash>;", the rmw_fastrtps_cpp format, so ROS 2 and
		 * zenoh-bridge peers see the type hash of the topic.
		 */
		template <typename MsgT, typename SourceT>
		WriterPtr<MsgT> bindWriter(const std::shared_ptr<dls::DDSParticipant>& participant,
		                           const dls::topicType& topic,
		                           MsgT SourceT::* field,
		                           eprosima::fastdds::dds::DataWriterQos qos = eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT);

		/**
		 * @brief Load the ROS 2 type hashes advertised by the readers and writers created with
		 * bindReader/bindWriter(participant, topic, field). Call it before binding them.
		 * @param type_hashes YAML map ROS type name -> hash, e.g.
		 *   dls2_interface/msg/DlsStatus: "RIHS01_<64 lowercase hex>"
		 * as printed by `ros2 run dls2_interface print_hashes`. A missing node is fine, empty values
		 * are ignored, malformed ones are reported and ignored.
		 */
		void loadTypeHashes(const YAML::Node& type_hashes);

		// Type hash configured for a ROS type name ("pkg/msg/Name"), empty if none
		std::string typeHash(const std::string& ros_type_name) const;

		// DDS type name of a ROS 2 message ("pkg::msg::dds_::Name_") -> ROS type name ("pkg/msg/Name")
		static std::string rosTypeName(const std::string& dds_type_name);

		template <typename InputT, typename OutputT>
    	void tick(InputT& input, OutputT& output);

		/**
		 * @brief Append "typehash=<hash>;" to the USER_DATA of a DataReaderQos or DataWriterQos, if a
		 * hash is configured for the topic type (see loadTypeHashes). For readers and writers that
		 * are not created by bindReader/bindWriter, e.g. raw DDSParticipant readers.
		 */
		template <typename QosT>
		void addTypeHash(QosT& qos, const dls::topicType& topic) const;

	private:

		// References to OrchestratorBase-owned containers
		std::vector<std::shared_ptr<dls::ReaderBase>>& readers_;
		std::vector<std::shared_ptr<dls::WriterBase>>& writers_;

		std::vector<std::unique_ptr<ReaderBindingBase>> reader_bindings_;
		std::vector<std::unique_ptr<WriterBindingBase>> writer_bindings_;

		std::map<std::string, std::string> type_hashes_; // ROS type name -> RIHS01 hash
	};
	
} // end namespace dls

#include "dls2/supervisor/telemetry_base.tpp"
