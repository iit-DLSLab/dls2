#ifndef SIGNAL_READER_HPP
#define SIGNAL_READER_HPP

#include "dls2/msg_wrappers/signal_reader_base.hpp"

namespace dls
{
	template <typename SignalType>
	class SignalReader : public SignalReaderBase
	{
	public:
		SignalReader(std::shared_ptr<dls::DDSParticipant>, const dls::topicType&, const std::shared_ptr<SignalType>);
		~SignalReader();
		SignalReader() = delete;

		SignalType getData();

	public:
		const std::shared_ptr<SignalType> signal_;
		virtual WrapperBase* getWrapperBasePtr() override;
	};
} // end namespace dls

#include "dls2/msg_wrappers/signal_reader.tpp"

#endif /* end of include guard: SIGNAL_READER_HPP */