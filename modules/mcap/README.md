# MCAP

## Introduction
[MCAP](https://mcap.dev/guides) (pronounced "em-cap") is a modular container file format for heterogeneous timestamped data. It is ideal for robotics applications, as it can record multiple streams of structured and unstructured data (e.g. ROS, Protobuf, JSON Schema, etc.) in a single file.

## MCAP integration in DLS2
To integrate MCAP in DLS2, the [MCAP cpp](https://github.com/foxglove/mcap/tree/main/cpp) headers have been included as *third_party* and two main libraries have been installed in the DLS2 docker image as dependencies:

* **lz4**
* **zstd**

This allowed us to include only the main files needed for using MCAP and getting rid of [Conan](https://conan.io/), the tool used to build the entire MCAP repository.

The following classes have been implemented:
* **MCAPWriterUtils** - to record a new MCAP log file
* **MCAPReaderUtils** - to read an MCAP log file
* **MCAPReaderSupport** - to playback an MCAP log file (publishing its messages on DDS topics)

These classes are used by the **WebSocketTranslator** class, that is responsible for both launching Foxglove and encoding the messages exchanged on DDS topics into JSON, used to creade MCAP log files.\
Foxglove is a *Service* that does not start automatically, but can be enabled whenever needed. If you want to use MCAP, you need to launch it from the **Console** layer with:
```
$ dls -lconsole
$ loadService dls_web_socket_translator
```
**Note**\
If you do not launch the **Log** layer (or if you launch it after other layers) the outcomes will be shown on the Console terminal.

## How to record a new MCAP log file
The **MCAPWriterUtils** class is used to record a new MCAP log file. If you want to start a new recording, you can run:
* **startRecording** - to record a new MCAP log file
* **stopRecording** - to stop an active MCAP log file recording

You can also use the *printTopicsMCAPDataMap* function to see the topics, associated MCAP channels and number of messages published, while recording.

## How to read and playback an MCAP log file
The **MCAPReaderUtils** is used to read MCAP log files. It is possible to print their content on the terminal while reading them by enabling the *print_mcap_log* parameter of the *readMCAPLog* function.

This class uses the *publishMessageOnTopic* function of the **MCAPReaderSupport** class to also publish the MCAP log file messages on the associated DDS topics. In this way it is possible to do a playback of the log file offline and test specific code with the recorded data. To do this, you can launch from the **Console** layer:
* **startPlaybackMCAP** - to read an MCAP log file and publish its data on DDS topics
* **stopPlaybackMCAP** - to stop an active MCAP log file playback.

## How to inspect an MCAP log file
To play an MCAP log file without using the implemented reader in DLS2, it is possible to load the log file in Foxglove or Plotjuggler, using them as MCAP readers.

To inspect an MCAP log file locally instead, you can run the [MCAP CLI](https://mcap.dev/guides/cli) tool installed in the DLS2 docker image and use its functionalities. Some important ones for example are:
```
$ mcap info <MCAP_log_file_name>.mcap
```
to see an MCAP log file statistics.

```
$ mcap convert <ROSBAG_file_name>.bag <MCAP_log_file_name>.mcap
``` 
to convert a rosbag in an MCAP file (with ros1 message encoding, not json).

```
$ mcap doctor <MCAP_log_file_name>.mcap
``` 
to check possible errors in an MCAP file structure.

```
$ mcap recover <MCAP_log_file_name>.mcap -o <output_MCAP_log_file_name>.mcap
```
to recover data from a potentially corrupt MCAP file.

A complete documentation about this tool is available at the link above.

## Resources
* [MCAP official documentation](https://mcap.dev/)
* [MCAP official GitHub repository](https://github.com/foxglove/mcap)