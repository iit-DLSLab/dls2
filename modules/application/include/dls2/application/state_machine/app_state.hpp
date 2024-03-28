#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "dls2/state_machine/state_machine.hpp"

namespace dls{class PeriodicApp;}// forward declaration

namespace state_machine
{
    namespace app{
        class AppStateMachine; // forward declaration
        class AppState : public State
        {
        public:
            AppState(dls::PeriodicApp* periodic_app, AppStateMachine *sm, const std::string name="", bool realtime = false);

            virtual ~AppState();

            dls::PeriodicApp* periodic_app;
            AppStateMachine* sm; // state machine
        };
    }
}

#endif /* end of include guard: APP_STATE_HPP */