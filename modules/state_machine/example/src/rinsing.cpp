#include "dls2/state_machine/washing_machine/rinsing.hpp"
#include "dls2/state_machine/washing_machine/washing_machine.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace washing_machine{
        Rinsing::Rinsing(WashingMachine *sm) : sm(sm) {}
        void Rinsing::activity()
        {
            while (!sm->isRaised(sm->quit))
            {
                std::cout << "RINSING state\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                sm->nextState(sm->enlapsed_20_min);
                break;
            }
            if(sm->isRaised(sm->quit))
            {
                sm->nextState(sm->quit);
            }
        }
    }
}