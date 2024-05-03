#ifndef SIGNAL_READER_HPP
#define SIGNAL_READER_HPP

#include "dls2/msg_wrappers/signal_reader_base.hpp"

namespace dls
{
	template <typename SignalType>
	class SignalReader : public SignalReaderBase
	{
	public:
		friend class Plugin;

		SignalReader(std::shared_ptr<dls::DDSParticipant>, const dls::topicType&, const std::shared_ptr<SignalType>, const std::function<void()>& auxiliary_callback = [](){}, eprosima::fastdds::dds::DataReaderQos qos = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT);
		
		~SignalReader();
		SignalReader() = delete;

		SignalType getData();

		bool is_receiving_data() const override;

	protected:
		const std::shared_ptr<SignalType> signal_;
		std::shared_ptr<dls::DDSSubListener> listener_;

		virtual WrapperBase* getWrapperBasePtr() override;
		const std::function<void()> auxiliary_callback;
	};
} // end namespace dls

#include "dls2/msg_wrappers/signal_reader.tpp"

#endif /* end of include guard: SIGNAL_READER_HPP */