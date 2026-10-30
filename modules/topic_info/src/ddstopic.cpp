#include "dls2/util/messaging/dds_participant.hpp"

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
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

std::vector<double> times{};
std::chrono::high_resolution_clock::time_point first;
std::chrono::high_resolution_clock::time_point last;
bool received = false;  // false until the first message, which only starts the measurement
bool log_csv = false;
std::vector<std::pair<double, double>> csv_samples{};  // (time, period) collected since the last CSV write
void * msg;  // just to avoid warnings during compilation..... :)
std::mutex status_mutex;

void my_handler(int s){
	std::cout << s << std::endl; // just to avoid warnings during compilation..... :')
	stop = true;
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
		std::cout << "\tddstopic hz <topic_name> <(optional) domain> [--csv <file>] [--time <seconds>]" << std::endl;
		std::cout << "\t\t--csv <file>: log time, period and rate of every received message" << std::endl;
		std::cout << "\t\t--time <seconds>: stop after this time instead of waiting for CTRL+C" << std::endl;
		return EXIT_FAILURE;
	}

	// CTRL-C handler
	signal(SIGINT, my_handler);

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
			std::cout << "Usage: ddstopic hz <topic_name> <(optional) domain> [--csv <file>] [--time <seconds>]" << std::endl;
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
		csv << "time_s,period_s,rate_hz\n" << std::setprecision(9);
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
									msg = tuple;
									// No period yet on the first message: measuring from the program start would count the discovery time
									if(!received)
									{
										received = true;
										first = last = now;
										return;
									}
									double diff = std::chrono::duration<double>(now-last).count();
									last = now;
									if(log_csv)
									{
										csv_samples.emplace_back(std::chrono::duration<double>(now-first).count(), diff);
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
		auto write_csv = [&](const std::vector<std::pair<double, double>>& samples)
		{
			for(const auto& [time, period]: samples)
			{
				csv << time << "," << period << "," << (period > 0.0 ? 1.0/period : 0.0) << "\n";
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
			std::vector<std::pair<double, double>> current_samples;
			{
				std::lock_guard<std::mutex> lock(status_mutex);
				current_times = times;
				current_samples.swap(csv_samples);
			}
			if(log_csv)
			{
				write_csv(current_samples);
			}
			double size = current_times.size();
			if(size>0)
			{
				// Compute mean, standard deviation, max and min values
				double mean = 0.0;
				double max = 0.0;
				double min = 10.0; //10 seconds as upper bound is a lot so it is fine
				for(int i = 0; i< size; i++)
				{
					double value = current_times[i];
					mean+= value/size;
					if(value>max){max = value;}
					if(value<min){min = value;}
				}
				double diff = 0.0;
				for(int i = 0; i< size; i++)
				{
					diff += pow(current_times[i] - mean,2)/size;
				}
				double stdev = sqrt(diff);
				std::cout << elapsed.str() << "rate: " << 1.0/mean << " hz, stdev: " << stdev << ", max: " << max << " s, min: " << min << " s"<<std::endl;
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
			std::vector<std::pair<double, double>> current_samples;
			{
				std::lock_guard<std::mutex> lock(status_mutex);
				current_samples.swap(csv_samples);
			}
			write_csv(current_samples);
		}
		


	}
	return EXIT_SUCCESS;

}