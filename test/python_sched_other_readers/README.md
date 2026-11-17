# Python SCHED_OTHER blind_state readers
Launch an arbitrary number of lightweight python subscribers of `rt/blind_state`, scheduled with `SCHED_OTHER`.
Useful to load the DDS discovery server / network with many python subscribers.

Each node is a single Fast DDS participant (the `dls2_py` `Reader`) polled by a periodic python loop.
It is not a dls2 PeriodicApp: no state machine, no supervisor, no activation needed.
(A dls2 PeriodicApp waits in IDLE until the framework sends it `activate`, and opens ~12 DDS participants per process.)

The scheduler settings are read from [plugin/config/scheduler.yaml](plugin/config/scheduler.yaml),
same format as the other dls2 plugins:

```yaml
sched_policy : SCHED_OTHER   # SCHED_OTHER or SCHED_FIFO (+ sched_priority)
period : 100                 # ms, loop period
```

Each node applies the policy to itself before creating any DDS thread, so all its threads inherit it.

Two terminals:

- T1: `dls --servers` (plus the HAL or any other `rt/blind_state` publisher)
- T2: `./test/python_sched_other_readers/launch_blind_state_readers.sh -n 20`

Main options (`-h` for all):

- `-n <num>` number of nodes
- `-c <yaml>` use another scheduler config (default `plugin/config/scheduler.yaml`)
- `-s <sec>` delay between launches
- `-f` print node output on the terminal instead of per-node log files
- `-D` dry run, only print the commands

Every `-w` seconds (default 2, integer) the launcher prints the number of samples received so far by each node:

```
[summary t=15s] received samples per node (blind_state_reader_<i>), '-' not subscribed yet, 'X' exited
   0: 4350         1: 4243         2: 4290         3: 4237         4: 4236         5: 0            6: - X
```

Counts are refreshed by the nodes every 0.5 s.
When the first node dies, the launcher prints its exit code and the tail of its log.

Every node also prints, each `-r` seconds, the receive rate measured in the DDS listener, the loop rate, the last `sequence_id` and `timestamp`.
Ctrl-C stops all the nodes. Run the script, do not source it.

If `BlindState.py` is not found automatically set `BLIND_STATE_READER_TYPES_PATH` (e.g. `/usr/lib/python3/dist-packages/dls_messages`).
Use `DLS2_PYTHON` to select the interpreter.
