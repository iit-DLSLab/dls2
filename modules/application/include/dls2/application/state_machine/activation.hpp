#ifndef APP_ACTIVATION_HPP
#define APP_ACTIVATION_HPP

#include "dls2/state_machine/state_machine.hpp"
#include "dls2/application/state_machine/app_state.hpp"
namespace state_machine
{
    namespace app{
        class Activation : public AppState
        {
        public:
            Activation(dls::App* app, AppStateMachine *sm);
            virtual void activity() override;
        };
    }
}

#endif /* end of include guard: APP_ACTIVATION_HPP */