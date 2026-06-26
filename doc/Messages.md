# Messages
This package defines and build off-the-shelf messages for DLS2 and their related wrappers. It also provides you the cmake function that can be used by the user to generate custom messages and wrappers.

# Message generation
Each message is defined by an idl file inside the [idls](idls) folder. See [here](https://fast-dds.docs.eprosima.com/en/latest/fastddsgen/dataTypes/dataTypes.html) for details about how to define a message in IDL format.

To generate messages for fastdds, the [fastddgen](https://fast-dds.docs.eprosima.com/en/latest/fastddsgen/introduction/introduction.html) tool is used (check the cmake function _dls_add_message_ inside [dls_message.cmake](cmake/dls_message.cmake) file).

# Wrapper concept and generation
A message wrapper is a C++ class that wraps the types defined in the idl file to C++ types. It is used in DLS2 to customize the C++ data types 


To have DLS2 independent from the middleware that is providing the messages (e.g. fastdds, cyclonedds, etc.), DLS2 defines wrappers around idls files that allows the user to customize, to certain extend, the data type to be used for each field of the idl.