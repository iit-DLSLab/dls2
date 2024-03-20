#include "dls2/application/state_machine/idle.hpp"
#include "dls2/application/state_machine/periodic_app_sm.hpp"
#include "dls2/application/periodic_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Idle::Idle(dls::PeriodicApp* periodic_app, PeriodicAppSM *sm) : PeriodicAppState(periodic_app, sm) {}
        void Idle::activity()
        {
            std::cout << "Idle state\n";
            periodic_app->setDefaultSchedulerPolicy();
            sm->waitAsynchEvent({sm->activation_request, sm->quit_request});
            if(sm->isRaised(sm->activation_request))
            {
                sm->nextState(sm->activation_request);
            }
            else if(sm->isRaised(sm->quit_request))
            {
                sm->nextState(sm->quit_request);
            }
        }
    }
}