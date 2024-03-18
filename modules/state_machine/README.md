# State machine
This repository contains the implementation of a simple finite state machine interface. The execution of the state machine can be either controlled by explicitly calling state transitions, or the state transitions can be executed autonomously.

# Overview
A state machine is defined by:
* states
* events (either asynchronous or based on conditions)
* transition table

The transition table maps the current state to the next state, based on the happened event.

States and events are implemented as classes. This allows to abstract from any specific implementation of a state machine.

An activity is associated with each state. The activity describes the work performed when the state machine is in that state and handles the transition to the next states based on the occurrence of the events.

Events can be asynchronous, which means that the state machine needs to detect the occurrence of such event at any time. To do that, asynchronous events are a specialization of events, and in the state machine they are associated to a boolean value: when it is true, the asynchronous event has appened, false otherwise. If an event happens in a state that does not handle it, the event is discarded. So there is no memory about past events.

The _StateMachine_ class implements the base class of a state machine. It stores:
* the current state
* the transition table
* the asynchronous event to boolean mapping

It can be run autonomously. For example, you can launch the state machine in a thread, by executing the _start()_ function. The state transitions can also be explicitly triggered, by calling _transit(event)_. 

# How to create a state machine
In the folder [example](example) there is an example implementing a specialization of the state machine. You can notice that:

* the state machine inherits from _StateMachine_ class
* each state is a separate class implementing the _activity()_ function, which performs the tasks of the state. Such activity can be implemented in such a way that when the tasks are completed, the state machine goes in the next state directly. This is the implementation in the example. But, you can also have a explicitly event-driven state machine. In this case, at the end of the activity, the state machine does not need to automatically go in the next state. You can then call the _transit(event)_ function to transit to the next state and its tasks are then executed
* each state stores the pointer to the state machine, to acces to member functions and events
* each event and asynchronous event is also implemented as a separate class
* the state machine creates a member variable for each state and event
* the transition table and the asynchronous event to boolean mapping are defined in the state machine constructor