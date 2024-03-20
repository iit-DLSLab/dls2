#ifndef APP_STATE_MACHINE_HPP
#define APP_STATE_MACHINE_HPP

#include "dls2/state_machine/state_machine.hpp"
#include "dls2/application/state_machine/initialization.hpp"
#include "dls2/application/state_machine/breaking_rt.hpp"
#include "dls2/application/state_machine/activation.hpp"
#include "dls2/application/state_machine/deactivation.hpp"
#include "dls2/application/state_machine/run.hpp"
#include "dls2/application/state_machine/fail.hpp"
#include "dls2/application/state_machine/idle.hpp"
#include "dls2/application/state_machine/quit.hpp"
#include "dls2/application/state_machine/events.hpp"

namespace dls{class PeriodicApp;}// forward declaration

namespace state_machine
{
    namespace app{
        class PeriodicAppSM : public StateMachine
        {
        public:
            PeriodicAppSM(dls::PeriodicApp* periodic_app);

            // STATES
            Initialization INITIALIZATION;
            Idle IDLE;
            // BreakingRT BREAKING_RT;
            Activation ACTIVATION;
            Deactivation DEACTIVATION;
            Run RUN;
            Fail FAIL;
            Quit QUIT;

            // EVENTS
            Initialized initialized;
            Deactivated deactivated;
            FailedActivation failed_activation;
            Activated activated;
            Failure failure;
            // NotRT not_rt;
            // RT rt;

            // ASYNCHRONOUS EVENTS
            ActivationRequest activation_request;
            DeactivationRequest deactivation_request;
            QuitRequest quit_request;
        };
    }
}

#endif /* end of include guard: APP_STATE_MACHINE_HPP */