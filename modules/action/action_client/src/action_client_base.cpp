#include "dls2/action/action_client_base.hpp"

namespace dls
{
    // =========================================================================
    // Action base Implementation
    // =========================================================================
    ActionClientBase::ActionClientBase(
        const std::string &action_name,
        const std::shared_ptr<DDSParticipant> dds_participant,
        const std::shared_ptr<CommandManager> command_manager)
        :
            action_name(action_name),
            dds_participant(dds_participant),
            command_manager(command_manager),
            is_action_completed(false)
    {
        command_manager->addCommand(  action_name,
                                            action_name + " action",
                                            &ActionClientBase::consoleCommand, this, {}, true);
    }

    bool ActionClientBase::isActionComplete()
    {
        return is_action_completed;
    }
} // end namespace dls