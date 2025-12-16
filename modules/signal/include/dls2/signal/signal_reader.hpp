#ifndef SIGNAL_READER_HPP
#define SIGNAL_READER_HPP

#include "dls2/signal/signal_reader_base.hpp"

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
		
		virtual bool hasSequenceId() override;

	protected:
		const std::shared_ptr<SignalType> signal_;
		std::shared_ptr<dls::DDSSubListener> listener_;

		virtual WrapperBase* getWrapperBasePtr() override;
		const std::function<void()> auxiliary_callback;

		template <typename T, typename = int>
		struct HasSequenceId : std::false_type { };
		template <typename T>
		struct HasSequenceId <T, decltype((void) T::sequence_id, 0)> : std::true_type { };

		bool has_sequence_id_;
	};
} // end namespace dls

#include "dls2/signal/signal_reader.tpp"

#endif /* end of include guard: SIGNAL_READER_HPP */