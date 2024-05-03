#ifndef ACTION_SERVER_BASE_TPP
#define ACTION_SERVER_BASE_TPP

#include "dls2/action/action_server_base.hpp"

namespace dls
{
	// =========================================================================
	// Action base Implementation
	// =========================================================================
	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::ActionServerBase(const std::string &ID,
																	 const dls::topicType &topic_goal,
																	 const dls::topicType &topic_feedback,
																	 const dls::topicType &topic_result,
																	 const GOAL_t &goal,
																	 const FEEDBACK_t &feedback,
																	 const RESULT_t &result)
		: 	PeriodicAppPlugin(ID),
			goal(goal),
			feedback(feedback),
			result(result)
	{
		this->buildInput<GOAL_t>(topic_goal, &this->goal, std::bind(&ActionServerBase::startAction, this), false);
		this->buildOutput<FEEDBACK_t>("feedback", topic_feedback, &this->feedback);
		this->buildOutput<RESULT_t>("result", topic_result, &this->result);
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::~ActionServerBase(){}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	void ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::run(const std::chrono::system_clock::time_point &time)
	{
		static_cast<void>(time); // remove warning during compilation
		read();
		runAction();
		write("feedback");
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	std::string ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::where(){
		return "I don't know what to say: override me in the derived classes of ActionServerBase";
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	void ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::startAction()
	{		
		sm.raiseEvent(sm.activation_request);
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	void ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::stopAction()
	{
		sm.raiseEvent(sm.deactivation_request);
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	void ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::writeResult()
	{
		write("result");
	}
} // end namespace dls

#endif /* end of include guard: ACTION_SERVER_BASE_TPP */