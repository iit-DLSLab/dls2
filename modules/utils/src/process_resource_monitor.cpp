#include "dls2/util/process_resource_monitor.hpp"

using namespace dls;

ProcessResourceMonitor::ProcessResourceMonitor(pid_t pid, size_t process_monitor_window_size) : 
	sys_page_size_(sysconf(_SC_PAGE_SIZE))
	, sys_ticks_per_sec_(sysconf(_SC_CLK_TCK))
{
	stat_path_ = "/proc/" + std::to_string(pid) + "/stat";
	statm_path_ = "/proc/" + std::to_string(pid) + "/statm";

	total_ram_kb_ = getTotalRAMkB();
	if(total_ram_kb_ <= 0){
		std::cerr << "Total amount of ram not found\n";
	}

	cpu_percentage_w_ = std::make_unique<NumericalMovingWindow<double>>(process_monitor_window_size);
	memory_percentage_w_ = std::make_unique<NumericalMovingWindow<double>>(process_monitor_window_size);
};

size_t ProcessResourceMonitor::update()
{
	std::lock_guard<std::mutex> lock(this->resource_mutex_);
	if (!updateAllocatedMemory())
	{
		return 1;
	}

	unsigned long long total_ticks = 0;
	if (!updateCpuTicks(total_ticks))
	{
		return 2;
	}

	auto now = std::chrono::steady_clock::now();

	if (last_total_ticks_ == 0)
	{
		// First sample – just store baseline, no % yet.
		last_total_ticks_ = total_ticks;
		last_time_ = now;
		return 0;
	}

	auto delta_time = toSec<double>(now - last_time_);
	if (delta_time <= 0.0 || total_ticks < last_total_ticks_)
	{
		// Time went backwards or counters reset
		last_total_ticks_ = total_ticks;
		last_time_ = now;
		return 3;
	}

	unsigned long long d_ticks = total_ticks - last_total_ticks_;
	double d_seconds = static_cast<double>(d_ticks) / static_cast<double>(sys_ticks_per_sec_);

	// cpu_percentage is in [0, N_cores * 100]
	auto cpu_percentage = 100.0 * (d_seconds / delta_time); // 100 * (CPU_seconds_used / wall_seconds_elapsed)

	cpu_percentage_w_->push(cpu_percentage);

	last_total_ticks_ = total_ticks;
	last_time_ = now;

	return 0;
}

double ProcessResourceMonitor::getCpuPercent() const
{
	std::lock_guard<std::mutex> lock(this->resource_mutex_);
	return cpu_percentage_w_->mean();
}

double ProcessResourceMonitor::getMemPercent() const
{
	std::lock_guard<std::mutex> lock(this->resource_mutex_);
	return memory_percentage_w_->mean();
}

bool ProcessResourceMonitor::updateAllocatedMemory()
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

	long page_size_kb = sys_page_size_ / 1024; // in case x86-64 is configured to use 2MB pages
	auto memory_usage = static_cast<double>(resident) * static_cast<double>(page_size_kb);
	auto memory_percentage = 100.0 * (memory_usage / total_ram_kb_);
	memory_percentage_w_->push(memory_percentage);
	return true;
}

bool ProcessResourceMonitor::updateCpuTicks(unsigned long long &total_ticks)
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

long ProcessResourceMonitor::getTotalRAMkB() {
    std::ifstream meminfo("/proc/meminfo");
    std::string key;
    long value;
    std::string unit;

    while (meminfo >> key >> value >> unit) {
        if (key == "MemTotal:") {
            return value; // kB
        }
    }
    return -1;
}