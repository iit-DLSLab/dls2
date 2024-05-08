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

        /*! @brief Wait the state of an application until the stop_wait variable becomes true or the state is found
        */
        bool waitState(const std::string &app_name, const std::string &state, bool& stop_wait) const;

        /*! @brief Wait the state of an application until the stop_wait variable becomes true or the state is found
        * @details Using atomic_bool instead of bool
        */
        bool waitState(const std::string &app_name, const std::string &state, std::atomic_bool& stop_wait) const;

        /*! @brief Wait the state of an application until the stop_wait variable becomes true or the state is found
        */
        bool waitApp(const std::string &app_name, bool& stop_wait) const;

        /*! @brief Wait the state of an application until the stop_wait variable becomes true or the state is found
        * @details Using atomic_bool instead of bool
        */
        bool waitApp(const std::string &app_name, std::atomic_bool& stop_wait) const;

        bool findApp(const std::string &app_name) const;

        bool findState(const std::string &app_name, const std::string &state) const;

        std::map<std::string, std::pair<std::string, bool>> app_states;

    private:
        dls::DDSParticipant dds_sm_watcher;
    };
}

#endif /* end of include guard: STATE_MACHINE_WATCHER_HPP */