# Procedures

## Introduction
This repositories contains a set of default procedures that can be run at run-time to standardize behaviours.

## What is a procedure?
A procedure is a sequence of commands that are sent to the DLS2 network to generate automated behaviours of your robot or, more in general, of your simulations/experiments. Those commands are the same commands you would write directly on the console.

A procedure it is implemented by inheriting from the AppPlugin class, so it follows the application state machine and can be customizable according to the rules of any class inheriting from the AppPlugin class.

When you define your custom procedure, it will be automatically loaded at run-time when launching the service layer, and it will go in the idle state.

To run the procedure, you just need to run in the console the command _Supervisor::runProcedure <procedure_name>_.

## Example
In the folder [stand_up](./stand_up/) you can find ad example of a procedure.

As you can see in this example, a command manager is used to send commands in the DLS2 network. Upon activation, it is a good practice to scan for the applications that have the commands you want to use. In this case, if they are not running, the procedure cannot start and it goes back to the idle state.

It is also a good practice to wait for commands to be visible before running it. Otherwise the _callCommand_ could not take any effect.

When creating your own procedure, remember to install the include in _\${DLS_PROCEDURES_HEADER_DIR}_ and the target in _\${DLS_INSTALL_PROCEDURE_DIR}_.