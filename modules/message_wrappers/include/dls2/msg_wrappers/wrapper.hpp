#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include "dls2/msg_wrappers/wrapper_base.hpp"

#include <mutex>

namespace dls
{
	template <typename MsgType>
	class Wrapper : public WrapperBase
	{
	public:
		typedef MsgType type;

		Wrapper();
		virtual ~Wrapper();
		
        /*!
         * @brief User defined conversion function
         */
		virtual operator MsgType() const = 0;

		virtual Wrapper &operator= (const MsgType&) = 0;

		virtual void* getMsg() override;
		virtual void loadMsg(void*) override;

	protected:
		MsgType message;
	};
} // end namespace dls

#include "dls2/msg_wrappers/wrapper.tpp"

#endif /* end of include guard: WRAPPER_HPP */
