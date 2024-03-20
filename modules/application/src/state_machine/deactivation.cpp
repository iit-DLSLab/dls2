#include "dls2/application/state_machine/deactivation.hpp"
#include "dls2/application/state_machine/periodic_app_sm.hpp"
#include "dls2/application/periodic_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Deactivation::Deactivation(dls::PeriodicApp* periodic_app, PeriodicAppSM *sm) : PeriodicAppState(periodic_app, sm) {}
        void Deactivation::activity()
        {
            std::cout << "Deactivation state\n";
            bool deactivated = false;
            while(!deactivated && !sm->isRaised(sm->quit_request))
            {
                // Compute when the next period should start
		        auto next_loop_time = periodic_app->getPeriod() + std::chrono::system_clock::now();

                // Run
		        deactivated = periodic_app->deactivation(std::chrono::time_point_cast<std::chrono::system_clock::duration, std::chrono::system_clock, std::chrono::duration<double>>(std::chrono::system_clock::now()));

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