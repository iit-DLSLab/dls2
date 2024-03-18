#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <map>
#include <atomic>
#include <memory>
#include <vector>

namespace state_machine
{
    // Add virtual destructor

    class Entity
    {
    public:
        Entity();
        virtual ~Entity();
        // id, num_entity and operator< are needed only to use an entity in std::map
        bool operator<(const Entity &rhs) const;
        bool operator==(const Entity& other) const;
    protected:
        int id;
    private:
        static int num_entity;
    };
    class State : public Entity
    {
    public:
        virtual void activity() = 0;
    };
    class Event : public Entity
    {
        public:
            virtual ~Event();
    };
    class AsyncEvent : public Event
    {
    };

    class StateMachine
    {
    public:
        // Constructor
        StateMachine();
        // Destructor
        virtual ~StateMachine();
        // start the state machine, by executing the current state activity in a while loop
        void startSM();
        // raise an asynchronous event in the state machine. Set variable of the asynch event to true
        bool raiseEvent(const AsyncEvent &event);
        // check if the state machine is ended
        bool quit;
        // initialize the state machine
        void init(State *state, const std::map<std::pair<State *, Event>, State *> &transitions, const std::vector<AsyncEvent> &async_events);
        // run the current state activity
        void runState();
        // stop the state machine
        void stopSM();
        // check whether the asynchronous event has happened
        bool isRaised(const AsyncEvent &event);
        // go to the next state based on the input event
        void nextState(const Event &event);
        // go to the next state based on the input asynch event and consume it (i.e. set to false the corresponding variable)
        void nextState(const AsyncEvent &async_event);
        // go to the next state based on the input event, and execute the next state
        void transit(const Event &event);
        // go to the next state based on the input asynch event, consume it (i.e. set to false the corresponding variable) and execute the next state
        void transit(const AsyncEvent &async_event);
        
        // current state
        State *state;

        // Transition table
        std::map<std::pair<State *, Event>, State *> transitions;

    private:
        // set variable of the asynch event to false
        void consumeEvent(const AsyncEvent &async_event);
        // notify that the state machine has changed its state
        void notifyState();

        // Store asynchronous event occurrence.
        // -- When the asynch event happens, the correspondig boolean value is set to true
        // -- When the fsm change state based on an asynch event, the correspondig boolean value is set to false
        std::map<AsyncEvent, std::atomic_bool> is_async_event;
    };
}

#endif /* end of include guard: STATE_MACHINE_HPP */