#ifndef GO_HOME_CLIENT_HPP
#define GO_HOME_CLIENT_HPP

#include "dls2/action/action_client.hpp"
#include "actions/goHome/msg_wrapper_types.hpp"

namespace dls
{
	// =========================================================================
	// Go Home action server
	// =========================================================================
	/*! @brief A class implementing the Go Home action server.
	 */
	class GoHomeClient : public ActionClient<actions_msg_wrappers::goHome::GOAL_t, actions_msg_wrappers::goHome::FEEDBACK_t, actions_msg_wrappers::goHome::RESULT_t>
	{
	public:
	
		/*! @brief Constructor
		 * @param dds_participant pointer to the dds participant
		 * @param command_manager pointer to the command manager
		 * @param pRobot a pointer to the robot object 
		 */
		GoHomeClient(	const std::string &action_name,
						const std::shared_ptr<DDSParticipant> dds_participant,
						const std::shared_ptr<CommandManager> command_manager,
						const std::shared_ptr<robotlib::RobotBase> pRobot);

		/*! @brief Destructor
		 */
		~GoHomeClient() = default;

		virtual bool consoleCommand() override;
	private:
		robotlib::JointState home_configuration;
		dls::actions_msg_wrappers::goHome::GOAL_t goal;
	}; // end namespace dls
}
#endif /* end of include guard: GO_HOME_CLIENT_HPP */