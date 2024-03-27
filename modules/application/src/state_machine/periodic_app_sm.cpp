#include "dls2/application/state_machine/periodic_app_sm.hpp"
#include "dls2/application/periodic_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace app{
        PeriodicAppSM::PeriodicAppSM(dls::PeriodicApp* periodic_app)
            : StateMachine(periodic_app->getID())
            , INITIALIZATION(periodic_app, this)
            , IDLE(periodic_app, this)
            , ACTIVATION(periodic_app, this)
            , DEACTIVATION(periodic_app, this)
            , RUN(periodic_app, this)
            , FAIL(periodic_app, this)
            , QUIT(periodic_app, this)
            {
                std::map<std::pair<State *, Event>, State *> transitions = {
                    {{&INITIALIZATION, initialized}, &IDLE},
                    {{&IDLE, activation_request}, &ACTIVATION},
                    {{&ACTIVATION, activated}, &RUN},
                    {{&ACTIVATION, failed_activation}, &IDLE},
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