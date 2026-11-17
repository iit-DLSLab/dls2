# Cpp-python communication testing
Testing all the off-the-shelf DLS2 idls, including ros2 interface. Cpp publish, python receive.

Three terminals:

- T1: `dls --servers`
- T2: `source modules/plugin/python/scripts/setup.sh && python3 build/dls2/test/cpp_python_communication/generated/all_idl_readers.py`
- T3: `./bin/Release/cpp_python_communication_all_writers`    

The test ends when the python node has received all the messages from all the writers.