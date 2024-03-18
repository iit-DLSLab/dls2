#ifndef WASHING_MACHINE_HPP
#define WASHING_MACHINE_HPP

#include "dls2/state_machine/state_machine.hpp"
#include "dls2/state_machine/washing_machine/idle.hpp"
#include "dls2/state_machine/washing_machine/filling_water.hpp"
#include "dls2/state_machine/washing_machine/washing.hpp"
#include "dls2/state_machine/washing_machine/rinsing.hpp"
#include "dls2/state_machine/washing_machine/spinning.hpp"
#include "dls2/state_machine/washing_machine/stop.hpp"
#include "dls2/state_machine/washing_machine/events.hpp"

namespace state_machine
{
    namespace washing_machine{

        // DEFINE STATE MACHINE BY INHERITING FROM StateMachine CLASS
        class WashingMachine : public StateMachine
        {
        public:
            WashingMachine();

            // STATES
            Idle IDLE;
            FillingWater FILLING_WATER;
            Washing WASHING;
            Rinsing RINSING;
            Spinning SPINNING;
            Stop STOP;

            // NOT ASYNCHRONOUS EVENTS
            Start start_filling;
            Filled_to_5l filled_to_5l;
            Enlapsed_45_min enlapsed_45_min;
            Enlapsed_20_min enlapsed_20_min;
            Done done;
            
            // ASYNCHRONOUS EVENTS
            Quit quit;
        };
    }
}

#endif /* end of include guard: WASHING_MACHINE_HPP */