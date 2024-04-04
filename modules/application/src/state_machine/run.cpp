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
        : AppState(app, sm, "run") {}
        void Run::activity()
        {
            app->run();
        }
    }
}