#include "dls2/application/state_machine/app_state_machine.hpp"
#include "dls2/application/app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        AppStateMachine::AppStateMachine(dls::App* app)
            : StateMachine(app->getID())
            , INITIALIZATION(app, this)
            , IDLE(app, this)
            , ACTIVATION(app, this)
            , DEACTIVATION(app, this)
            , RUN(app, this)
            , FAIL(app, this)
            , QUIT(app, this)
            {
                std::map<std::pair<State *, Event>, State *> transitions = {
                    {{&INITIALIZATION, initialized}, &IDLE},
                    {{&IDLE, activation_request}, &ACTIVATION},
                    {{&ACTIVATION, activated}, &RUN},
                    {{&ACTIVATION, failed_activation}, &IDLE},
                    {{&ACTIVATION, stop_activation}, &IDLE},
                    {{&RUN, deactivation_request}, &DEACTIVATION},
                    {{&RUN, failure}, &FAIL},
                    {{&DEACTIVATION, deactivated}, &IDLE},
                    {{&FAIL, deactivation_request}, &DEACTIVATION},

                    {{&IDLE, quit_request}, &QUIT},
                    {{&ACTIVATION, quit_request}, &QUIT},
                    {{&RUN,  quit_request}, &QUIT},
                    {{&FAIL, quit_request}, &QUIT},
                    {{&DEACTIVATION, quit_request}, &QUIT},
                    };
                std::vector<AsyncEvent> async_events = {activation_request, deactivation_request, quit_request};

                init(&INITIALIZATION, transitions, async_events);
            }
    }
}