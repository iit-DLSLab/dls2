# Common actions
## Introduction
This repository provides a set of common action servers, like *goHome* and *goFold* procedures.

#### Prerequisites
Read at least at the [Introduction](https://fast-dds.docs.eprosima.com/en/latest/02-formalia/titlepage.html) and [Getting Started](https://fast-dds.docs.eprosima.com/en/latest/fastdds/getting_started/getting_started.html) sections of the FastDDS documentation.

## How to create and use an action server
* Create a class inheriting from ActionServerBase, implementing the *runAction()* function. As an example, you can look at the *goHome* and *goFold* folders. You can create custom topics and messages if needed
* Once the class is created, you need to use an action client to send the goal to the action server. To do that you can:
    * Make sure the action server is loaded: launch *loadAction <action_name>* from the command line
    * Use the ActionClient class: TODO
  If you want to send a goal from the console, you can create a console command in you plugin. In this console function you need to send the *runAction* command to the service layer in this way: TODO

  The *runAction* function:
  * load the action server
  * send the goal
  * once the action is finished, it closes the action server