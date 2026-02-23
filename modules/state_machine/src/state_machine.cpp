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

    State::State(const std::string& name, bool realtime) 
    : name(name)
    , realtime(realtime){}
    State::State()
    : name("")
    , realtime(false){};
    State::~State(){}
    void State::makeRealTime(){
        realtime = true;
    }

    StateMachine::StateMachine(const std::string& name)
    : name(name)
    , state(nullptr)
    , quit(false)
    , notifier(name, dls::domains::layers, dls::topics::state_machine)
    {
        eprosima::fastdds::dds::DataWriterQos qos(eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT);
        qos.history().kind = eprosima::fastdds::dds::KEEP_ALL_HISTORY_QOS;
        qos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;
        qos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
        
        notifier.setQos(qos);

        state_machine_msg.app_name() = name;
        state_machine_msg.state() = "";
        state_machine_msg.realtime() = false;
    }
    StateMachine::~StateMachine(){}
    void StateMachine::init(
                            State *state,
                            const std::map<std::pair<State *, Event>, State *> &transitions, 
                            const std::vector<AsyncEvent> &async_events)
    {
        initState(state);
        this->transitions = transitions;
        for (auto &event : async_events)
        {
            this->async_events[event].store(false);
        }

        runState();
    }

    void StateMachine::initState(State *state)
    {
	{
		std::lock_guard<std::mutex> lock(state_mutex);
        this->state = state;
		this->desired_state_ = state;
		this->desired_state_timestamp_ = std::chrono::steady_clock::now();
	}
        notifyState();
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
	// discard event if not usable in the current state --> does not remember event happened in past
	// states
	auto des_state_it = transitions.find({state, event});
	if (des_state_it == transitions.end())
        {
            return false;
        }
        else
        {
		{
			std::lock_guard<std::mutex> lock(state_mutex);
			desired_state_ = des_state_it->second;
			desired_state_timestamp_ = std::chrono::steady_clock::now();
		}

            async_events[event].store(true);
            async_cv.notify_all();
        }
        return true;
    }

    void StateMachine::consumeEvent(const AsyncEvent &event)
    {
        async_events[event].store(false);
    }

    bool StateMachine::isRaised(const AsyncEvent &event)
    {
        return async_events[event].load();
    }

    void StateMachine::nextState(const Event &event)
    {
	 State * new_state = nullptr;
	{
		std::lock_guard<std::mutex> lock(state_mutex);
		new_state = transitions[{state, event}];
	}
	if (new_state == nullptr)
        {
            std::cerr << "dls2 state machine - transition does not exist" << std::endl;
        }
        else
	{
		{
			std::lock_guard<std::mutex> lock(state_mutex);
			state = new_state;
			desired_state_ = state;
			desired_state_timestamp_ = std::chrono::steady_clock::now();
		}
            notifyState();
	}
    }

    void StateMachine::nextState(const AsyncEvent &event)
    {
        consumeEvent(event);

	State * new_state = nullptr;
	{
		std::lock_guard<std::mutex> lock(state_mutex);
		new_state = transitions[{state, event}];
	}

	if (new_state == nullptr)
        {
            std::cerr << "dls2 state machine - transition does not exist" << std::endl;
        }
        else
	{
		{
			std::lock_guard<std::mutex> lock(state_mutex);
			state = new_state;
			desired_state_ = state;
			desired_state_timestamp_ = std::chrono::steady_clock::now();
		}
            notifyState();
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

std::string StateMachine::getStateName()
{
	std::lock_guard<std::mutex> lock(state_mutex);
	return state->name;
}
std::string StateMachine::getDesiredStateName()
{
	std::lock_guard<std::mutex> lock(state_mutex);
	return desired_state_->name;
}
std::chrono::steady_clock::time_point StateMachine::getDesiredStateStamp()
{
	std::lock_guard<std::mutex> lock(state_mutex);
	return desired_state_timestamp_;
}

    void StateMachine::notifyState()
    {
        state_machine_msg.state() = state->name;
        state_machine_msg.realtime() = state->realtime;

        notify();
    }

    void StateMachine::notifyRT(bool realtime)
    {
        state_machine_msg.realtime() = realtime;
        notify();
    }

    void StateMachine::notify(){
        notifier.sendMessage(&state_machine_msg);
    }
}