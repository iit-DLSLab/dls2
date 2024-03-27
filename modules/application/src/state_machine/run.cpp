#include "dls2/application/state_machine/run.hpp"
#include "dls2/application/state_machine/periodic_app_sm.hpp"
#include "dls2/application/periodic_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Run::Run(dls::PeriodicApp* periodic_app, PeriodicAppSM *sm) 
        : PeriodicAppState(periodic_app, sm, "run") {}
        void Run::activity()
        {
            //set RT scheduling policy
            periodic_app->setRTSchedulerPolicy();
            bool failure = false;

            while(      !sm->isRaised(sm->deactivation_request)
                    &&  !sm->isRaised(sm->quit_request)
                    &&  !failure)
            {
                // Compute when the next period should start
		        auto next_loop_time = periodic_app->getPeriod() + std::chrono::system_clock::now();

                // Run
		        periodic_app->run(std::chrono::time_point_cast<std::chrono::system_clock::duration, std::chrono::system_clock, std::chrono::duration<double>>(std::chrono::system_clock::now()));

                // Check failure
                failure = periodic_app->checkFailure();

                // Check realtime
                periodic_app->checkRT(next_loop_time);

                // Pause execution if a pause request was made
                if(periodic_app->isPaused())
                    periodic_app->pauseExecution();
                
                // Update scheduler attributes if the current time factor has changed
                if(periodic_app->newTimeFactor())
                {
                    periodic_app->setRTSchedulerPolicy();
                }

                sched_yield();
            }

            if (failure)
            {
                sm->nextState(sm->failure);
            }
            else if(sm->isRaised(sm->deactivation_request))
                sm->nextState(sm->deactivation_request);
            else if (sm->isRaised(sm->quit_request))
            {
                periodic_app->setDefaultSchedulerPolicy();
                sm->nextState(sm->quit_request);
            }
        }
    }
}