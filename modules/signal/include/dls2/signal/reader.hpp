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

		void read() override;

		std::chrono::steady_clock::time_point get_latest_timestamp() override;
		double get_latest_period_ms() override;

		MsgType msg;
	protected:
		std::shared_ptr<dls::DDSSubListener> listener_;
		std::function<void()> auxiliary_callback;
	};

	template <typename MsgType>
	using ReaderPtr = std::shared_ptr<Reader<MsgType>>;

} // end namespace dls

#include "dls2/signal/reader.tpp"

#endif /* end of include guard: SIGNAL_READER_HPP */