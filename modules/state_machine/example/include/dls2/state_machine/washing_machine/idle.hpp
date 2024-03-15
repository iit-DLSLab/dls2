#ifndef IDLE_HPP
#define IDLE_HPP

#include "dls2/state_machine/state_machine.hpp"

namespace state_machine
{
    namespace washing_machine{
        class WashingMachine; // forward declaration

        class Idle : public State
        {
        public:
            Idle(WashingMachine *sm);
            virtual void activity() override;
            WashingMachine *sm; // state machine
        };
    }
}

#endif /* end of include guard: IDLE_HPP */