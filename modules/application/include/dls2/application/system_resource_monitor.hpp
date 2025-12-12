#pragma once

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <deque>

namespace dls
{

	struct CpuTimes {
		unsigned long long user = 0;
		unsigned long long nice = 0;
		unsigned long long system = 0;
		unsigned long long idle = 0;
		unsigned long long iowait = 0;
		unsigned long long irq = 0;
		unsigned long long softirq = 0;
		unsigned long long steal = 0;
		unsigned long long guest = 0;
		unsigned long long guest_nice = 0;

		unsigned long long total() const {
			return user + nice + system + idle + iowait +
				irq + softirq + steal;
		}

		unsigned long long idleAll() const {
			return idle + iowait;
		}
	};

/**
 * @brief Simple class to monitor CPU and memory usage of a system exploiting the Linux /proc filesystem.
 * More details at https://docs.kernel.org/filesystems/proc.html
 */
class SystemResourceMonitor
{
	public:

		void readProcStat();
		void computeCpusUsage();

		[[nodiscard]] const std::vector<double>& getCpusUsage();

	private:

		std::deque<std::vector<CpuTimes>> cpus_times_{};
		std::vector<CpuTimes> cpus_times_tmp_{};

		std::vector<double> latest_cpus_usage_;
};
} // namespace dls
