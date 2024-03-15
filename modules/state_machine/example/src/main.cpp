#include "dls2/state_machine/washing_machine/washing_machine.hpp"
#include <iostream>
#include <thread>

void self_standing();
void self_standing_thread();
void manual_triggering();

int main()
{
    std::cout << "Sequential test: testing start function. The execution of this thread waits that the state machine quits" << std::endl;
    self_standing();
    std::cout << "Parallel test: launching state machine in another thread and send asynchronous events" << std::endl;
    self_standing_thread();
    std::cout << "Manual test: manual execution of current state" << std::endl;
    manual_triggering();
    return 0;
}

void self_standing()
{
    state_machine::washing_machine::WashingMachine wash_machine;
    wash_machine.startSM();
}

void self_standing_thread()
{
    state_machine::washing_machine::WashingMachine wash_machine;
    std::thread thread(&state_machine::washing_machine::WashingMachine::startSM, &wash_machine);
    int timeout = 1000;
    std::cout << "WAIT " << timeout << "ms..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
    std::cout << "Send quit request"<< std::endl;
    wash_machine.raiseEvent(wash_machine.quit);
    thread.join();
}

void manual_triggering()
{
    state_machine::washing_machine::WashingMachine wash_machine;
    wash_machine.runState();
    if(*wash_machine.state == wash_machine.FILLING_WATER)
    {
        wash_machine.runState();
    }
    if(*wash_machine.state == wash_machine.WASHING)
    {
        wash_machine.runState();
    }
    // wash_machine.raiseEvent(wash_machine.quit);
    if(*wash_machine.state == wash_machine.RINSING)
    {
        wash_machine.runState();
    }
    if(*wash_machine.state == wash_machine.SPINNING)
    {
        wash_machine.runState();
    }
    if(*wash_machine.state == wash_machine.STOP)
    {
        wash_machine.runState();
    }
}