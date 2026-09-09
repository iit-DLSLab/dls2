# External process integration test

From the PEGASUS repository root, build and run this module independently:

```sh
cmake -S dls2-barebone/dls2/modules/external_process \
  -B /tmp/dls-external-process-tests \
  -DDLS_EXTERNAL_PROCESS_BUILD_TESTS=ON
cmake --build /tmp/dls-external-process-tests -j2
ctest --test-dir /tmp/dls-external-process-tests --output-on-failure
```

Requires Linux, CMake, a C++17 compiler, and Boost filesystem/system development
libraries. ROS and the rest of DLS are not required for this standalone test build.


The test launches dummy child/grandchild processes and checks graceful shutdown,
forced shutdown of processes ignoring signals, cleanup after the launcher exits,
repeated start/stop requests, reactivation, destructor cleanup, and a missing
executable. It does not launch ROS or Nav2. Use `ctest -V` to see each scenario.
