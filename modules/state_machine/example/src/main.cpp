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
    state_machine::washing_machine::WashingMachine washing_machine;
    washing_machine.start();
}

void self_standing_thread()
{
    state_machine::washing_machine::WashingMachine washing_machine;
    std::thread thread(&state_machine::washing_machine::WashingMachine::start, &washing_machine);
    int timeout = 1000;
    std::cout << "WAIT " << timeout << "ms..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
    std::cout << "Send quit request"<< std::endl;
    washing_machine.raiseEvent(washing_machine.quit);
    thread.join();
}

void manual_triggering()
{
    state_machine::washing_machine::WashingMachine washing_machine;
    washing_machine.runState();
    if(*washing_machine.state == washing_machine.FILLING_WATER)
    {
        washing_machine.runState();
    }
    if(*washing_machine.state == washing_machine.WASHING)
    {
        washing_machine.runState();
    }
    // washing_machine.raiseEvent(washing_machine.quit);
    if(*washing_machine.state == washing_machine.RINSING)
    {
        washing_machine.runState();
    }
    if(*washing_machine.state == washing_machine.SPINNING)
    {
        washing_machine.runState();
    }
    if(*washing_machine.state == washing_machine.STOP)
    {
        washing_machine.runState();
    }
}