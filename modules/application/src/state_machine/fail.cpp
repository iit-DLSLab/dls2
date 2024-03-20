#include "dls2/application/state_machine/fail.hpp"
#include "dls2/application/state_machine/periodic_app_sm.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Fail::Fail(dls::PeriodicApp* periodic_app, PeriodicAppSM *sm) : PeriodicAppState(periodic_app, sm) {}
        void Fail::activity()
        {
            std::cout << "Fail state\n";

            sm->waitAsynchEvent({sm->deactivation_request, sm->quit_request});
            if(sm->isRaised(sm->deactivation_request))
            {
                sm->nextState(sm->deactivation_request);
            }
            else if(sm->isRaised(sm->quit_request))
            {
                sm->nextState(sm->quit_request);
            }
        }
    }
}