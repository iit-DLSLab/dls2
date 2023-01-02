/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
*******************************************************************************/
#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include <mutex>

namespace dls
{
	template <typename MsgType>
	class Wrapper
	{
	public:
		Wrapper();
		~Wrapper();
		
		virtual operator MsgType() const = 0;
		virtual Wrapper &operator= (const MsgType&) = 0;

		void* getMsg();
		void loadMsg(void*);

	protected:
		MsgType message;
		mutable std::mutex wrapper_mutex;

	};
} // end namespace dls

#include "dls2/msg_wrappers/wrapper.tpp"

#endif /* end of include guard: WRAPPER_HPP */
