#ifndef APP_INITIALIZATION_HPP
#define APP_INITIALIZATION_HPP

#include "dls2/state_machine/state_machine.hpp"
#include "dls2/application/state_machine/app_state.hpp"
namespace state_machine
{
    namespace app{
        class AppStateMachine; // forward declaration

        class Initialization : public AppState
        {
        public:
            Initialization(dls::PeriodicApp* periodic_app, AppStateMachine *sm);
            virtual void activity() override;
        };
    }
}

#endif /* end of include guard: APP_INITIALIZATION_HPP */