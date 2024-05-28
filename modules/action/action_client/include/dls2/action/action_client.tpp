#ifndef ACTION_CLIENT_TPP
#define ACTION_CLIENT_TPP

#include "dls2/action/action_client.hpp"

namespace dls
{
	// =========================================================================
	// Action base Implementation
	// =========================================================================
	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	ActionClient<GOAL_t, FEEDBACK_t, RESULT_t>::ActionClient(
		const std::string &action_name,
		const std::shared_ptr<CommandManager> command_manager,
		const topicType &topic_goal,
		const topicType &topic_feedback,
		const topicType &topic_result,
		const GOAL_t &goal,
		const FEEDBACK_t &feedback,
		const RESULT_t &result)
		:
			ActionClientBase(action_name, command_manager),
			config(YAML::LoadFile("/usr/include/dls2/actions/" + action_name + "/config/config.yml")),
			goal_writer(this->dds_participant,
					  topic_goal,
					  std::make_shared<GOAL_t>(goal)),
		  	feedback_reader(this->dds_participant,
						  topic_feedback,
						  std::make_shared<FEEDBACK_t>(feedback)),
		  	result_reader(this->dds_participant,
						topic_result,
						std::make_shared<RESULT_t>(result),
						std::function<void()>([&]() -> void
											{ 	
												std::lock_guard<std::mutex> lock(this->action_mutex);
												action_cv.notify_one();
												this->is_action_completed = true; }))
	{
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	void ActionClient<GOAL_t, FEEDBACK_t, RESULT_t>::sendGoal(const GOAL_t &goal)
	{
		this->goal_writer.setSignal(goal);
		this->goal_writer.publish();
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	RESULT_t ActionClient<GOAL_t, FEEDBACK_t, RESULT_t>::waitForResult()
	{
		// std::cout << "Waiting for the result...\n";
		std::unique_lock<std::mutex> lock(this->action_mutex);
		this->action_cv.wait(lock, [&]() -> bool
							 { return this->is_action_completed; });
		// std::cout << "Result is received\n";
		return this->result_reader.getData();
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	void ActionClient<GOAL_t, FEEDBACK_t, RESULT_t>::callAction(const GOAL_t& goal)
	{
		{
			std::lock_guard<std::mutex> lock(this->action_mutex);
			this->is_action_completed = false;
		}
		this->sendGoal(goal);
	}
} // end namespace dls

#endif /* end of include guard: ACTION_CLIENT_TPP */