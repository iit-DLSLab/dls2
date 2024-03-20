#ifndef APP_RUN_HPP
#define APP_RUN_HPP

#include "dls2/state_machine/state_machine.hpp"
#include "dls2/application/state_machine/periodic_app_state.hpp"
namespace state_machine
{
    namespace app{
        class PeriodicAppSM; // forward declaration

        class Run : public PeriodicAppState
        {
        public:
            Run(dls::PeriodicApp* periodic_app, PeriodicAppSM *sm);
            virtual void activity() override;
        };
    }
}

#endif /* end of include guard: APP_RUN_HPP */