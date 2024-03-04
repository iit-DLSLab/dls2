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
		: 	PeriodicPluginBase(ID),
			request_activation(false),
			initialized(true),
			goal(goal),
			feedback(feedback),
			result(result)
	{
		this->buildInput<GOAL_t>(topic_goal, &this->goal, std::bind(&ActionServerBase::startAction, this));
		this->buildOutput<FEEDBACK_t>("feedback", topic_feedback, &this->feedback);
		this->buildOutput<RESULT_t>("result", topic_result, &this->result);

		handle_activation_request_thread = std::thread(&ActionServerBase::handleActivationRequest, this);
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::~ActionServerBase(){
		request_activation_cv.notify_one();
		handle_activation_request_thread.join();
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	void ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::run(const std::chrono::system_clock::time_point &time)
	{
		static_cast<void>(time); // remove warning during compilation
		if (active && initialized)
		{
			read();
			runAction();
			if(isActionStopped())
			{
				write("feedback");
				write("result");
			}
			else
				write("feedback");
		}
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	std::string ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::where(){
		return "I don't know what to say: override me in the derived classes of ActionServerBase";
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	void ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::startAction()
	{		
		requestActivation();
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	void ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::stopAction()
	{
		deactivate();
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	bool ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::isActionStopped()
	{
		return !this->active;
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	void ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::handleActivationRequest()
	{
		while(true)
		{
			std::unique_lock<std::mutex> lock(request_activation_mutex);
			request_activation_cv.wait(lock);
			if(should_quit){
				break;
			}
			if(areInputsReceivingData(true) && areOutputsUnique()){
				active = true;
			}
			else
			{
				scout_err << "Action " << this->getID() << " cannot be activated. Unloading..."<< std::endl;
				command_manager.callCommand("unloadAction", {this->getID()}, "ServiceLayer");
			}
			request_activation = false;
		}
	}

	template <typename GOAL_t, typename FEEDBACK_t, typename RESULT_t>
	void ActionServerBase<GOAL_t, FEEDBACK_t, RESULT_t>::requestActivation(){
		if(active){
			scout_warn << "Action " << ID_ << " is already active"<< std::endl;
		}
		else if (request_activation){
			scout_warn << "Activation has been already requested for the action " << ID_ << std::endl;
		}
		else{
			request_activation = true;
			request_activation_cv.notify_one();
		}
	}
} // end namespace dls

#endif /* end of include guard: ACTION_SERVER_BASE_TPP */