#include "dls2/application/state_machine/quit.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/periodic_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Quit::Quit(dls::PeriodicApp* periodic_app, AppStateMachine *sm) 
        : AppState(periodic_app, sm, "quit") {}
        void Quit::activity()
        {
            sm->stop();
        }
    }
}