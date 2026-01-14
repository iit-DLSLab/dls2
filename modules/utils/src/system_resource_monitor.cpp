#include "dls2/util/system_resource_monitor.hpp"

using namespace dls;

SystemResourceMonitor::SystemResourceMonitor(size_t resource_monitor_window_size){
    resource_monitor_window_size_ = resource_monitor_window_size;
	mem_usage_w_ = std::make_unique<NumericalMovingWindow<double>>(resource_monitor_window_size_);
	temperature_w_ = std::make_unique<NumericalMovingWindow<double>>(resource_monitor_window_size_);
}

void SystemResourceMonitor::monitor(){
    readProcStat();
    computeCpusUsage();
    computeMemUsage();
    computeTemperature();
}

const std::vector<double>& SystemResourceMonitor::getCpusUsage(){
	return latest_cpus_usage_;
}

double SystemResourceMonitor::getMemUsage(){
	return latest_mem_usage_;
}

std::pair<std::string, double> SystemResourceMonitor::getTemperature(){
	return latest_temperature_;
}

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

    const auto core_num = prev_cpus_times.size();

    // Initialize a numerical window for each core if not done
    if(cpu_usage_w_.size() != core_num){
        cpu_usage_w_.resize(core_num);
        for(auto& window : cpu_usage_w_){
            window = std::make_unique<NumericalMovingWindow<double>>(resource_monitor_window_size_);
        }
    }

    
	std::vector<double> result;
    result.reserve(core_num);

    for (size_t i = 0; i < core_num; i++) {
        unsigned long long idleDiff  = new_cpus_times[i].idleAll() - prev_cpus_times[i].idleAll();	
        unsigned long long totalDiff = new_cpus_times[i].total() - prev_cpus_times[i].total();

        double usage = 0.0;
        if (totalDiff > 0){
            usage = 100.0 * (totalDiff - idleDiff) / totalDiff;
        }

        cpu_usage_w_.at(i)->push(usage);

        result.push_back(cpu_usage_w_.at(i)->mean());
    }

	latest_cpus_usage_ = result;
}

void SystemResourceMonitor::computeMemUsage()
{    
    long long totalKB = 0;
    long long availableKB = 0;

    FILE* f = std::fopen("/proc/meminfo", "r");
    if (!f){
        return;
    }

    char line[256];

    while (std::fgets(line, sizeof(line), f)) {
        if (std::strncmp(line, "MemTotal:", 9) == 0) {
            std::sscanf(line + 9, "%lld", &totalKB);
        } else if (std::strncmp(line, "MemAvailable:", 13) == 0) {
            std::sscanf(line + 13, "%lld", &availableKB);
        }

        if (totalKB && availableKB){
            break;
        }
    }

    std::fclose(f);

    if (totalKB <= 0){
        mem_usage_w_->push(0.0);
        latest_mem_usage_ = mem_usage_w_->mean();
        return;
    } 
    if (availableKB <= 0){
        mem_usage_w_->push(100.0);
        latest_mem_usage_ = mem_usage_w_->mean();
        return;
    } 
    
    auto usage = 100.0 * (1.0 - static_cast<double>(availableKB) / static_cast<double>(totalKB));
    mem_usage_w_->push(usage);
    latest_mem_usage_ = mem_usage_w_->mean();
}

void SystemResourceMonitor::computeTemperature(const std::string& desired_type)
{
    // One-time initialization: discover zones and read their "type"
    if (!thermal_zones_type2path_.has_value()) {
        
        // Constructing an empty std::map
        thermal_zones_type2path_.emplace(); 
        
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

            thermal_zones_type2path_->emplace(buf, std::move(temp_path));
        }
        ::closedir(dir);

        if(thermal_zones_type2path_->empty()){
            // No zones found
            return;
        }
    }

    if(!thermal_zones_type2path_.has_value()){
        return;
    }
    auto zone_it = thermal_zones_type2path_->find(desired_type);
    if(zone_it == thermal_zones_type2path_->end()){
        // Desired zone type not found
        return;
    }

    const auto& zone_path = zone_it->second;
    int fd = ::open(zone_path.c_str(), O_RDONLY);
    if (fd < 0) {
        // If temp can't be read this time, skip it
        return;
    }

    char buf[32];
    ssize_t n = ::read(fd, buf, sizeof(buf) - 1);
    ::close(fd);
    if (n <= 0){
        return;
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

    temperature_w_->push(temp_celsius);

    latest_temperature_.first = desired_type;
    latest_temperature_.second = temperature_w_->mean();
}