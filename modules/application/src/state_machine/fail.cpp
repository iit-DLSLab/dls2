#include "dls2/application/state_machine/fail.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/periodic_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Fail::Fail(dls::PeriodicApp* periodic_app, AppStateMachine *sm) 
        : AppState(periodic_app, sm, "fail") {}
        void Fail::activity()
        {
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