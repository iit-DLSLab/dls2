#ifndef RINSING_HPP
#define RINSING_HPP

#include "dls2/state_machine/state_machine.hpp"

namespace state_machine
{
    namespace washing_machine{
        class WashingMachine; // forward declaration

        class Rinsing : public State
        {
        public:
            Rinsing(WashingMachine *sm);
            virtual void activity() override;
            WashingMachine *sm; // state machine
        };
    }
}

#endif /* end of include guard: RINSING_HPP */