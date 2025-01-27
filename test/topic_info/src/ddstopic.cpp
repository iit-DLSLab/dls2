#include "dls2/util/messaging/dds_reader.hpp"

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <numeric>
#include <mutex>
#include <vector>

volatile sig_atomic_t stop;


// bool stop = false;
unsigned long int window_size = 1000;

std::vector<double> times{};
std::chrono::system_clock::time_point last = std::chrono::high_resolution_clock::now();
void * msg;  // just to avoid warnings during compilation..... :)
std::mutex status_mutex;

void my_handler(int s){
	std::cout << s << std::endl; // just to avoid warnings during compilation..... :')
	stop = true;
}


int main(int argc, char** argv){

	if (argc<2)
	{
		std::cout << "Wrong number of arguments: Minimum 2 arguments required" << std::endl;
		std::cout << "Usage examples:" << std::endl;
		std::cout << "\tddstopic list <(optional) domain>" << std::endl;
		std::cout << "\tddstopic participants <(optional) domain>" << std::endl;
		std::cout << "\tddstopic hz <topic_name> <(optional) domain>" << std::endl;
		return EXIT_FAILURE;
	}

	// CTRL-C handler
	signal(SIGINT, my_handler);

	const std::string command = argv[1];

    int domain = 3;

	if(command == "list" || command == "participants"){
		if(argc==3){
			domain = strtol(argv[2], NULL, 10);
		}
	}else if(command == "hz"){
		if(argc==4){
			domain = strtol(argv[3], NULL, 10);
		}
	}else{
		return EXIT_FAILURE;
	}

    dls::DDSParticipant participant("ddstopic", domain, eprosima::fastdds::rtps::DiscoveryProtocol::SUPER_CLIENT);

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
		const std::string topic_name = argv[2];

		participant.addReader(topic_name+"_reader",
							dls::topicType(topic_name, participant.get_discovery_database()[topic_name]),
							std::function<void(void *)>
							{
								[&](void *tuple)
								{
									std::lock_guard<std::mutex> lock(status_mutex);
									std::chrono::system_clock::time_point now = std::chrono::high_resolution_clock::now();
									double diff = std::chrono::duration<double>(now-last).count();
									last = now;
									if(times.size()==window_size)
									{
										times.erase(times.begin());
									}
									times.push_back(diff);
									msg = tuple;
								}
							});

		//Loop that calculates the frequency, use CTRL + C to cancel
		while(!stop)
		{
			std::vector<double> current_times;
			{
				std::lock_guard<std::mutex> lock(status_mutex);
				current_times = times;
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
				std::cout <<"rate: " << 1.0/mean << " hz, stdev: " << stdev << ", max: " << max << " s, min: " << min << " s"<<std::endl;
			}

			// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			sleep(1);
		}
		


	}
	return EXIT_SUCCESS;

}