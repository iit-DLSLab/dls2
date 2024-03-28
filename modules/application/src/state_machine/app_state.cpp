#include "dls2/application/state_machine/app_state.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/periodic_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        AppState::AppState(
            dls::PeriodicApp* periodic_app,
            AppStateMachine *sm,
            const std::string name,
            bool realtime) 
        : State(name, realtime)
        , periodic_app(periodic_app)
        , sm(sm){}

        AppState::~AppState(){}
    }
}