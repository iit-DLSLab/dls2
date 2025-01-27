
#ifndef WRAPPER_TPP
#define WRAPPER_TPP

#include "dls2/msg_wrappers/wrapper.hpp"
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>

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
    this->message = *((MsgType*) tuple);
    *this = this->message;
}
#endif /* end of include guard: WRAPPER_TPP */
