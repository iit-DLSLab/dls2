#include <dls2/application/sched_utils.hpp>

#include <cstring>
#include <cstdio>
#include <sched.h>
#include <stdlib.h>
#include <iostream>
#include <thread>

SchedulerUtils::SchedulerUtils() : sched_rt_policy("SCHED_DEADLINE"), curr_time_factor(1.0)
{
	memset(&scheduler_attributes, 0, sizeof(struct sched_attr));
	scheduler_attributes.size = sizeof(struct sched_attr);
}

void SchedulerUtils::init(const YAML::Node& config_scheduler)
{
	std::cout << "\nInitializing SchedulerUtils with config: " << config_scheduler << "\n" << std::endl;
	if( config_scheduler["sched_rt_policy"])
	{
		sched_rt_policy = config_scheduler["sched_rt_policy"].as<std::string>();
	}
	
	// set scheduler attributes based on the configuration
	period = std::chrono::milliseconds(config_scheduler["period"].as<int>());
	
	if(sched_rt_policy == "SCHED_DEADLINE")
	{
		double sched_runtime_factor = config_scheduler["runtime_factor"].as<double>();
		double sched_deadline_factor = config_scheduler["deadline_factor"].as<double>();
		runtime = period * sched_runtime_factor;
		deadline = period * sched_deadline_factor;
	}
	else if (sched_rt_policy == "SCHED_FIFO")
	{
		if( config_scheduler["sched_priority"])
		{
			//save fifo priority param
			priority = config_scheduler["sched_priority"].as<int>();
		}
		else
		{
			priority = 45;
		}
	}
}

void SchedulerUtils::setRTSchedulerPolicy()
{
	if(sched_rt_policy == "SCHED_FIFO")
	{
		setSCHED_FIFO();
	}
	else if(sched_rt_policy == "SCHED_DEADLINE")
	{
		setSCHED_DEADLINE();
	}
}

void SchedulerUtils::setSCHED_FIFO()
{
	scheduler_attributes.sched_policy = SCHED_FIFO;
	scheduler_attributes.sched_priority = this->priority;

	if (sched_setattr(0, &scheduler_attributes, 0) < 0) {
		perror("sched_setattr");
		exit(-1);
	}
}

void SchedulerUtils::setSCHED_DEADLINE()
{
	scheduler_attributes.sched_policy = SCHED_DEADLINE;
	scheduler_attributes.sched_period = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*curr_time_factor).count();
	scheduler_attributes.sched_runtime = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(runtime*curr_time_factor).count();
	scheduler_attributes.sched_deadline = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(deadline*curr_time_factor).count();

	if (sched_setattr(0, &scheduler_attributes, 0) < 0) {
		perror("sched_setattr");
		exit(-1);
	}
}

void SchedulerUtils::setDefaultSchedulerPolicy()
{
	scheduler_attributes.sched_policy = SCHED_OTHER;

	if (sched_setattr(0, &scheduler_attributes, 0) < 0) {
            perror("sched_setattr - could not switch to SCHED_OTHER with sched_setattr");
		exit(-1);
	}
}

void SchedulerUtils::executeEndLoopTask()
{
	if (sched_rt_policy == "SCHED_DEADLINE")
		sched_yield();
	else if (sched_rt_policy == "SCHED_FIFO")
	{
		desired_time += std::chrono::duration_cast<std::chrono::steady_clock::duration>(period * curr_time_factor);
		std::this_thread::sleep_until(this->desired_time);
	}
}

void SchedulerUtils::setCurrentTimeFactor(double time_factor)
{
	this->curr_time_factor = time_factor;
}

void SchedulerUtils::setPrioritySCHED_FIFO(int priority)
{
	this->priority = priority;
}

void SchedulerUtils::initDesiredTime(std::chrono::steady_clock::time_point desired_time)
{
	this->desired_time = desired_time;
}

double SchedulerUtils::getCurrentTimeFactor()
{
	return this->curr_time_factor;
}

SchedulerUtils::period_t SchedulerUtils::getPeriod()
{
	return this->period;
}

std::string SchedulerUtils::getPolicy()
{
	return this->sched_rt_policy;
}