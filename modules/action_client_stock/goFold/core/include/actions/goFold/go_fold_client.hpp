#ifndef GO_FOLD_CLIENT_HPP
#define GO_FOLD_CLIENT_HPP

#include "dls2/action/action_client.hpp"
#include "actions/goFold/msg_wrapper_types.hpp"

namespace dls
{
	// =========================================================================
	// Go Fold action server
	// =========================================================================
	/*! @brief A class implementing the Go Fold action server.
	 */
	class GoFoldClient : public ActionClient<actions_msgs::goFold::GOAL_t, actions_msgs::goFold::FEEDBACK_t, actions_msgs::goFold::RESULT_t>
	{
	public:
	
		/*! @brief Constructor
		 * @param command_manager pointer to the command manager
		 * @param pRobot a pointer to the robot object 
		 */
		GoFoldClient(	const std::string &action_name,
						const std::shared_ptr<CommandManager> command_manager,
						const std::shared_ptr<robotlib::RobotBase> pRobot);

		/*! @brief Destructor
		 */
		~GoFoldClient() = default;

		virtual bool consoleCommand() override;
	private:
		robotlib::JointState fold_configuration;
		dls::actions_msgs::goFold::GOAL_t goal;
	}; // end namespace dls
}
#endif /* end of include guard: GO_FOLD_CLIENT_HPP */