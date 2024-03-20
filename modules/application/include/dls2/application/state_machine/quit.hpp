#ifndef APP_QUIT_HPP
#define APP_QUIT_HPP

#include "dls2/state_machine/state_machine.hpp"
#include "dls2/application/state_machine/periodic_app_state.hpp"
namespace state_machine
{
    namespace app{
        class PeriodicAppSM; // forward declaration

        class Quit : public PeriodicAppState
        {
        public:
            Quit(dls::PeriodicApp* periodic_app, PeriodicAppSM *sm);
            virtual void activity() override;
        };
    }
}

#endif /* end of include guard: APP_QUIT_HPP */