#ifndef ACTION_SERVER_BASE_HPP
#define ACTION_SERVER_BASE_HPP

#include "dls2/plugin_base/periodic_plugin_base.hpp"

#include <memory>

namespace dls
{
	// =========================================================================
	// Action Server Class Declaration
	// =========================================================================
	/*! @brief A class representing a base action server that can be called asynchronously from external processes.
	 *
	 * It can be seen as a long-running service, that start running when it receives a goal, by providing feedback and a final result.
	 *
	 * \tparam GOAL_t the wrapper type used for the goal
	 * \tparam FEEDBACK_t the wrapper type used for the feedback
	 * \tparam RESULT_t the wrapper type used for the result
	 *
	 */
	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	class ActionServerBase : public PeriodicPluginBase
	{
	public:
		/*! @brief Constructor
		 *
		 * @param ID the ID of this action
		 * @param topic_goal the topic on which this action listens for goals
		 * @param topic_feedback the topic on which this action publishes feedback
		 * @param topic_result the topic on which this action publishes the result
		 * @param goal the goal wrapper pointer
		 * @param feedback the feedback wrapper pointer
		 * @param result the result wrapper pointer
		 */
		ActionServerBase(const std::string &ID,
						 const dls::topicType &topic_goal,
						 const dls::topicType &topic_feedback,
						 const dls::topicType &topic_result,
						 const GOAL_t &goal,
						 const FEEDBACK_t &feedback,
						 const RESULT_t &result);

		/*! @brief Destructor
		 */
		virtual ~ActionServerBase() = default;

		/*! @brief Print state of the action server*/
		virtual std::string where() override;

	protected:
		virtual void run(const std::chrono::system_clock::time_point &time) override;

		//! @brief The function implementing the action
		virtual void runAction() = 0;
		
		//! @brief Function starting the action
		void startAction();
		//! @brief Function stopping the action
		void stopAction();
		//! @brief Function checking if the action is stopped
		bool isActionStopped();

		//! Check if additional conditions has been satisfied to start the action
		bool initialized;

		//! Action waiting for a goal message
		GOAL_t goal;
		//! Action publishing feedback
		FEEDBACK_t feedback;
		//! Action publishing the result
		RESULT_t result;
	};

} // end namespace dls

#include "dls2/action/action_server_base.tpp"

#endif /* end of include guard: ACTION_SERVER_BASE_HPP */