#include "dls2/application/periodic_app.hpp"
#include "dls2/util/time/time.hpp"
#include <fstream>
#include <filesystem>


using namespace dls;

PeriodicApp::PeriodicApp(const std::string &ID) 
	: App(ID)
	, config_scheduler(YAML::LoadFile("/usr/include/dls2/schedulers/" + ID + "/scheduler.yaml"))
	, period(std::chrono::milliseconds(config_scheduler["period"].as<int>()))
	, pause_mutex()
	, is_paused(false)
	, pause_request()
	, time_factor()
{
    this->pid = syscall(SYS_gettid);
	this->cur_time_factor = this->time_factor.getRealTimeFactor();

	memset(&scheduler_attributes, 0, sizeof(struct sched_attr));
	scheduler_attributes.size = sizeof(struct sched_attr);
	scheduler_attributes.sched_policy = SCHED_DEADLINE;

	// Period defined in nanoseconds
	sched_runtime_factor = config_scheduler["runtime_factor"].as<double>();
	sched_deadline_factor = config_scheduler["deadline_factor"].as<double>();

	scheduler_attributes.sched_period  = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*cur_time_factor).count();
	scheduler_attributes.sched_runtime = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*sched_runtime_factor*cur_time_factor).count();
	scheduler_attributes.sched_deadline = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*sched_deadline_factor*cur_time_factor).count();

	this->command_manager.addCommand<>
	(
		"pause",
		"Pause the execution of " + this->getID(),
		std::function<bool()>([&]()->bool
        {
			std::lock_guard<std::mutex> lock(this->pause_mutex);
			this->is_paused = true;
			this->pause_request.notify_all();
			scout_sys << this->getID() << " execution paused" << std::endl;
            return true;
		}),
		{{0,1}},
		true
	);

	this->command_manager.addCommand<>
	(
		"continue",
		"Continue the execution of " + this->getID(),
		std::function<bool()>([&]()->bool
		{
			std::lock_guard<std::mutex> lock(this->pause_mutex);
			this->is_paused = false;
			this->pause_request.notify_all();
			scout_sys << this->getID() << " execution continued" << std::endl;
            return true;
		}),
		{{1,0}},
		true
	);
}

AppStatus PeriodicApp::run()
{
	setStatus(AppStatus::RUNNING);

    int ret;
    unsigned int flags = 0;

    ret = sched_setattr(0, &scheduler_attributes, flags);
    if (ret < 0) {
        perror("sched_setattr");
        exit(-1);
    }

	#if DEBUG_SCHEDULER
	// Defini static variables to be used to debug the run time
	std::vector<double> run_time_vector;
	#endif

	do
	{
		// Calculate when the next period needs to start
		auto next_loop_time = this->period + std::chrono::system_clock::now();

		// Run one epoch
		#if DEBUG_SCHEDULER
		// Get current start time
		const std::chrono::system_clock::time_point start = std::chrono::high_resolution_clock::now();
		#endif
		run(std::chrono::time_point_cast<std::chrono::system_clock::duration, std::chrono::system_clock, std::chrono::duration<double>>(std::chrono::system_clock::now()));
		#if DEBUG_SCHEDULER
		// Get current stop time
		const std::chrono::system_clock::time_point stop = std::chrono::high_resolution_clock::now();
		// Get enlapsed run time
		double run_time = (std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count())/1000000000.0;
		// Collect run time
		run_time_vector.push_back(run_time);
		#endif

		// Check realtime
		if(std::chrono::system_clock::now() > next_loop_time)
		{
		 	setStatus(AppStatus::BREAKING_REALTIME);
		}

		// Pause execution if a pause request was made
		{
			std::unique_lock<std::mutex> lock(this->pause_mutex);
			if(this->is_paused)
			{
				this->pause_request.wait
				(
					lock,
					[&]{ return !this->is_paused; }
				);
			}
		}

		if(abs(this->time_factor.getRealTimeFactor() - this->cur_time_factor) > 0.02)
		{
			this->cur_time_factor = this->time_factor.getRealTimeFactor();

			memset(&scheduler_attributes, 0, sizeof(struct sched_attr));
			scheduler_attributes.size = sizeof(struct sched_attr);
			scheduler_attributes.sched_policy = SCHED_DEADLINE;

			double sched_runtime_factor = config_scheduler["runtime_factor"].as<double>();
			double sched_deadline_factor = config_scheduler["deadline_factor"].as<double>();

			scheduler_attributes.sched_period  = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*cur_time_factor).count();
			scheduler_attributes.sched_runtime = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*sched_runtime_factor*cur_time_factor).count();
			scheduler_attributes.sched_deadline = (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(period*sched_deadline_factor*cur_time_factor).count();

			unsigned int flags = 0;
			auto ret = sched_setattr(pid, &scheduler_attributes, flags);
			if (ret < 0) {
				perror("sched_setattr");
				// exit(-1);
			}
		}


		sched_yield();

	}while(!this->should_quit);

	#if DEBUG_SCHEDULER
	// Define debug folder
	const std::string debug_folder (std::string(std::getenv("HOME"))+"/dls2_debug_scheduler");
	// Create debug folder if it does not exist
	if(!std::filesystem::exists(debug_folder))
	{
		std::filesystem::create_directory(debug_folder);
	}
	// Write statistics in a file
	std::ofstream out(debug_folder+"/"+this->ID+".txt");
	std::vector<double> run_time_vector_unsorted = run_time_vector;
	// Largest run time values
	const int num_max_run_time(10);
	std::sort(run_time_vector.begin(), run_time_vector.end(), std::greater<double>());
	run_time_vector.erase( unique(run_time_vector.begin(), run_time_vector.end() ), run_time_vector.end());
	out << "LARGEST " << num_max_run_time << " RUN TIME:\n";
	for(int i=0; i<num_max_run_time; i++)
	{
		out << std::to_string(run_time_vector[i]) + "\n";
	}
	// Print info about if the run time has taken more or less than the period
	double period_seconds(period.count()/1000000.0);
	out << "\nperiod = "+std::to_string(period_seconds) +"\n"
			+ "runtime_factor = "+std::to_string(sched_runtime_factor) +"\n"
			+ "deadline_factor = "+std::to_string(sched_deadline_factor) +"\n";
	if(run_time_vector[0]>(period_seconds))
	{
		out << "\nThe run function has taken more than the desired period, that's bad! :(";
	}
	else
	{
		out << "\nThe run function has always taken less that the desired period, that's good! :)";
	}
	out << "\n----------------------------------------------------------------------------------------------------\nALL RUN TIME:\n";
	// All run time values
	for(double v : run_time_vector_unsorted)
	{
		out << std::to_string(v) + "\n";
	}
	
	#endif

	return this->getStatus();
}

AppStatus PeriodicApp::stop()
{
	// inform the component that it should exit at the end of the epoch
	this->should_quit = true;

	// unpause the component if it has been paused
	{
		std::lock_guard<std::mutex> lock(this->pause_mutex);
		this->is_paused = false;
		this->pause_request.notify_all();
	}
	
	this->setStatus(AppStatus::STOPPED);
	return this->getStatus();
}
