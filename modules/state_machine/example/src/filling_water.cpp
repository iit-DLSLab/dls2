#include "dls2/state_machine/washing_machine/filling_water.hpp"
#include "dls2/state_machine/washing_machine/washing_machine.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace state_machine
{
    namespace washing_machine{
        FillingWater::FillingWater(WashingMachine *sm) : sm(sm) {}
        void FillingWater::activity()
        {
            while (!sm->isRaised(sm->quit))
            {
                std::cout << "FILLING_WATER state\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                sm->nextState(sm->filled_to_5l);
                break;
            }
            if(sm->isRaised(sm->quit))
            {
                sm->nextState(sm->quit);
            }
        }
    }
}