#ifndef SIGNAL_READER_HPP
#define SIGNAL_READER_HPP

#include "dls2/msg_wrappers/signal_reader_base.hpp"

namespace dls
{
	template <typename SignalType>
	class SignalReader : public SignalReaderBase
	{
	public:
		friend class PeriodicPluginBase;

		SignalReader(std::shared_ptr<dls::DDSParticipant>, const dls::topicType&, const std::shared_ptr<SignalType>);
		SignalReader(std::shared_ptr<dls::DDSParticipant>, const dls::topicType&, const std::shared_ptr<SignalType>, const std::function<void()>& auxiliary_callback);
		~SignalReader();
		SignalReader() = delete;

		SignalType getData();

	protected:
		const std::shared_ptr<SignalType> signal_;
		virtual WrapperBase* getWrapperBasePtr() override;
		const std::function<void()> auxiliary_callback;
	};
} // end namespace dls

#include "dls2/msg_wrappers/signal_reader.tpp"

#endif /* end of include guard: SIGNAL_READER_HPP */