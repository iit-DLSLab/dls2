#include "dls2/application/state_machine/periodic_app_state.hpp"
#include "dls2/application/state_machine/periodic_app_sm.hpp"
#include "dls2/application/periodic_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        PeriodicAppState::PeriodicAppState(
            dls::PeriodicApp* periodic_app,
            PeriodicAppSM *sm,
            const std::string name) 
        : State(name)
        , periodic_app(periodic_app)
        , sm(sm) {}
    }
}