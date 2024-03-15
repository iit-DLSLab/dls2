#include "dls2/state_machine/washing_machine/washing_machine.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace washing_machine{
        WashingMachine::WashingMachine()
            : StateMachine(), IDLE(this), FILLING_WATER(this), WASHING(this), RINSING(this), SPINNING(this), STOP(this)
        {
            std::map<std::pair<State *, Event>, State *> transitions = {
                {{&IDLE, start}, &FILLING_WATER},
                {{&FILLING_WATER, filled_to_5l}, &WASHING},
                {{&WASHING, enlapsed_45_min}, &RINSING},
                {{&RINSING, enlapsed_20_min}, &SPINNING},
                {{&SPINNING, done}, &STOP},
                {{&IDLE, quit}, &STOP},
                {{&FILLING_WATER, quit}, &STOP},
                {{&WASHING, quit}, &STOP},
                {{&RINSING, quit}, &STOP},
                {{&SPINNING, quit}, &STOP},   
                };
            std::vector<AsyncEvent> async_events = {quit};
            init(&IDLE, transitions, async_events);
        }
    }
}