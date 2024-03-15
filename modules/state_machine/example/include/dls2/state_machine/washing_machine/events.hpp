#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "dls2/state_machine/state_machine.hpp"

namespace state_machine
{
    namespace washing_machine{
        // DEFINE NOT ASYNCHRONOUS EVENTS BY INHERITING FROM Event CLASS
        class Start : public Event{};
        class Filled_to_5l : public Event{};
        class Enlapsed_45_min : public Event{};
        class Enlapsed_20_min : public Event{};
        class Done : public Event{};
        // DEFINE ASYNCHRONOUS EVENTS BY INHERITING FROM AsyncEvent CLASS
        class Quit : public AsyncEvent{};
    }
}

#endif /* end of include guard: EVENTS_HPP */