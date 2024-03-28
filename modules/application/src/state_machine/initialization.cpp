#include "dls2/application/state_machine/initialization.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Initialization::Initialization(dls::PeriodicApp* periodic_app, AppStateMachine *sm) 
        : AppState(periodic_app, sm, "initialization") {}
        void Initialization::activity(){}
    }
}