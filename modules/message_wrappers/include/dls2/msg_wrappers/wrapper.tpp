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
#ifndef WRAPPER_TPP
#define WRAPPER_TPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include <fastrtps/types/DynamicDataFactory.h>

using namespace dls;

template <typename MsgType>
Wrapper<MsgType>::Wrapper()
{ }

template <typename MsgType>
Wrapper<MsgType>::~Wrapper()
{}

template <typename MsgType>
void* Wrapper<MsgType>::getMsg()
{
    this->message = (MsgType) *this;
    return &(this->message);
}

template <typename MsgType>
void Wrapper<MsgType>::loadMsg(void* tuple)
{
    // eprosima::fastrtps::types::DynamicData* data = (eprosima::fastrtps::types::DynamicData*) tuple;
	// std::cout << "$$$ " << data->get_string_value(2) << std::endl;

    this->message = *((MsgType*) tuple);

    // eprosima::fastrtps::types::DynamicData* data_2 = (eprosima::fastrtps::types::DynamicData*) (&(this->message));

    // *this = this->message;
}
		
#endif /* end of include guard: WRAPPER_TPP */
