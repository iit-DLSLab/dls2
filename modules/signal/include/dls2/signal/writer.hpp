
#ifndef WRITER_HPP
#define WRITER_HPP

#include "dls2/signal/writer_base.hpp"

#include <type_traits>

namespace dls
{
	template <typename MsgType>
	class Writer : public WriterBase
	{
	public:

		Writer(std::shared_ptr<dls::DDSParticipant>, const dls::topicType&, eprosima::fastdds::dds::DataWriterQos qos = eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT);
		~Writer();
		Writer() = delete;

		virtual void publish() override;

		/*!
		@brief Check if the signal msg has a header field
		*/
		virtual bool hasHeader() override;

		/*!
		@brief Check if the signal msg has a timestamp field
		*/
		virtual bool hasTimestamp() override;

		/*!
		@brief Check if the signal msg has a sequence id field
		*/
		virtual bool hasSequenceId() override;

		/*!
		@brief Set the timestamp of the signal stored in any child class
		@param[in] timestamp		
		*/
		virtual void setTimestamp(unsigned long long timestamp) override;

		/*!
		@brief Set the timestamp of the signal stored in any child class
		@param[in] sequence_id		
		*/
		virtual void setSequenceId(uint32_t sequence_id) override;
	
		MsgType msg;
		unsigned long long prev_stamp { 0 };
	private:
		
		template <typename T, typename = std::void_t<>>
		struct HasHeader : std::false_type { };
		template <typename T>
		struct HasHeader <T, std::void_t<decltype(std::declval<T>().header())>> : std::true_type { };

		bool has_header_;

		template <typename T, typename = std::void_t<>>
		struct HasHeaderTimeStamp : std::false_type { };
		template <typename T>
		struct HasHeaderTimeStamp <T, std::void_t<decltype(std::declval<T>().header().timestamp())>> : std::true_type { };

		bool has_header_timestamp_;

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

		template <typename T, typename = std::void_t<>>
		struct HasHeaderSequenceId : std::false_type { };
		template <typename T>
		struct HasHeaderSequenceId <T, std::void_t<decltype(std::declval<T>().header().sequence_id())>> : std::true_type { };

		bool has_header_sequence_id_;

		unsigned long sequence_id_{ 0 };
		bool isSameTimestamp();
	};

	template <typename MsgType>
	using WriterPtr = std::shared_ptr<Writer<MsgType>>;
} // end namespace dls

#include "dls2/signal/writer.tpp"

#endif /* end of include guard: WRITER_HPP */
