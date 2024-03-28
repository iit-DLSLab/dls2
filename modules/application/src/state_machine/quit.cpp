#include "dls2/application/state_machine/quit.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Quit::Quit(dls::App* app, AppStateMachine *sm) 
        : AppState(app, sm, "quit") {}
        void Quit::activity()
        {
            app->quit();
            // sm->stop();
        }
    }
}