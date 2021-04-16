#!/bin/bash
#echo ${1} ${2}
sed -i "s/setName(\"\(.*\)\")/setName\(\"unique_identifer_msgs::msg::dds_::\1_\"\)/" ${1}/${2}PubSubTypes.cxx
