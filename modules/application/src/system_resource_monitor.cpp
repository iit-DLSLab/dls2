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

void SystemResourceMonitor::computeTemperature(const std::string& desired_type)
{
    // One-time initialization: discover zones and read their "type"
    if (!thermal_zones_initialized_) {
        const char* base_dir = "/sys/class/thermal";
        DIR* dir = ::opendir(base_dir);
        if (!dir) {
            // If we can't open the dir, nothing to do.
            return;
        }

        struct dirent* de;
        while ((de = ::readdir(dir)) != nullptr) {
            // Looking for directories named "thermal_zone*"
            if (std::strncmp(de->d_name, "thermal_zone", 12) != 0)
                continue;

            std::string base_path = std::string(base_dir) + "/" + de->d_name;
            std::string type_path = base_path + "/type";
            std::string temp_path = base_path + "/temp";

            // Read type once; if it fails, skip this zone
            int fd = ::open(type_path.c_str(), O_RDONLY);
            if (fd < 0)
                continue;

            char buf[128];
            ssize_t n = ::read(fd, buf, sizeof(buf) - 1);
            ::close(fd);
            if (n <= 0)
                continue;

            buf[n] = '\0';
            // Strip trailing newline and whitespace
            while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r' ||
                             buf[n - 1] == ' '  || buf[n - 1] == '\t')) {
                buf[--n] = '\0';
            }

            ThermalZone z;
            z.type = buf;
            z.temp_path = temp_path;
            thermal_zones_.push_back(std::move(z));
        }
        ::closedir(dir);

        thermal_zones_initialized_ = true;
        if (thermal_zones_.empty()) {
            // No zones found, nothing more to do
            return;
        }
    }
	
    for (const auto& z : thermal_zones_) {
        if(z.type != desired_type){
            continue;
        }

        int fd = ::open(z.temp_path.c_str(), O_RDONLY);
        if (fd < 0) {
            // If temp can't be read this time, skip it
            continue;
        }

        char buf[32];
        ssize_t n = ::read(fd, buf, sizeof(buf) - 1);
        ::close(fd);
        if (n <= 0){
            continue;
        }

        buf[n] = '\0';
        // Strip trailing whitespace
        while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r' ||
                         buf[n - 1] == ' '  || buf[n - 1] == '\t')) {
            buf[--n] = '\0';
        }

        // temp is in millidegrees Celsius
        long temp_milli = std::strtol(buf, nullptr, 10);
        double temp_celsius = temp_milli / 1000.0;

        latest_temperature_ = temp_celsius;
    }
}

const std::vector<double>& SystemResourceMonitor::getCpusUsage(){
	return latest_cpus_usage_;
}

double SystemResourceMonitor::getTemperature(){
	return latest_temperature_;
}