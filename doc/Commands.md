# What is a command in DLS2?
As you might already know, DLS2 is a distributed system, where each entity/module can easliy join the pletora of entities communicating in a specific domain (or connected to a server). Such entities can define a set of console commands that an user can execute to interactively change their behaviour while running all together. To comply with the distributed structure, a publisher-subscriber paradigm is implemented to execute a console command belonging to a specific entity.

## DDS entities
Each entity has a _CommandManager_. It owns **a single Domain Participant (DP)** named `<CommandOwner>::commands_monitor` in the _dls::domains::command_ DDS domain, whatever the number of commands of the entity. This DP holds only two DDS entities, both on the _command_call_ topic:

| entity | role |
|---|---|
| Data Writer (DW) `<CommandOwner>::commands_monitor` | sends the command calls, the list requests and the list replies |
| Data Reader (DR) `<CommandOwner>::commands_listener` | receives the command calls (serving **all** the commands of the entity), the list requests and the list replies |

Every message on _command_call_ is a `dls2_interface::msg::CommandCall`, whose `owner` field is the **destination** of the message (empty for all the entities). The DR of each CommandManager drops the messages addressed to another entity, and dispatches the others according to the command name:

| `command_name` | message | other fields |
|---|---|---|
| a command name | command call | `args`: arguments, comma separated |
| `__dls_list_commands__` (reserved) | list request | `args`: request id, `header.frame_id`: requester |
| `__dls_list_reply__` (reserved) | list reply (`owner` is the requester) | `args`: active commands, comma separated, `ret`: request id, `header.frame_id`: replying entity |

The commands themselves do not create any DDS entity: each command is stored in an internal map of the CommandManager (command name → function and argument types).

Both DDS entities are RELIABLE, KEEP_ALL and VOLATILE: a sample is resent until every matched reader has acknowledged it, and a reader never overwrites a sample it has not read yet.

## Threads
Each CommandManager runs three threads (`std::jthread`, stopped through their stop token when the CommandManager is destroyed):
- **executor**: executes the received command calls;
- **replier**: replies to the received list requests;
- **level watcher**: activates/deactivates the commands when the level (state) of the CommandManager changes (see below).

The DDS listener of the single DR does not execute or publish anything: it queues the received command calls for the executor and the list requests for the replier, and stores the received list replies for the waiting query.

## How a command is called
When typing an existing command name in the console, the DW of the console layer publishes it on the _command_call_ topic, with the owner of the command and its arguments. The DR of each CommandManager receives it. If the owner is its own (or empty, meaning all the entities) the call is queued. The executor then executes it if the command name is one of the **active** commands of the entity, otherwise the call is ignored.

The commands of an entity are therefore executed **one at a time, in the order they are received**: a long command (e.g. one waiting for a user input with `CommandHelper::readValue`) delays the next commands of the same entity. The commands of different entities are still executed in parallel.

A command owned by the calling CommandManager itself is executed directly by the calling thread, without going through DDS.

## How the commands are discovered
The available commands are discovered with a client-server mechanism:
1. A client (e.g. the console, when listing the commands or calling one of them) publishes a list request on _command_call_ (reserved command name `__dls_list_commands__`), addressed to the entity to query (or to all the entities), with a request id.
2. Each queried CommandManager (server) replies on _command_call_ (reserved command name `__dls_list_reply__`), addressed to the client, with the list of its active commands. The reply is sent by the replier thread, so a running command does not delay it and the entity does not look gone while it is busy.
3. The client waits until every expected entity has replied, or until a timeout expires, and then returns the commands received. The expected entities are the CommandManagers matched with its DW (the DPs named `<owner>::commands_monitor`), or only the queried one when an owner is given.

The CommandManager functions using this mechanism are `getCommandsList()`, `findByName()`, `findByOwner()`, `find()`, `callCommand()` and `waitCommand()`. Each of them costs one list request, i.e. a round trip with the queried entities (about 1 ms on the same machine once the entities are matched). An entity started less than about 1-2 s before the request may not be matched yet, and is missing from that reply only.

The timeout is a setting of the CommandManager: it can be passed to the constructor, or changed with `setListReplyTimeout(ms)` (`getListReplyTimeout()` returns it). The default is `CommandManager::DEFAULT_LIST_REPLY_TIMEOUT_MS` (300 ms). It is reached only when an expected entity does not reply, e.g. while it is shutting down.

To avoid flooding the entities with list requests (e.g. a loop of `callCommand`/`waitCommand`), the result of a query is reused by the following queries of the same CommandManager for a short time, set with `setListCacheValidity(ms)` (`getListCacheValidity()` returns it; default `CommandManager::DEFAULT_LIST_CACHE_VALIDITY_MS`, 150 ms; 0 disables the reuse). Concurrent queries of the same CommandManager share a single request. A command activated or deactivated in another entity during this time is seen by the first query after it, while a change of the commands of the CommandManager itself is seen at once. The console asks for the list only once per `Tab` press.

A command is identified by the owner, i.e. the entity that has created it, and the actual command name. The format is as follows

    CommandOwner::command_name

where _CommandOwner_ is the name of the entity that has created the command, while _command_name_ is the actual name of the command.

For example in this case

    periodic::startMotion

the entity _periodic_ has created a command called _startMotion_.

# How a command is activated
Each entity might have several commands. However, a command might have preconditions to be match before it can be made available. For example, there is no need to stop the robot motion if it is not moving. So there is no need of a _stopMotion_ command if _startMotion_ has not been executed. Therefore, a state machine is used to identify which command is available in which state, avoiding to provide unuseful commands.

Let's try to understand with an example. Consider the entity _periodic_. This motion generator can have the following commands:
- _activate_: the motion generator starts sending feet desired references. Such desired configuration is the one the robot has when activating the generator
- _deactivate_: the generator stops sending references
- _goHome_: send desired feet positions corresponding to the home configuration.
- _startMotion_: start the robot motion (e.g. trotting)
- _stopMotion_: stop the robot motion

The following state machine describes what happens when executing each of these commands:

\image html commands.png width=800cm

In each state only a subsets of commands is available. In STATE 0 only _activate_ is available and, when executed, the state machine goes in STATE 1 where _goHome_, _startMotion_ and _deactivate_ are now available, but _activate_ is not anymore. _deactivate_ brings the system back to STATE 0, _goHome_ keeps the state machine in STATE 1 and _startMotion_ moves it in STATE 2, where _deactivate_ and _stopMotion_ are available, but _goHome_ is not. _stopMotion_ moves the state back to STATE 1. 

There is a state machine per owner. So if for example it is running the _pid_ and _trunk_controller_ controllers, the _periodic_ motion generator and the _leg_odometry_ state estimator, each of them has its own state machine managing its own commands.

The user should take care about building the state machine for each entity she/he is defining. Such state machine prevents to run commands that depends on the outputs of others, thus improving the robustness of the simulation/experiment.

# How to define a command
Each command belongs to an instance of the *CommandManager* class. The command manager is responsible for the creation of commands and for their activation/deactivation depending on the command state machine. There is a state machine per *CommandManager* instance.

Let's consider that you have an instance of *CommandManager* called *command_manager*. To define a command you have to use the *addCommand* function:

    command_manager.addCommand( "command_name",
                                "comment_of_the_command",
                                &ClassName::function_name,
                                instance_ptr,
                                states_transition);

In this function you specify:
  * the console command name, which is used from the command line to invoke the function. It cannot be empty, contain `,` (used as separator in the command lists) or start with `__dls` (reserved): such commands are not added and an error is printed
  * the console command description
  * *ClassName::function_name*, that is the reference to the function to be added as command, belonging to *ClassName* class
  * *instance_ptr*, which is the pointer to an instance of *ClassName*
  * *states_transition*, which is a set of tuples defined as *{{state_from, state_to}, ...}*. Each tuple *{state_from, state_to}* is a set of states (intergers) where
    * *state_from* is a state where the command is available
    * *state_to* is a state where the state machine goes, when executing the command from *state_from*

As an example, let's define the commands of the previous example

        command_manager.addCommand
	    (
            "activate",
            "Activate Periodic Generator",
            &Periodic::activate,
            this,
            {{0,1}}
	    );
        command_manager.addCommand
	    (
            "deactivate",
            "Deactivate Periodic Generator",
            &Periodic::deactivate,
            this,
            {{1,0},{2,0}}
	    );
        command_manager.addCommand
	    (
            "goHome",
            "Execute go home procedure",
            &Periodic::goHome,
            this,
            {{1,1}}
	    );
        command_manager.addCommand
	    (
            "startMotion",
            "Start the motion",
            &Periodic::startMotion,
            this,
            {{1,2}}
	    );
        command_manager.addCommand
	    (
            "stopMotion",
            "Stop the motion",
            &Periodic::stopMotion,
            this,
            {{2,1}}
	    );

As you can see, there are 1 or more transition states. You could also define a command with no transition, with the empty set *{}* (not *{{}}*). In this case the command is always available.

Notice also that we have used *this* as pointer to the instance class of Periodic. This is because in this example the console commands were added in the same class (Periodic) where the functions associated to the commands are defined. As you can see [here](https://gitlab.advr.iit.it/dls-lab/dls2/-/tree/develop/modules%2Fplugin%2Fskeletons%2Fperiodic#create-custom-console-commands), this is always the case.