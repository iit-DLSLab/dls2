#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include "dls2/util/time/duration_utils.hpp"

namespace dls
{

/**
 * @brief Simple class to monitor CPU and memory usage of a process exploiting the Linux /proc filesystem.
 * More details at https://docs.kernel.org/filesystems/proc.html
 */
class ResourceMonitor
{
  public:
	explicit ResourceMonitor(pid_t pid);

	size_t update();
	const double &getCpuPercent() const;
	const double &getMemPercent() const;
	static long getTotalRAMkB();
	
  private:
	bool updateAllocatedMemory();
	bool updateCpuTicks(unsigned long long &total_ticks);

	long int sys_page_size_;
	long int sys_ticks_per_sec_;
	unsigned long long last_total_ticks_{0};
	std::chrono::steady_clock::time_point last_time_{};
	
	mutable std::mutex resource_mutex_;
	double cpu_percent_{0.0};
	double memory_percent_{0.0};

	std::string stat_path_{};
	std::string statm_path_{};

	long total_ram_kb_{0};
};
} // namespace dls
