#ifndef GO_FOLD_SERVER_HPP
#define GO_FOLD_SERVER_HPP

#include "dls2/action/action_server_base.hpp"
#include "actions/goFold/msg_wrapper_types.hpp"
#include "dls2/msg_wrappers/blind_state.hpp"

namespace dls
{
	// =========================================================================
	// Go Home action server
	// =========================================================================
	/*! @brief A class implementing the Go Home action server.
	 */
	class GoFoldServer : public ActionServerBase<actions_msgs::goFold::GOAL_t, actions_msgs::goFold::FEEDBACK_t, actions_msgs::goFold::RESULT_t>
	{
	public:
		/*! @brief Constructor
		 * @param ID The ID of the action server
		 * @param pRobot A pointer to the robot object 
		 */
		GoFoldServer(const std::string &ID, const std::shared_ptr<robotlib::RobotBase> pRobot);

		/*! @brief Destructor
		 */
		~GoFoldServer() = default;

		virtual void runAction() override;
		virtual bool checkActivation() override;
	private:
		//! The robot object
		std::shared_ptr<robotlib::RobotBase> pRobot;

        //! Filter coefficient
        double fCoeff{0.003};

		//! Additional input for the action
		BlindState blind_state;

	}; // end namespace dls
}
#endif /* end of include guard: GO_FOLD_SERVER_HPP */