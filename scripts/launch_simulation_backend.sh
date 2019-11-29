#!/bin/bash

roslaunch hyq_description2 world.launch&
sleep 5
roslaunch dls2_to_roscontrol dls2_to_roscontrol.launch&
rosrun roscontrol_to_dls2 roscontrol_to_dls2_node

