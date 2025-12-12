#include "dls2/application/system_resource_monitor.hpp"

using namespace dls;

void SystemResourceMonitor::readProcStat()
{
    cpus_times_tmp_.clear();

    FILE* f = std::fopen("/proc/stat", "r");
    if (!f){
        return;
	}

    char buf[256];

    while (std::fgets(buf, sizeof(buf), f)) {
        // Lines starting with "cpu"
        if (buf[0] != 'c' || buf[1] != 'p' || buf[2] != 'u')
            break;

        // After "cpu", expecting either digit(s) or space (for the total), stop otherwise
        if (!std::isdigit(static_cast<unsigned char>(buf[3])) &&
            !std::isspace(static_cast<unsigned char>(buf[3]))) {
            break;
        }

        CpuTimes t{};
        char label[16];

        int n = std::sscanf(
            buf,
            "%15s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
            label,
            &t.user, &t.nice, &t.system, &t.idle, &t.iowait,
            &t.irq, &t.softirq, &t.steal, &t.guest, &t.guest_nice
        );

        if (n < 5) {
            // Not enough data to be useful (user, nice, system, idle, iowait)
            continue;
        }

        cpus_times_tmp_.push_back(t);
    }

    std::fclose(f);

	if(cpus_times_tmp_.empty()){
		return;
	}

	cpus_times_.push_back(cpus_times_tmp_);
	
	// Delete old samples, just latest 2 are needed
	while(cpus_times_.size() > 2){
		this->cpus_times_.pop_front();
	}
}

void SystemResourceMonitor::computeCpusUsage()
{
	if(cpus_times_.size() < 2){
		latest_cpus_usage_.clear();
		return;
	}

	const auto& prev_cpus_times = cpus_times_.front();
	const auto& new_cpus_times = cpus_times_.back();

	std::vector<double> result;
    result.reserve(prev_cpus_times.size());

    for (size_t i = 0; i < prev_cpus_times.size(); i++) {
        unsigned long long idleDiff  = new_cpus_times[i].idleAll() - prev_cpus_times[i].idleAll();	
        unsigned long long totalDiff = new_cpus_times[i].total() - prev_cpus_times[i].total();

        double usage = 0.0;
        if (totalDiff > 0){
            usage = 100.0 * (totalDiff - idleDiff) / totalDiff;
        }

        result.push_back(usage);
    }

	latest_cpus_usage_ = result;
}

const std::vector<double>& SystemResourceMonitor::getCpusUsage(){
	return latest_cpus_usage_;
}