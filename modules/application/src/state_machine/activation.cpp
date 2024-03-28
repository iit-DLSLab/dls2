#include "dls2/application/state_machine/activation.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/periodic_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Activation::Activation(dls::PeriodicApp* periodic_app, AppStateMachine *sm) 
        : AppState(periodic_app, sm, "activation"){}
        void Activation::activity()
        {
            // Wait for timeout seconds the input readyness
    		double timeout = 10.0; //seconds
            double enlapsed_time = 0.0;
            bool activate = false;

            auto start = std::chrono::high_resolution_clock::now();

            // Check if the app can be activated until
            // -- either it can be activated
            // -- or the timeout is expired
            // -- or a quit request is received
            while(!activate && enlapsed_time <= timeout && !sm->isRaised(sm->quit_request)){
                // activate = periodic_app->activation();

                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                enlapsed_time = std::chrono::duration_cast<std::chrono::seconds>(
                                std::chrono::high_resolution_clock::now() - start).count();
            }

            if(activate)
                sm->nextState(sm->activated);
            else if (enlapsed_time>timeout)
                sm->nextState(sm->failed_activation);
            else // quit request
                sm->nextState(sm->quit_request);
        }
    }
}