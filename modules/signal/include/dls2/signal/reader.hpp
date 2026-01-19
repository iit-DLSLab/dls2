#ifndef READER_HPP
#define READER_HPP

#include "dls2/signal/reader_base.hpp"

namespace dls
{
	template <typename MsgType>
	class Reader : public ReaderBase
	{
	public:
		Reader(std::shared_ptr<dls::DDSParticipant>, const dls::topicType&, const std::function<void()>& auxiliary_callback = [](){}, eprosima::fastdds::dds::DataReaderQos qos = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
		
		~Reader();
		Reader() = delete;

		bool is_receiving_data() const override;
		bool hasStartedReceivingData() override;
		bool hasHeader() override;
		bool hasSequenceId() override;
		int sampleCount() override;


		void read() override;

		std::chrono::steady_clock::time_point get_latest_timestamp() override;
		double get_latest_period_ms() override;
		virtual uint32_t getMissedSequenceIds() override;

		MsgType msg;
	protected:
		std::shared_ptr<dls::DDSSubListener> listener_;
		std::function<void()> auxiliary_callback;
	
	private:
		template <typename T, typename = std::void_t<>>
		struct HasHeader : std::false_type { };
		template <typename T>
		struct HasHeader <T, std::void_t<decltype(std::declval<T>().header())>> : std::true_type { };

		bool has_header_;

		template <typename T, typename = std::void_t<>>
		struct HasSequenceId : std::false_type { };
		template <typename T>
		struct HasSequenceId <T, std::void_t<decltype(std::declval<T>().sequence_id())>> : std::true_type { };

		bool has_sequence_id_;

		uint32_t missed_sequence_ids_{ 0 };
		unsigned long prev_sequence_id_ { 0 };
	};

	template <typename MsgType>
	using ReaderPtr = std::shared_ptr<Reader<MsgType>>;

} // end namespace dls

#include "dls2/signal/reader.tpp"

#endif /* end of include guard: SIGNAL_READER_HPP */