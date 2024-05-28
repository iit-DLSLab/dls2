#include "dls2/application/state_machine/initialization.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Initialization::Initialization(dls::App* app, AppStateMachine *sm) 
        : AppState(app, sm, "initialization") {}
        void Initialization::activity(){}
    }
}