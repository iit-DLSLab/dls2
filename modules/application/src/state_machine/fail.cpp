#include "dls2/application/state_machine/fail.hpp"
#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        Fail::Fail(dls::App* app, AppStateMachine *sm) 
        : AppState(app, sm, "fail") {}
        void Fail::activity()
        {
            app->fail();
        }
    }
}