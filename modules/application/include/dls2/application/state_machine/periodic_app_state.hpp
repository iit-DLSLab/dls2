#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "dls2/state_machine/state_machine.hpp"

namespace dls{class PeriodicApp;}// forward declaration

namespace state_machine
{
    namespace app{
        class PeriodicAppSM; // forward declaration
        class PeriodicAppState : public State
        {
        public:
            PeriodicAppState(dls::PeriodicApp* periodic_app, PeriodicAppSM *sm, const std::string name="");
            dls::PeriodicApp* periodic_app;
            PeriodicAppSM* sm; // state machine
        };
    }
}

#endif /* end of include guard: APP_STATE_HPP */