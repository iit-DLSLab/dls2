#ifndef APP_FAIL_HPP
#define APP_FAIL_HPP

#include "dls2/state_machine/state_machine.hpp"
#include "dls2/application/state_machine/periodic_app_state.hpp"
namespace state_machine
{
    namespace app{
        class PeriodicAppSM; // forward declaration

        class Fail : public PeriodicAppState
        {
        public:
            Fail(dls::PeriodicApp* periodic_app, PeriodicAppSM *sm);
            virtual void activity() override;
        };
    }
}

#endif /* end of include guard: APP_FAIL_HPP */