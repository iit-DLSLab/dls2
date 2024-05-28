#include "dls2/state_machine/washing_machine/washing.hpp"
#include "dls2/state_machine/washing_machine/washing_machine.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace washing_machine{
        Washing::Washing(WashingMachine *sm) : sm(sm) {}
        void Washing::activity()
        {
            while (!sm->isRaised(sm->quit))
            {
                std::cout << "WASHING state\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                sm->nextState(sm->enlapsed_45_min);
                break;
            }
            if(sm->isRaised(sm->quit))
            {
                sm->nextState(sm->quit);
            }
        }
    }
}