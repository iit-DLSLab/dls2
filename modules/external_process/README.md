# Managed external processes

`ManagedExternalProcess` keeps the asynchronous lifecycle API used by plugins:
`start()`, `running()`, `requestStop()`, `stopComplete()`, and `stopAndWait()`.
Repeated start/stop calls remain guarded. `running()` describes launcher liveness.

It delegates OS process ownership to `dls::utils::OwnedProcess` in
`dls2/util/owned_process.hpp`. That class creates a dedicated session/process group,
tracks leader and group liveness separately, and reaps children it owns.
`shutdownProcesses()` is shared with `run_dls2`:

- The framework interrupts the whole owned group.
- The plugin wrapper interrupts the launcher first, allowing ROS launch to stop
  its nodes, then escalates to SIGTERM and SIGKILL for the whole group.
- The wrapper retains configurable interrupt/terminate intervals and uses a
  two-second final wait after SIGKILL. Failure is propagated through the future;
  the destructor logs it. It does not report success merely because SIGKILL was sent.

The owning executable must enable child subreaping if it needs to reap orphaned
descendants itself; `run_dls2` does so. The reusable wrapper does not change this
process-wide setting. A separately grouped descendant is not covered by its
ancestor's group signals if its immediate owner is forcibly killed.

`ShutdownSignal` handles incoming SIGINT/SIGTERM in `run_dls2` and
`child_process_launcher`. Its callback requests application shutdown, which reaches
the plugin's cleanup hooks. `shutdownProcesses()` sends outgoing signals to owned
children; the plugin wrapper does not install another signal handler.

## Integration test

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
executable. It also checks session isolation, group liveness after leader exit,
both initial signal policies, and compatibility with the framework shutdown API.
It does not launch ROS or Nav2. Use `ctest -V` to see each scenario.
