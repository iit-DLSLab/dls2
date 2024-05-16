#ifndef APP_EVENTS_HPP
#define APP_EVENTS_HPP

#include "dls2/state_machine/state_machine.hpp"

namespace state_machine
{
    namespace app{
        // DEFINE NOT ASYNCHRONOUS EVENTS BY INHERITING FROM Event CLASS
        class Initialized : public Event{};
        class FailedActivation : public Event{};
        class Activated : public Event{};
        class Deactivated : public Event{};
        class Failure : public Event{};
        class NotRT : public Event{};
        class RT : public Event{};
        // DEFINE ASYNCHRONOUS EVENTS BY INHERITING FROM AsyncEvent CLASS
        class ActivationRequest : public AsyncEvent{};
        class DeactivationRequest : public AsyncEvent{};
        class QuitRequest : public AsyncEvent{};
        class StopActivation : public AsyncEvent{};
    }
}
#endif /* end of include guard: APP_EVENTS_HPP */