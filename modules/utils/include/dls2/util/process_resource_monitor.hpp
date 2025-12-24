#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>

#include "dls2/util/numerical_moving_window.hpp"
#include "dls2/util/time/duration_utils.hpp"

namespace dls
{

/**
 * @brief Simple class to monitor CPU and memory usage of a process exploiting the Linux /proc filesystem.
 * More details at https://docs.kernel.org/filesystems/proc.html
 */
class ProcessResourceMonitor
{
  public:
	explicit ProcessResourceMonitor(pid_t pid, size_t process_monitor_window_size);

	size_t update();
	double getCpuPercent() const;
	double getMemPercent() const;
	static long getTotalRAMkB();
	
  private:
	bool updateAllocatedMemory();
	bool updateCpuTicks(unsigned long long &total_ticks);

	long int sys_page_size_;
	long int sys_ticks_per_sec_;
	unsigned long long last_total_ticks_{0};
	std::chrono::steady_clock::time_point last_time_{};
	
	mutable std::mutex resource_mutex_;
	std::unique_ptr<NumericalMovingWindow<double>> cpu_percentage_w_;
	std::unique_ptr<NumericalMovingWindow<double>> memory_percentage_w_;

	std::string stat_path_{};
	std::string statm_path_{};

	long total_ram_kb_{0};
};
} // namespace dls
