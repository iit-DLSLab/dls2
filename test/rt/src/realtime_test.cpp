#include <dls2/application/sched_attr.hpp>
#include <cstring>
#include <chrono>
#include <cstdio>
#include <stdlib.h>
#include <csignal>
#include <iostream>
#include <thread>
#include <string>

bool run = true;

std::atomic_bool start_front(false);
std::atomic_bool start_back(false);

int count_front = 0;
int count_back = 0;
int count_tot = 0;

void quit(int){
    run = false;
}

void front_work(std::stop_token st){
    while(!st.stop_requested())
    {
        start_front.wait(false);
        if(!run)
            break;
        count_front++;
        start_front.store(false);
    }   
}

void back_work(std::stop_token st){
    while(!st.stop_requested())
    {
        start_back.wait(false);
        if(!run)
            break;
        count_back++;
        start_back.store(false);
    }
}

void waitWork(){
    // it is implemented as a spin lock (not efficient but unblocking)
    while(start_front.load() && start_back.load()){}
}

int main(int argc, char **argv)
{
    static_cast<void>(argc); //avoid warning in compilation
    static_cast<void>(argv); //avoid warning in compilation

    // handle CTRL+C
    signal(
        SIGINT,
        quit);

    std::jthread front(front_work);
    std::jthread back(back_work);

    // set realtime scheduler
    struct sched_attr scheduler_attributes;

    memset(&scheduler_attributes, 0, sizeof(struct sched_attr));
	scheduler_attributes.size = sizeof(struct sched_attr);
	scheduler_attributes.sched_policy = SCHED_DEADLINE;

    std::chrono::nanoseconds period(5'000'000);// 5ms
    std::chrono::nanoseconds runtime(5'000'000);
    std::chrono::nanoseconds deadline(5'000'000);
	scheduler_attributes.sched_period  = (unsigned long long) period.count();
	scheduler_attributes.sched_runtime = (unsigned long long) runtime.count();
	scheduler_attributes.sched_deadline = (unsigned long long) deadline.count();

	unsigned int flags = 0;
    int ret = sched_setattr(0, &scheduler_attributes, flags);
    if (ret < 0) {
        perror("sched_setattr");
        exit(-1);
    }

    // run
    while (run){
        // do work
        count_tot++;
        // unblock threads
        start_front.store(true);
        start_back.store(true);
        start_front.notify_one();
        start_back.notify_one();
        // wait work of threads
        waitWork();

        sched_yield();
    }

    // unblock threads
    start_front.store(true);
    start_back.store(true);
    start_front.notify_one();
    start_back.notify_one();

    std::cout << "\ncount: " << std::to_string(count_tot) <<  ", count_front: " + std::to_string(count_front) + ", count_back: " + std::to_string(count_back) << std::endl;
	return 0;
}