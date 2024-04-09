# Common actions
## Introduction
This repository provides a set of common action servers, like *goHome* and *goFold* procedures.

#### Prerequisites
Read at least at the [Introduction](https://fast-dds.docs.eprosima.com/en/latest/02-formalia/titlepage.html) and [Getting Started](https://fast-dds.docs.eprosima.com/en/latest/fastdds/getting_started/getting_started.html) sections of the FastDDS documentation.

## How to create and use an action server
* Create a class inheriting from ActionServerBase, implementing the *runAction()* function. As an example, you can look at the *goHome* and *goFold* folders. You can create custom topics and messages if needed
* Once the class is created, you need to use an action client to send the goal to the action server.