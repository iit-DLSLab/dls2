#ifndef WASHING_HPP
#define WASHING_HPP

#include "dls2/state_machine/state_machine.hpp"

namespace state_machine
{
    namespace washing_machine{
        class WashingMachine; // forward declaration

        class Washing : public State
        {
        public:
            Washing(WashingMachine *sm);
            virtual void activity() override;
            WashingMachine *sm; // state machine
        };
    }
}

#endif /* end of include guard: WASHING_HPP */