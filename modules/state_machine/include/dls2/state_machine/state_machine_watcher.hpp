#ifndef STATE_MACHINE_WATCHER_HPP
#define STATE_MACHINE_WATCHER_HPP

#include "dls2/util/messaging/dds_reader.hpp"
#include "dls2/domains/domains.hpp"
#include "dls2/topics/topics.hpp"
#include "dls2/util/messaging/dds_participant.hpp"

#include <map>
#include <memory>

namespace state_machine
{
    class StateMachineWatcher
    {
    public:
        StateMachineWatcher(const std::string &name);
        ~StateMachineWatcher();

        bool waitState(const std::string &app_name, const std::string &state) const;

        std::map<std::string, std::pair<std::string, bool>> app_states;

    private:
        dls::DDSParticipant dds_sm_watcher;
    };
}

#endif /* end of include guard: STATE_MACHINE_WATCHER_HPP */