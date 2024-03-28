#include "dls2/application/state_machine/app_state.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        AppState::AppState(
            dls::App* app,
            AppStateMachine *sm,
            const std::string name,
            bool realtime) 
        : State(name, realtime)
        , app(app)
        , sm(sm){}

        AppState::~AppState(){}
    }
}