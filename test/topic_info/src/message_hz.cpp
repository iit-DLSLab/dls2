// // Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
// //
// // Licensed under the Apache License, Version 2.0 (the "License");
// // you may not use this file except in compliance with the License.
// // You may obtain a copy of the License at
// //
// //     http://www.apache.org/licenses/LICENSE-2.0
// //
// // Unless required by applicable law or agreed to in writing, software
// // distributed under the License is distributed on an "AS IS" BASIS,
// // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// // See the License for the specific language governing permissions and
// // limitations under the License.

// /**
//  * @file TypeLookup_main.cpp
//  *
//  */

// // #include "LookupPublisher.h"
// #include "LookupSubscriber.hpp"

// #include <fastrtps/log/Log.h>
// #include <iostream>

// using namespace eprosima::fastrtps;

// int main(
//         int argc,
//         char** argv)
// {
//     std::cout << "Starting " << std::endl;
//     int type = 1;
//     int count = 5;
//     long sleep = 100;
//     if (argc > 1)
//     {
//         if (strcmp(argv[1], "publisher") == 0)
//         {
//             type = 1;
//             if (argc >= 3)
//             {
//                 count = atoi(argv[2]);
//                 if (argc == 4)
//                 {
//                     sleep = atoi(argv[3]);
//                 }
//             }
//         }
//         else if (strcmp(argv[1], "subscriber") == 0)
//         {
//             type = 2;
//         }
//     }
//     else
//     {
//         std::cout << "publisher OR subscriber argument needed" << std::endl;
//         Log::Reset();
//         return 0;
//     }

//     switch (type)
//     {
//         // case 1:
//         // {
//         //     TypeLookupPublisher mypub;
//         //     if (mypub.init())
//         //     {
//         //         mypub.run(count, sleep);
//         //     }
//         //     break;
//         // }
//         case 2:
//         {
//             LookupSubscriber mysub;
//             if (mysub.init())
//             {
//                 mysub.run();
//             }
//             break;
//         }
//     }
//     Log::Reset();
//     return 0;
// }


#include "LookupSubscriber.hpp"
// #include "dls_messages/dds/controller_commandPubSubTypes.h"
// #include "dls_messages/dds/trunk_controller_debugPubSubTypes.h"
// #include <dls_messages/dds/base_statePubSubTypes.h>
// #include <dls_messages/dds/mpc_generator_outputPubSubTypes.h>

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <numeric>
#include <mutex>

bool stop = false;
unsigned long int window_size = 10000;
std::vector<double> times{};
std::chrono::system_clock::time_point last = std::chrono::high_resolution_clock::now();
void * msg;  // just to avoid warnings during compilation..... :)
std::mutex status_mutex;

void my_handler(int s){
	std::cout << s << std::endl; // just to avoid warnings during compilation..... :')
	stop = true;
}

int main(int argc, char** argv)
{
	if (argc<2)
	{
		std::cout << "Wrong number of arguments: it needs topic name" << std::endl;
		return EXIT_FAILURE;
	}

	// CTRL-C handler
   struct sigaction sigIntHandler;
   sigIntHandler.sa_handler = my_handler;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;
   sigaction(SIGINT, &sigIntHandler, NULL);

	// Choose domain
	int domain = 3;
	if(argc==3)
	{
		domain = strtol(argv[2], NULL, 10);
	}

	// Choose topic
	dls::topicType topic;
	
	const std::string topic_name = argv[1];
	if(topic_name == "blind_state")
	{
		topic = dls::topics::low_level_estimation::blind_state;
	}
	else if(topic_name == "base_state")
	{
		topic = dls::topics::high_level_estimation::base_state;
	}
	else if(topic_name == "controller_signal")
	{
		topic = dls::topics::controller_signal;
	}
	else if(topic_name == "trajectory_generator")
	{
		topic = dls::topics::trajectory_generator;
	}
	else if(topic_name == "t265_odometry")
	{
		topic = dls::topics::high_level_estimation::t265_odometry;
	}
	else if(topic_name == "trunk_controller_signal")
	{
		topic = dls::topicType("trunk_controller", new  ControlSignalMsgPubSubType());
	}
	else if(topic_name == "pid_signal")
	{
		topic = dls::topicType("pid", new  ControlSignalMsgPubSubType());
	}
	else if(topic_name == "trunk_controller_debug")
	{
		topic = dls::topicType("trunk_controller_debug", new  TrunkControllerDebugMsgPubSubType());
	}
	else if(topic_name == "desired_torques")
	{
		topic = dls::topics::desired_torques;
	}
	else if(topic_name == "mpc_controller")
	{
		topic = dls::topicType("mpc_controller", new  ControlSignalMsgPubSubType());
	}
	else if(topic_name == "mpc_generator_output")
	{
		topic = dls::topics::mpc_generator_output;
	}
	else
	{
		std::cout << "Wrong topic name" << std::endl;
		return EXIT_FAILURE;
	}

	dls::LookupListener sub
	(
		"dds_hz",
		domain,
		topic,
		std::function<void(void *)>
		{
			[&](void *tuple)
			{
				// std::cout << "received data" << std::endl;
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
		}
	);

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
	return EXIT_SUCCESS;
}