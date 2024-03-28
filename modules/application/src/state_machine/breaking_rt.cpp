#include "dls2/application/state_machine/breaking_rt.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        BreakingRT::BreakingRT(dls::App* app, AppStateMachine *sm) : AppState(app, sm) {}
        void BreakingRT::activity()
        {
            std::cout << "BreakingRT state\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            sm->nextState(sm->rt);
        }
    }
}