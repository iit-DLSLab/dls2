#include "dls2/state_machine/washing_machine/stop.hpp"
#include "dls2/state_machine/washing_machine/washing_machine.hpp"

#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace washing_machine{
        Stop::Stop(WashingMachine *sm) : sm(sm) {}
        void Stop::activity(){
            std::cout << "STOP state\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            sm->stop();
        }
    }
}