#ifndef ACTION_CLIENT_BASE_HPP
#define ACTION_CLIENT_BASE_HPP

#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/command/command_manager.hpp"

namespace dls
{
	// =========================================================================
	// Action CLient Class Declaration
	// =========================================================================
	/*! @brief A class representing a base client for an action client.
	 *
	 */

	class ActionClientBase
	{
	public:
		/*! @brief Constructor
		 *
		 * @param dds_participant the domain participant
		 * @param command_manager the command manager
		 */
		ActionClientBase(
			const std::string &action_name,
			const std::shared_ptr<CommandManager> command_manager);

		/*! @brief Destructor
		 */
		virtual ~ActionClientBase() = default;

		/*! @brief Funtion checking if the action has been completed*/
		bool isActionComplete();

		/*! @brief Console command used to call the action*/
		virtual bool consoleCommand() = 0;

	protected:
		//! Name of the action
		const std::string action_name;
		//! Domain participant
		const std::shared_ptr<DDSParticipant> dds_participant;
		//! Command manager
		const std::shared_ptr<CommandManager> command_manager;
		//! Boolean indicating if the action has been completed
		bool is_action_completed;
	};

} // end namespace dls

#endif /* end of include guard: ACTION_CLIENT_BASE_HPP */