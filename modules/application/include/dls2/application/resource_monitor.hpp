#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>

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

	bool update();
	const double &getCpuPercent() const;
	const double &getMemUsage() const;

  private:
	bool updateAllocatedMemory();
	bool updateCpuTicks(unsigned long long &total_ticks);

	long int ticks_per_sec_;
	unsigned long long last_total_ticks_{0};
	std::chrono::steady_clock::time_point last_time_{};
	
	mutable std::mutex resource_mutex_;
	double cpu_percent_{0.0};
	double memory_usage_{0.0};

	std::string stat_path_{};
	std::string statm_path_{};
};
} // namespace dls
