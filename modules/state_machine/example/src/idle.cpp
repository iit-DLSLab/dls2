#include "dls2/state_machine/washing_machine/idle.hpp"
#include "dls2/state_machine/washing_machine/washing_machine.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace washing_machine{
        Idle::Idle(WashingMachine *sm) : sm(sm) {}
        void Idle::activity(){
            while (!sm->isRaised(sm->quit))
            {
                std::cout << "IDLE state\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                sm->nextState(sm->start);
                break;
            }
            if(sm->isRaised(sm->quit))
            {
                sm->nextState(sm->quit);
            }
        }
    }
}