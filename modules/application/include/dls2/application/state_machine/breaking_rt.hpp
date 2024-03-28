#ifndef APP_BREAKING_RT_HPP
#define APP_BREAKING_RT_HPP

#include "dls2/state_machine/state_machine.hpp"
#include "dls2/application/state_machine/app_state.hpp"
namespace state_machine
{
    namespace app{
        class AppStateMachine; // forward declaration

        class BreakingRT : public AppState
        {
        public:
            BreakingRT(dls::App* app, AppStateMachine *sm);
            virtual void activity() override;
        };
    }
}

#endif /* end of include guard: APP_BREAKING_RT_HPP */