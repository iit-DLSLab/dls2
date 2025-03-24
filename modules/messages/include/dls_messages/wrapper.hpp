#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include "dls_messages/wrapper_base.hpp"

#include <mutex>

namespace dls
{
	template <typename MsgType>
	class Wrapper : public WrapperBase
	{
	public:
		typedef MsgType type;

		Wrapper();
		Wrapper(const Wrapper& other);

		virtual ~Wrapper();
		
        /*!
         * @brief User defined conversion function
         */
		virtual operator MsgType() = 0;

		virtual Wrapper &operator= (const MsgType&) = 0;

		virtual void* getMsg() override;
		virtual void loadMsg(void*) override;

	protected:
		MsgType message;
	};
} // end namespace dls

#include "dls_messages/wrapper.tpp"

#endif /* end of include guard: WRAPPER_HPP */
