#ifndef STAND_UP_PROCEDURE_HPP
#define STAND_UP_PROCEDURE_HPP

#include "dls2/plugin/app_plugin.hpp"
#include <dls2/msg_wrappers/blind_state.hpp> //  off-the-shelf wrapper
#include "dls2/state_machine/state_machine_watcher.hpp"

namespace procedures{
    class StandUp : public AppPlugin
    {
    public:
        StandUp(const std::string &ID, const std::shared_ptr<robotlib::RobotBase> robot);
        ~StandUp();

        AppStatus run() override;
        
        bool checkActivation() override;

        bool deactivating() override;

    private:
        BlindState blind_state;

        state_machine::StateMachineWatcher sm_watcher;
    };
}
#endif