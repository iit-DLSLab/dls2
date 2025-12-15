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
#include <iomanip>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

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

	struct ThermalZone {
        std::string type;      // human-readable label (e.g. x86_pkg_temp)
        std::string temp_path;  // path to /sys/.../temp
    };

/**
 * @brief Simple class to monitor CPU and memory usage of a system exploiting the Linux /proc filesystem.
 * More details at https://docs.kernel.org/filesystems/proc.html
 * 
 * Temperature-related useful command: 
 * export THERMAL_ZONE_REGEX="thermal_zone*" && paste <(cat /sys/class/thermal/$THERMAL_ZONE_REGEX/type) <(cat /sys/class/thermal/$THERMAL_ZONE_REGEX/temp) | column -s $'\t' -t | sed 's/\(.\)..$/.\1°C/'
 * 
 * Available temperatures example:
 * 		- INT3400 Thermal: hardware sensor managed by Intel DPTF
 * 		- pch_cometlake: general chipset components
 * 		- TMEM: general temperature-monitored point
 * 		- TSKN: thermal-critical zone (abstraction, usually CPU/GPU)
 * 		- NGFF: M2 slot 
 * 		- B0D4: general temperature-monitored point
 * 		- iwlwifi_1: Intel Wireless (iwlwifi) network adapter
 * 		- x86_pkg_temp: CPU Package Temperature
 */
class SystemResourceMonitor
{
	public:

		void readProcStat();
		void computeCpusUsage();
		void computeTemperature(const std::string& desired_type = "x86_pkg_temp");

		[[nodiscard]] const std::vector<double>& getCpusUsage();
		[[nodiscard]] double getTemperature();

	private:

		std::deque<std::vector<CpuTimes>> cpus_times_{};
		std::vector<CpuTimes> cpus_times_tmp_{};

		std::vector<double> latest_cpus_usage_;

		std::vector<ThermalZone> thermal_zones_{};
		bool thermal_zones_initialized_{ false };
		double latest_temperature_{ 0.0 };

};
} // namespace dls
