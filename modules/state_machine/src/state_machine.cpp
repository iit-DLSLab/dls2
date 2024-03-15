#include "dls2/state_machine/state_machine.hpp"
#include <iostream>
namespace state_machine
{
    int Entity::num_entity = 0;
    Entity::Entity() { id = ++num_entity; }
    bool Entity::operator<(const Entity &rhs) const { return id < rhs.id; }
    bool Entity::operator==(const Entity& other) const
    {
        return this->id==other.id;
    }

    void State::activity() {} // do nothing

    StateMachine::StateMachine() : state(nullptr), quit(false) {}

    void StateMachine::init(State *state, const std::map<std::pair<State *, Event>, State *> &transitions, const std::vector<AsyncEvent> &async_events)
    {
        this->state = state;
        this->transitions = transitions;
        for (auto &event : async_events)
        {
            this->is_async_event[event].store(false);
        }
    }

    void StateMachine::startSM()
    {
        while (!quit)
            runState();
    }

    void StateMachine::stopSM()
    {
        quit = true;
    }

    void StateMachine::runState()
    {
        if(!quit)
            state->activity();
    }

    bool StateMachine::raiseEvent(const AsyncEvent &event)
    {
        // discard event if not usable in the current state --> does not remember event happened in past states
        if (transitions.find({state, event}) == transitions.end())
        {
            return false;
        }
        else
        {
            is_async_event[event].store(true);
        }
        return true;
    }

    void StateMachine::consumeEvent(const AsyncEvent &event)
    {
        is_async_event[event].store(false);
    }

    bool StateMachine::isRaised(const AsyncEvent &event)
    {
        return is_async_event[event].load();
    }

    void StateMachine::nextState(const Event &event)
    {
        state = transitions[{state, event}];
        if(state==nullptr)
        {
            throw std::runtime_error("dls2 state machine - transition does not exist");
        }
    }

    void StateMachine::nextState(const AsyncEvent &event)
    {
        consumeEvent(event);
        state = transitions[{state, event}];
        if(state==nullptr)
        {
            throw std::runtime_error("dls2 state machine - transition does not exist");
        }
    }

    void notifyState()
    {
        // TODO
    }

}