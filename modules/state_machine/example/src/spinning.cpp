#include "dls2/state_machine/washing_machine/spinning.hpp"
#include "dls2/state_machine/washing_machine/washing_machine.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace washing_machine{
        Spinning::Spinning(WashingMachine *sm) : sm(sm) {}
        void Spinning::activity()
        {
            while (!sm->isRaised(sm->quit))
            {
                std::cout << "SPINNING state\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                sm->nextState(sm->done);
                break;
            }
            if(sm->isRaised(sm->quit))
            {
                sm->nextState(sm->quit);
            }
        }
    }
}