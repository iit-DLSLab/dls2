
#ifndef WRAPPER_BASE_HPP
#define WRAPPER_BASE_HPP

#include <mutex>
#include <memory>

namespace dls
{
	class WrapperBase
	{
	public:
        template <typename SignalType>
		friend class SignalWriter;
		friend class PeriodicPluginBase;

		WrapperBase();
		virtual ~WrapperBase();

		virtual void* getMsg() = 0;
		virtual void loadMsg(void*) = 0;

	protected:
        /*!
         * @brief Set wrapper values. This function is used by a WrapperBase object masquerating a Wrapper one.
		 When this function is called, the input is statically casted to the same Wrapper class that is masquerated by the object calling the function.
		 * @param[in] wrapper WrapperBase pointer to a Wrapper class
         */
		virtual void setDataFromWrapperBase(WrapperBase* wrapper_base){static_cast<void>(wrapper_base);};

		mutable std::mutex wrapper_mutex;
	};
} // end namespace dls

#endif /* end of include guard: WRAPPER_BASE_HPP */