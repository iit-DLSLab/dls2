#include "dls2/application/state_machine/breaking_rt.hpp"
#include "dls2/application/state_machine/periodic_app_sm.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        BreakingRT::BreakingRT(dls::PeriodicApp* periodic_app, PeriodicAppSM *sm) : PeriodicAppState(periodic_app, sm) {}
        void BreakingRT::activity()
        {
            std::cout << "BreakingRT state\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            sm->nextState(sm->rt);
        }
    }
}