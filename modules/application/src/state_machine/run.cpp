#include "dls2/application/state_machine/run.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Run::Run(dls::App* app, AppStateMachine *sm) 
        : AppState(app, sm, "run", true) {}
        void Run::activity()
        {
            app->run();
            //set RT scheduling policy
            // app->setRTSchedulerPolicy();
            // bool failure = false;
            // bool realtime_prec = true;
            // bool realtime_curr = realtime_prec;
            // while(      !sm->isRaised(sm->deactivation_request)
            //         &&  !sm->isRaised(sm->quit_request)
            //         &&  !failure)
            // {
            //     // Compute when the next period should start
		    //     auto next_loop_time = app->getPeriod() + std::chrono::system_clock::now();

            //     // Run
		    //     app->run(std::chrono::time_point_cast<std::chrono::system_clock::duration, std::chrono::system_clock, std::chrono::duration<double>>(std::chrono::system_clock::now()));

            //     // Check failure
            //     failure = app->checkFailure();

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

            // if (failure)
            // {
            //     sm->nextState(sm->failure);
            // }
            // else if(sm->isRaised(sm->deactivation_request))
            //     sm->nextState(sm->deactivation_request);
            // else if (sm->isRaised(sm->quit_request))
            // {
            //     app->setDefaultSchedulerPolicy();
            //     sm->nextState(sm->quit_request);
            // }
        }
    }
}