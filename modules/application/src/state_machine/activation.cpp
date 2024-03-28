#include "dls2/application/state_machine/activation.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Activation::Activation(dls::App* app, AppStateMachine *sm) 
        : AppState(app, sm, "activation"){}
        void Activation::activity()
        {
            app->activation();
        }
    }
}