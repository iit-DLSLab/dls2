#pragma once

#include <dls2/application/sched_attr.hpp>
#include <string>
#include <yaml-cpp/yaml.h>
#include <chrono>

class SchedulerUtils
{
	public:

		typedef std::chrono::duration<double, std::micro> period_t;

		SchedulerUtils();
		~SchedulerUtils() = default;

		void init(const YAML::Node& config_scheduler);
		
		void setRTSchedulerPolicy();

		void setDefaultSchedulerPolicy();

		void executeEndLoopTask();

		void setCurrentTimeFactor(double time_factor);

		void initDesiredTime(std::chrono::steady_clock::time_point desired_time);

		void setPrioritySCHED_FIFO(int priority);

		double getCurrentTimeFactor();

		SchedulerUtils::period_t getPeriod();

		std::string getPolicy();

	private:
		void setSCHED_FIFO();

		void setSCHED_DEADLINE();

		//! Attrributes of the scheduler
		struct sched_attr scheduler_attributes;
		std::string sched_rt_policy;
		
		//! The period of this component
		period_t period;
		//! The current time factor of this component, used to scale the period, runtime and deadline of the SCHED_DEADLINE policy
		double curr_time_factor;

		// SCHED_FIFO params
		int priority;
		std::chrono::steady_clock::time_point desired_time;

		// SCHED_DEADLINE params
		//! Runtime attribute
		period_t runtime;
		//! Deadline attribute
		period_t deadline;

};