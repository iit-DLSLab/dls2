#include "dls2/state_machine/state_machine.hpp"
#include <iostream>
#include <thread>
namespace state_machine
{
    int Entity::num_entity = 0;
    Entity::Entity() { id = ++num_entity; }
    Entity::~Entity(){}
    bool Entity::operator<(const Entity &rhs) const { return id < rhs.id; }
    bool Entity::operator==(const Entity& other) const
    {
        return this->id==other.id;
    }

    Event::~Event(){}

    AsyncEvent::~AsyncEvent(){}

    StateMachine::StateMachine() : state(nullptr), quit(false) {}
    StateMachine::~StateMachine(){}
    void StateMachine::init(
                            State *state,
                            const std::map<std::pair<State *, Event>, State *> &transitions, 
                            const std::vector<AsyncEvent> &async_events)
    {
        this->state = state;
        this->transitions = transitions;
        for (auto &event : async_events)
        {
            this->is_async_event[event].store(false);
        }
    }

    void StateMachine::start()
    {
        while (!quit)
        {
            runState();
        }
    }

    void StateMachine::stop()
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
            async_cv.notify_all();
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
            std::cerr << "dls2 state machine - transition does not exist" << std::endl;
        }
    }

    void StateMachine::nextState(const AsyncEvent &event)
    {
        consumeEvent(event);
        state = transitions[{state, event}];
        if(state==nullptr)
        {
            std::cerr << "dls2 state machine - transition does not exist" << std::endl;
        }
    }

    void StateMachine::transit(const Event &event)
    {
        nextState(event);
        runState();
    }

    void StateMachine::transit(const AsyncEvent &event)
    {
        nextState(event);
        runState();
    }
    
    void StateMachine::waitAsynchEvent(const std::initializer_list<AsyncEvent>& async_events)
    {
        std::unique_lock lock(async_mutex);
        async_cv.wait(lock, [&] {
            bool condition = false;
            for(AsyncEvent event : async_events)
            {
                condition = condition || isRaised(event);
            }
            return condition;
            });
    }

    void notifyState()
    {
        // TODO
    }

}