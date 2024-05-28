#ifndef ACTION_CLIENT_HPP
#define ACTION_CLIENT_HPP

#include "dls2/topics/utils.hpp"
#include "dls2/msg_wrappers/signal_writer.hpp"
#include "dls2/msg_wrappers/signal_reader.hpp"
#include "dls2/util/messaging/dds_participant.hpp"
#include "dls2/command/command_manager.hpp"
#include "dls2/action/action_client_base.hpp"

#include <memory>
#include <yaml-cpp/yaml.h>

namespace dls
{
	// =========================================================================
	// Action CLient Class Declaration
	// =========================================================================
	/*! @brief A class representing a client for an action server.
	 *
	 * It can send goals to an action server, and receive feedback and results.
	 *
	 * \tparam GOAL_t the wrapper type used for the goal
	 * \tparam FEEDBACK_t the wrapper type used for the feedback
	 * \tparam RESULT_t the wrapper type used for the result
	 *
	 */
	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	class ActionClient : public ActionClientBase
	{
	public:
		/*! @brief Constructor
		 *
		 * @param action_name the name of the action
		 * @param topic_goal the topic on which this action publishes goals
		 * @param topic_feedback the topic on which this action listens feedback
		 * @param topic_result the topic on which this action listens the result
		 * @param goal the goal wrapper pointer
		 * @param feedback the feedback wrapper pointer
		 * @param result the result wrapper pointer
		 */
		ActionClient(
			const std::string &action_name,
			const std::shared_ptr<CommandManager> command_manager,
			const dls::topicType &topic_goal,
			const dls::topicType &topic_feedback,
			const dls::topicType &topic_result,
			const GOAL_t &goal,
			const FEEDBACK_t &feedback,
			const RESULT_t &result);

		/*! @brief Destructor
		 */
		virtual ~ActionClient() = default;

		/*! @brief Function sending a goal to the action server
		 *
		 * @param[in] goal the goal to send
		 */
		void sendGoal(const GOAL_t &goal);

		/*! @brief Function waiting for the result from the action server
		 *
		 * @return result the result from the action server
		 */
		RESULT_t waitForResult();

		/*! @brief Function getting the feedback from the action server
		 *
		 * @return feedback the feedback from the action server
		 */
		FEEDBACK_t getFeedback();

		// /*! @brief Calling an arbitrary action, waiting for the result - TODO*/
		// RESULT_t callAction(const GOAL_t& goal);

		/*! @brief Calling an arbitrary action*/
		void callAction(const GOAL_t& goal);

	protected:
        //! Config variable to load action settings
		YAML::Node config;
	private:
		//! Inputs
		SignalWriter<GOAL_t> goal_writer;
		//! Outputs
		SignalReader<FEEDBACK_t> feedback_reader;
		SignalReader<RESULT_t> result_reader;

		//! Mutex to wait the action to be completed
		std::mutex action_mutex;
		//! Mutex to notify that the action is completed
		std::condition_variable action_cv;
	};

} // end namespace dls

#include "dls2/action/action_client.tpp"

#endif /* end of include guard: ACTION_CLIENT_HPP */