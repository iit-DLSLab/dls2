#include "dls2/application/state_machine/quit.hpp"
#include "dls2/application/state_machine/periodic_app_sm.hpp"
#include "dls2/application/periodic_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Quit::Quit(dls::PeriodicApp* periodic_app, PeriodicAppSM *sm) 
        : PeriodicAppState(periodic_app, sm, "quit") {}
        void Quit::activity()
        {
            sm->stop();
        }
    }
}