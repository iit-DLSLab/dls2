#include "dls2/application/resource_monitor.hpp"

using namespace dls;

ResourceMonitor::ResourceMonitor(pid_t pid) : ticks_per_sec_(sysconf(_SC_CLK_TCK))
{
	stat_path_ = "/proc/" + std::to_string(pid) + "/stat";
	statm_path_ = "/proc/" + std::to_string(pid) + "/statm";

	std::cout << "statm_path_: " << statm_path_ << "\n";
};

bool ResourceMonitor::update()
{
	std::lock_guard<std::mutex> lock(this->resource_mutex_);
	if (!updateAllocatedMemory())
	{
		return false;
	}

	unsigned long long total_ticks = 0;
	if (!updateCpuTicks(total_ticks))
	{
		return false;
	}

	auto now = std::chrono::steady_clock::now();

	if (last_total_ticks_ == 0)
	{
		// First sample – just store baseline, no % yet.
		last_total_ticks_ = total_ticks;
		last_time_ = now;
		return false;
	}

	auto delta_time =
	    std::chrono::duration_cast<std::chrono::duration<double>>(now - last_time_).count();
	if (delta_time <= 0.0 || total_ticks < last_total_ticks_)
	{
		// Time went backwards or counters reset
		last_total_ticks_ = total_ticks;
		last_time_ = now;
		return false;
	}

	unsigned long long d_ticks = total_ticks - last_total_ticks_;
	double d_seconds = static_cast<double>(d_ticks) / static_cast<double>(ticks_per_sec_);

	cpu_percent_ = 100.0 * (d_seconds / delta_time);

	last_total_ticks_ = total_ticks;
	last_time_ = now;

	return true;
}

const double& ResourceMonitor::getCpuPercent() const
{
	std::lock_guard<std::mutex> lock(this->resource_mutex_);
	return cpu_percent_;
}

const double& ResourceMonitor::getMemUsage() const
{
	std::lock_guard<std::mutex> lock(this->resource_mutex_);
	return memory_usage_;
}

bool ResourceMonitor::updateAllocatedMemory()
{
	int tSize = 0;
	int resident = 0;
	int share = 0;
	std::ifstream buffer(statm_path_);
	if (!(buffer >> tSize >> resident >> share))
	{
		return false;
	}
	buffer.close();

	long page_size_kb = ticks_per_sec_ / 1024; // in case x86-64 is configured to use 2MB pages
	memory_usage_ = static_cast<double>(resident) * static_cast<double>(page_size_kb);
	return true;
}

bool ResourceMonitor::updateCpuTicks(unsigned long long &total_ticks)
{
	std::ifstream stat(stat_path_);
	if (!stat.is_open())
	{
		return false;
	}

	std::string line = "";
	if (!std::getline(stat, line))
	{
		return false;
	}

	stat.close();

	// Find end of comm field: it is the last ')' in the line
	std::size_t rparen = line.rfind(')');
	if (rparen == std::string::npos)
	{
		return false;
	}

	// Everything after ") " is fields 3..N (state, ppid, ...)
	if (rparen + 2 >= line.size())
	{
		return false;
	}
	std::string rest = line.substr(rparen + 2);

	std::istringstream iss(rest);

	// Skip fields 3..13 (11 fields total)
	std::string token = "";
	for (int field = 3; field <= 13; ++field)
	{
		if (!(iss >> token))
		{
			return false;
		}
	}

	long utime = 0;
	long stime = 0;
	if (!(iss >> utime >> stime))
	{ // 14: utime, 15: stime
		return false;
	}

	total_ticks = utime + stime;
	return true;
}