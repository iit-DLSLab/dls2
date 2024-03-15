#ifndef SPINNING_HPP
#define SPINNING_HPP

#include "dls2/state_machine/state_machine.hpp"

namespace state_machine
{
    namespace washing_machine{
        class WashingMachine; // forward declaration

        class Spinning : public State
        {
        public:
            Spinning(WashingMachine *sm);
            virtual void activity() override;
            WashingMachine *sm; // state machine
        };
    }
}

#endif /* end of include guard: SPINNING_HPP */