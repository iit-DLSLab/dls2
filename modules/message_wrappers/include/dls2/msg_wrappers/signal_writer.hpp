
#ifndef SIGNAL_WRITER_HPP
#define SIGNAL_WRITER_HPP

#include "dls2/msg_wrappers/signal_writer_base.hpp"

#include <type_traits>

namespace dls
{
	template <typename SignalType>
	class SignalWriter : public SignalWriterBase
	{
	public:
		friend class PeriodicPluginBase;

		SignalWriter(std::shared_ptr<dls::DDSParticipant>, const dls::topicType&, const std::shared_ptr<SignalType>);
		~SignalWriter();
		
		std::shared_ptr<SignalType> operator->();

		virtual void publish() override;

		/*!
		@brief Check if the signal stored in any child class
		*/
		virtual bool hasTimestamp() override;

		/*!
		@brief Set the timestamp of the signal stored in any child class
		@param[in] timestamp		
		*/
		virtual void setTimestamp(double timestamp) override;
	
	private:
		const std::shared_ptr<SignalType> signal_;

		template <typename T, typename = int>
		struct HasTimeStamp : std::false_type { };
		template <typename T>
		struct HasTimeStamp <T, decltype((void) T::timestamp_, 0)> : std::true_type { };

		bool has_timestamp_;

		virtual void setDataFromWrapperBase(WrapperBase* wrapper_base) override;
	};
} // end namespace dls

#include "dls2/msg_wrappers/signal_writer.tpp"

#endif /* end of include guard: SIGNAL_WRITER_HPP */
