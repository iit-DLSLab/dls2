#include "dls2/util/messaging/dds_participant.hpp"

#include <fastdds/dds/xtypes/dynamic_types/DynamicData.hpp>

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <numeric>
#include <mutex>
#include <thread>
#include <vector>

// seconds to wait for the discovery process to finish (aritrary number)
#define DISCOVERY_TIME 2

volatile sig_atomic_t stop;


// bool stop = false;
unsigned long int window_size = 1000;

namespace fdds = eprosima::fastdds::dds;

// One received message
struct Sample
{
	double time;        // from the first message, receiver clock
	double period;      // from the previous message, receiver clock
	double pub_period;  // from the previous message, publisher timestamps
	long long lost;     // sequence_ids skipped since the previous message
	uint32_t sequence_id;
	double latency;     // from the publisher timestamp to the reception, system clock
};

std::vector<double> times{};
std::vector<double> pub_times{};
std::chrono::high_resolution_clock::time_point first;
std::chrono::high_resolution_clock::time_point last;
bool received = false;  // false until the first message, which only starts the measurement
bool log_csv = false;
bool check_stamp = false;  // --stamp: also measure the publisher rate and the lost messages
bool stamp_missing = false;  // the message has no sequence_id and timestamp to check
uint32_t last_sequence_id = 0;
double last_stamp_ns = 0.0;
unsigned long long lost_total = 0;
std::vector<Sample> csv_samples{};  // collected since the last CSV write
void * msg;  // just to avoid warnings during compilation..... :)
std::mutex status_mutex;

// Only set the flag: printing from a signal handler can deadlock
void my_handler(int){
	stop = true;
}

// `unsigned long sequence_id` and timestamp (nanoseconds) of a DLS2 message: in its DlsHeader `header`
// (`unsigned long long timestamp`) or, without it, at the top level (`double timestamp`)
bool read_stamp(fdds::DynamicData::_ref_type data, uint32_t& sequence_id, double& stamp_ns)
{
	const fdds::MemberId header_id = data->get_member_id_by_name("header");
	if(header_id == fdds::MEMBER_ID_INVALID)
	{
		return data->get_uint32_value(sequence_id, data->get_member_id_by_name("sequence_id")) == fdds::RETCODE_OK &&
			data->get_float64_value(stamp_ns, data->get_member_id_by_name("timestamp")) == fdds::RETCODE_OK;
	}
	fdds::DynamicData::_ref_type header = data->loan_value(header_id);
	uint64_t stamp = 0;
	const bool ok = header &&
		header->get_uint32_value(sequence_id, header->get_member_id_by_name("sequence_id")) == fdds::RETCODE_OK &&
		header->get_uint64_value(stamp, header->get_member_id_by_name("timestamp")) == fdds::RETCODE_OK;
	stamp_ns = static_cast<double>(stamp);
	if(header)
	{
		data->return_loaned_value(header);
	}
	return ok;
}

// Mean and standard deviation of values
std::pair<double, double> mean_stdev(const std::vector<double>& values)
{
	const double size = values.size();
	double mean = 0.0;
	for(double value: values)
	{
		mean += value/size;
	}
	double diff = 0.0;
	for(double value: values)
	{
		diff += pow(value - mean, 2)/size;
	}
	return {mean, sqrt(diff)};
}


int main(int argc, char** argv){

	// Extract the options, the remaining arguments are positional
	std::string csv_path;
	double record_time = 0.0;  // seconds, 0 means until CTRL+C
	std::vector<std::string> args;
	for(int i = 0; i < argc; i++){
		if(std::string(argv[i]) == "--csv" && i + 1 < argc){
			csv_path = argv[++i];
		}else if(std::string(argv[i]) == "--time" && i + 1 < argc){
			record_time = strtod(argv[++i], NULL);
		}else if(std::string(argv[i]) == "--stamp"){
			check_stamp = true;
		}else{
			args.push_back(argv[i]);
		}
	}
	argc = args.size();

	if (argc<2)
	{
		std::cout << "Wrong number of arguments: Minimum 2 arguments required" << std::endl;
		std::cout << "Usage examples:" << std::endl;
		std::cout << "\tddstopic list <(optional) domain>" << std::endl;
		std::cout << "\tddstopic participants <(optional) domain>" << std::endl;
		std::cout << "\tddstopic hz <topic_name> <(optional) domain> [--csv <file>] [--time <seconds>] [--stamp]" << std::endl;
		std::cout << "\t\t--csv <file>: log time, period and rate of every received message" << std::endl;
		std::cout << "\t\t--time <seconds>: stop after this time instead of waiting for CTRL+C" << std::endl;
		std::cout << "\t\t--stamp: also measure the publisher rate from the message timestamp and the lost sequence_ids" << std::endl;
		return EXIT_FAILURE;
	}

	// CTRL-C handler, also on kill and closed terminal (e.g. tmux pane) so the CSV is completed
	signal(SIGINT, my_handler);
	signal(SIGTERM, my_handler);
	signal(SIGHUP, my_handler);

	const std::string command = args[1];

    int domain = 3;

	if(command == "list" || command == "participants"){
		if(argc==3){
			domain = strtol(args[2].c_str(), NULL, 10);
		}
	}else if(command == "hz"){
		if(argc==4){
			domain = strtol(args[3].c_str(), NULL, 10);
		}
		if(argc<3){
			std::cout << "Usage: ddstopic hz <topic_name> <(optional) domain> [--csv <file>] [--time <seconds>] [--stamp]" << std::endl;
			return EXIT_FAILURE;
		}
	}else{
		return EXIT_FAILURE;
	}

	std::ofstream csv;
	if(!csv_path.empty()){
		csv.open(csv_path);
		if(!csv){
			std::cout << "Cannot open " << csv_path << std::endl;
			return EXIT_FAILURE;
		}
		csv << "time_s,period_s,rate_hz" << (check_stamp ? ",pub_period_s,lost,sequence_id,latency_s" : "") << "\n" << std::setprecision(9);
		log_csv = true;
		std::cout << "Logging to " << csv_path << std::endl;
	}

    dls::DDSParticipant participant("ddstopic", domain, eprosima::fastdds::rtps::DiscoveryProtocol::SUPER_CLIENT);
	
	sleep(DISCOVERY_TIME);
	
	if(command == "list"){
		std::vector<std::string> topicList = participant.getDiscoveredTopics();
		for(auto& topic: topicList){
			std::cout << "\t" << topic << std::endl;
		}

	}
	else if(command == "participants"){
		auto participant_vect  = participant.getParticipants();
		for(auto &t: participant_vect){
			std::cout << "\t" << t << std::endl;
		}
	}
	else if(command == "hz"){
		const std::string topic_name = args[2];

		// An unknown topic would give an empty type support, which crashes addReader
		auto discovery_database = participant.get_discovery_database();
		if(discovery_database.find(topic_name) == discovery_database.end())
		{
			std::cout << "Topic " << topic_name << " not discovered. Discovered topics:" << std::endl;
			for(auto& topic: participant.getDiscoveredTopics()){
				std::cout << "\t" << topic << std::endl;
			}
			return EXIT_FAILURE;
		}

		participant.addReader(topic_name+"_reader",
							dls::topicType(topic_name, discovery_database[topic_name]),
							std::function<void(void *)>
							{
								[&](void *tuple)
								{
									std::lock_guard<std::mutex> lock(status_mutex);
									std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
									// Same clock as the publisher timestamps
									const double received_ns = std::chrono::duration<double, std::nano>(std::chrono::system_clock::now().time_since_epoch()).count();
									msg = tuple;
									uint32_t sequence_id = 0;
									double stamp_ns = 0.0;
									// Messages from a discovered topic are dynamic data
									if(check_stamp && !read_stamp(*static_cast<fdds::DynamicData::_ref_type*>(tuple), sequence_id, stamp_ns))
									{
										stamp_missing = true;
										stop = true;
										return;
									}
									// No period yet on the first message: measuring from the program start would count the discovery time
									if(!received)
									{
										received = true;
										first = last = now;
										last_sequence_id = sequence_id;
										last_stamp_ns = stamp_ns;
										return;
									}
									double diff = std::chrono::duration<double>(now-last).count();
									last = now;
									double pub_diff = 0.0;
									long long lost = 0;
									if(check_stamp)
									{
										pub_diff = (stamp_ns - last_stamp_ns)*1e-9;
										// Publishers wrap the sequence_id at MAX_SEQUENCE_ID
										const long long max_id = dls::MAX_SEQUENCE_ID;
										lost = ((static_cast<long long>(sequence_id) - last_sequence_id - 1) % max_id + max_id) % max_id;
										lost_total += lost;
										last_sequence_id = sequence_id;
										last_stamp_ns = stamp_ns;
										if(pub_times.size()==window_size)
										{
											pub_times.erase(pub_times.begin());
										}
										pub_times.push_back(pub_diff);
									}
									if(log_csv)
									{
										csv_samples.push_back({std::chrono::duration<double>(now-first).count(), diff, pub_diff, lost, sequence_id, (received_ns - stamp_ns)*1e-9});
									}
									if(times.size()==window_size)
									{
										times.erase(times.begin());
									}
									times.push_back(diff);
								}
							},
							eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT,
							false);

		// Write the samples collected by the reader since the last call
		auto write_csv = [&](const std::vector<Sample>& samples)
		{
			for(const auto& sample: samples)
			{
				csv << sample.time << "," << sample.period << "," << (sample.period > 0.0 ? 1.0/sample.period : 0.0);
				if(check_stamp)
				{
					csv << "," << sample.pub_period << "," << sample.lost << "," << sample.sequence_id << "," << sample.latency;
				}
				csv << "\n";
			}
			csv.flush();
		};

		//Loop that calculates the frequency, use CTRL + C (or --time) to stop
		const auto start = std::chrono::steady_clock::now();
		const auto end = start + std::chrono::duration<double>(record_time);
		while(!stop)
		{
			// Elapsed time, out of the total with --time
			std::ostringstream elapsed;
			elapsed << "[" << std::fixed << std::setprecision(0)
				<< std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
			if(record_time > 0.0){
				elapsed << "/" << record_time;
			}
			elapsed << " s] ";

			std::vector<double> current_times;
			std::vector<double> current_pub_times;
			std::vector<Sample> current_samples;
			unsigned long long current_lost = 0;
			{
				std::lock_guard<std::mutex> lock(status_mutex);
				current_times = times;
				current_pub_times = pub_times;
				current_lost = lost_total;
				current_samples.swap(csv_samples);
			}
			if(log_csv)
			{
				write_csv(current_samples);
			}
			if(!current_times.empty())
			{
				// Compute mean, standard deviation, max and min values
				const auto [mean, stdev] = mean_stdev(current_times);
				const auto [min, max] = std::minmax_element(current_times.begin(), current_times.end());
				std::cout << elapsed.str() << "rate: " << 1.0/mean << " hz, stdev: " << stdev << ", max: " << *max << " s, min: " << *min << " s";
				// Same from the publisher timestamps
				if(!current_pub_times.empty())
				{
					const auto [pub_mean, pub_stdev] = mean_stdev(current_pub_times);
					std::cout << " | pub rate: " << 1.0/pub_mean << " hz, pub stdev: " << pub_stdev << ", lost: " << current_lost;
				}
				std::cout << std::endl;
			}
			else
			{
				std::cout << elapsed.str() << "waiting for messages" << std::endl;
			}

			if(record_time > 0.0)
			{
				const std::chrono::duration<double> remaining = end - std::chrono::steady_clock::now();
				if(remaining.count() <= 0.0)
				{
					break;
				}
				std::this_thread::sleep_for(std::min(remaining, std::chrono::duration<double>(1.0)));
			}
			else
			{
				sleep(1);
			}
		}
		// Samples received after the last write
		if(log_csv)
		{
			std::vector<Sample> current_samples;
			{
				std::lock_guard<std::mutex> lock(status_mutex);
				current_samples.swap(csv_samples);
			}
			write_csv(current_samples);
		}
		if(stamp_missing)
		{
			std::cout << "--stamp: " << topic_name << " has no sequence_id and timestamp, at the top level or in its header" << std::endl;
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;

}