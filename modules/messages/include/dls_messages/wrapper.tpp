
#ifndef WRAPPER_TPP
#define WRAPPER_TPP

#include "dls_messages/wrapper.hpp"
#include <iostream>

using namespace dls;

template <typename MsgType>
Wrapper<MsgType>::Wrapper()
{ }

template <typename MsgType>
Wrapper<MsgType>::Wrapper(const Wrapper& other): message(other.message)
{ }

template <typename MsgType>
Wrapper<MsgType>::~Wrapper()
{}

template <typename MsgType>
void* Wrapper<MsgType>::getMsg()
{
    // std::cout << "getting Msg - before conversion\n";
    this->message = (MsgType) *this;
    // std::cout << "getting Msg - after conversion\n";
    return &(this->message);
}

template <typename MsgType>
void Wrapper<MsgType>::loadMsg(void* tuple)
{
    this->message = *((MsgType*) tuple);
    *this = this->message;
}
#endif /* end of include guard: WRAPPER_TPP */
