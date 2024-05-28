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
        : AppState(app, sm, "deactivation") {}
        void Deactivation::activity()
        {
            app->deactivation();
        }
    }
}