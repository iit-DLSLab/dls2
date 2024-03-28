#include "dls2/application/state_machine/idle.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Idle::Idle(dls::App* app, AppStateMachine *sm) 
        : AppState(app, sm, "idle") {}
        void Idle::activity()
        {
            app->idle();
            // app->setDefaultSchedulerPolicy();
            // sm->waitAsynchEvent({sm->activation_request, sm->quit_request});
            // if(sm->isRaised(sm->activation_request))
            // {
            //     sm->nextState(sm->activation_request);
            // }
            // else if(sm->isRaised(sm->quit_request))
            // {
            //     sm->nextState(sm->quit_request);
            // }
        }
    }
}