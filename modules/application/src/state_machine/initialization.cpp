#include "dls2/application/state_machine/initialization.hpp"
#include "dls2/application/state_machine/periodic_app_sm.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Initialization::Initialization(dls::PeriodicApp* periodic_app, PeriodicAppSM *sm) 
        : PeriodicAppState(periodic_app, sm, "initialization") {}
        void Initialization::activity(){}
    }
}