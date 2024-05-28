#ifndef FILLING_WATER_HPP
#define FILLING_WATER_HPP

#include "dls2/state_machine/state_machine.hpp"

namespace state_machine
{
    namespace washing_machine{
        class WashingMachine; // forward declaration

        class FillingWater : public State
        {
        public:
            FillingWater(WashingMachine *sm);
            virtual void activity() override;
            WashingMachine *sm; // state machine
        };
    }
}

#endif /* end of include guard: FILLING_WATER_HPP */