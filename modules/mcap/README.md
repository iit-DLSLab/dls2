# MCAP

## Introduction
[MCAP](https://mcap.dev/guides) (pronounced "em-cap") is a modular container file format for heterogeneous timestamped data. It is ideal for robotics applications, as it can record multiple streams of structured and unstructured data (e.g. ROS, Protobuf, JSON Schema, etc.) in a single file.

## MCAP integration in DLS2
To integrate MCAP in DLS2, the [MCAP cpp](https://github.com/foxglove/mcap/tree/main/cpp) headers have been included and two main libraries have been installed in the DLS2 docker image as dependencies:

* lz4
* zstd

This allowed us to include only the essential files needed for using MCAP and getting rid of *Conan*, the tool used to build the entire MCAP repository.

The headers are used by the *MCAPUtils* class, implemented to decouple MCAP and Foxserver, and to easily allow this to use the MCAP methods for generating MCAP log file.

## How to record a new MCAP log file
To record a new MCAP log file, you have to launch first the **Log** layer with:

**Terminal 1**
```
$ dls -llog
```

this will launch the Foxserver, that:
* reads the data exchanged through Fast DDS topics
* uses the *MCAPUtils* functions to publish topics data 
* enables Foxglove, who acts as a reader for the MCAP messages

If you now launch the **Console** layer with:

**Terminal 2**
```
$ dls -lconsole
```

you can choose to run:
* **startRecording** : to record a new MCAP log file
* **stopRecording** : to stop an active MCAP log file recording

The MCAP log file will be automatically stored in your DLS2 workspace.

## How to inspect an MCAP log file
In the DLS2 docker image, there is the [MCAP CLI](https://mcap.dev/guides/cli) tool install, that allows to work with MCAP files.

For example, from the DLS2 docker image you can run the following and see the details about the log recorded:
```
$ mcap info <MCAP_log_file_name>
```

or you can convert a rosbag in a new MCAP file with:
```
$ mcap convert <ROSBAG_file_name> <MCAP_log_file_name>
``` 

A complete documentation about this tool is available at the link above.

In addition to this, you can use the *printTopicsMCAPDataMap* function of *MCAPUtils* to see the topics and associated MCAP channels and number of messages published, while recording.

## Resources
* [MCAP official documentation](https://mcap.dev/)
* [MCAP official GitHub repository](https://github.com/foxglove/mcap)