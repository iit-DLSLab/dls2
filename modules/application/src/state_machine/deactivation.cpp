#include "dls2/application/state_machine/deactivation.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/periodic_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Deactivation::Deactivation(dls::PeriodicApp* periodic_app, AppStateMachine *sm) 
        : AppState(periodic_app, sm, "deactivation", true) {}
        void Deactivation::activity()
        {
            bool deactivated = false;
            bool realtime_prec = true;
            bool realtime_curr = realtime_prec;
            while(!deactivated && !sm->isRaised(sm->quit_request))
            {
                // Compute when the next period should start
		        auto next_loop_time = periodic_app->getPeriod() + std::chrono::system_clock::now();

                // Run
		        deactivated = periodic_app->deactivation(std::chrono::time_point_cast<std::chrono::system_clock::duration, std::chrono::system_clock, std::chrono::duration<double>>(std::chrono::system_clock::now()));

                // Check realtime
                realtime_curr =  periodic_app->checkRT(next_loop_time);
                if  (realtime_curr!=this->realtime || 
                    (realtime_curr==this->realtime && !realtime_prec))
                {
                    sm->notifyRT(realtime_curr);
                }
                realtime_prec = realtime_curr;

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

            periodic_app->setDefaultSchedulerPolicy();

            if (deactivated)
            {
                sm->nextState(sm->deactivated);
            }
            else if (sm->isRaised(sm->quit_request))
            {
                sm->nextState(sm->quit_request);
            }
        }
    }
}