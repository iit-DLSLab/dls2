
#ifndef WRITER_HPP
#define WRITER_HPP

#include "dls2/signal/writer_base.hpp"

#include <type_traits>

namespace dls
{
	static constexpr uint32_t MAX_SEQUENCE_ID = 1000;

	template <typename MsgType>
	class Writer : public WriterBase
	{
	public:

		Writer(std::shared_ptr<dls::DDSParticipant>, const dls::topicType&, eprosima::fastdds::dds::DataWriterQos qos = eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT);
		~Writer();
		Writer() = delete;

		virtual void publish() override;

		/*!
		@brief Check if the signal stored timestamp in any child class
		*/
		virtual bool hasTimestamp() override;

		/*!
		@brief Check if the signal stored sequence id in any child class
		*/
		virtual bool hasSequenceId() override;

		/*!
		@brief Set the timestamp of the signal stored in any child class
		@param[in] timestamp		
		*/
		virtual void setTimestamp(double timestamp) override;

		/*!
		@brief Set the timestamp of the signal stored in any child class
		@param[in] sequence_id		
		*/
		virtual void setSequenceId(uint32_t sequence_id) override;
	
		MsgType msg;
	private:
		template <typename T, typename = std::void_t<>>
		struct HasTimeStamp : std::false_type { };
		template <typename T>
		struct HasTimeStamp <T, std::void_t<decltype(std::declval<T>().timestamp())>> : std::true_type { };

		bool has_timestamp_;

		template <typename T, typename = std::void_t<>>
		struct HasSequenceId : std::false_type { };
		template <typename T>
		struct HasSequenceId <T, std::void_t<decltype(std::declval<T>().sequence_id())>> : std::true_type { };

		bool has_sequence_id_;
	};

	template <typename MsgType>
	using WriterPtr = std::shared_ptr<Writer<MsgType>>;
} // end namespace dls

#include "dls2/signal/writer.tpp"

#endif /* end of include guard: WRITER_HPP */
