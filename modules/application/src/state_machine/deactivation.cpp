#include "dls2/application/state_machine/deactivation.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Deactivation::Deactivation(dls::App* app, AppStateMachine *sm) 
        : AppState(app, sm, "deactivation", true) {}
        void Deactivation::activity()
        {
            app->deactivation();
            // bool deactivated = false;
            // bool realtime_prec = true;
            // bool realtime_curr = realtime_prec;
            // while(!deactivated && !sm->isRaised(sm->quit_request))
            // {
            //     // Compute when the next period should start
		    //     auto next_loop_time = app->getPeriod() + std::chrono::system_clock::now();

            //     // Run
		    //     deactivated = app->deactivation(std::chrono::time_point_cast<std::chrono::system_clock::duration, std::chrono::system_clock, std::chrono::duration<double>>(std::chrono::system_clock::now()));

            //     // Check realtime
            //     realtime_curr =  app->checkRT(next_loop_time);
            //     if  (realtime_curr!=this->realtime || 
            //         (realtime_curr==this->realtime && !realtime_prec))
            //     {
            //         sm->notifyRT(realtime_curr);
            //     }
            //     realtime_prec = realtime_curr;

            //     // Pause execution if a pause request was made
            //     if(app->isPaused())
            //         app->pauseExecution();
                
            //     // Update scheduler attributes if the current time factor has changed
            //     if(app->newTimeFactor())
            //     {
            //         app->setRTSchedulerPolicy();
            //     }

            //     sched_yield();
            // }

            // app->setDefaultSchedulerPolicy();

            // if (deactivated)
            // {
            //     sm->nextState(sm->deactivated);
            // }
            // else if (sm->isRaised(sm->quit_request))
            // {
            //     sm->nextState(sm->quit_request);
            // }
        }
    }
}