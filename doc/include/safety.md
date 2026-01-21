Each app has a RobustEventNotifier entity acting as asynchronous event notifier and a DDSWriter acting as a periodic status notifier.
Both of them share the same goal: inform the supervisor about the current node situation in terms of logical health and resource consumption.

The safety layer is composed by a set of sanity checks that are performed in a separate monitoring thread for each object extending App class. The types of checks are strictly related to the functionalities of the node, meaning that a node extending from PeriodicAppPlugin will have a more complex set of checks rather than a node extending the App class only. Concerning the order, the sequence of checks reflects the object hierarchy, meaning that relevant concepts more related to the App class (e.g. internal state machine status) are firstly checked while quantities associated with PeriodicAppPlugin features (e.g. expected input frequency) are lastly checked. Each check is associated with an asynchronous event that is triggered as soon as the failure is detected and relevant data related to it are periodically published by the status notifier. All checks can be enabled/disabled or tuned modifying the `safety_layer.yaml` parameter file accordingly in order to shape the desired behavior of the safety layer (e.g. thresholds tuning). An additional check on RobustEventNotifier avoids spamming the same type of event (msg is published again after only after a tunable cool-down period).

A detailed list of all asynchronous event types is reported in the following: 

| Event ID      | Description   | Relevant parameters   | Possible cause
| --------      | -------       |---------              |---------------------
| `PROCESS_DIED`              |A node that has been launched/loaded by a layer app is supposed to execute but is not currently running | `enable_process_died` | Process internal error
| `WRONG_PROCESS_STATE`       |The underlying state machine implemented in the node is currently in a state different from the desired one (with 1 period tolerance for state switch) | `enable_wrong_process_state` | Unexpected state transition
| `WRONG_PROCESS_FREQUENCY`   |Current node frequency does not meet the desired frequency (with a 10% tolerance) | `enable_wrong_process_frequency` | Wrong parameters setup or anomaly in execution time 
| `MISSING_INPUT` | Node subscribers are not receiving data | `enable_missing_input` | Failure in publisher node or wrong topic name
| `CPU_USAGE_TOO_HIGH` | Overall or process-related cpu consumption is above threshold (average over a moving window)| `enable_cpu_usage_too_high`, `process_monitor_window_size`, `resource_monitor_window_size`, `cpu_threshold` | Poor resource management in code design
| `MEM_USAGE_TOO_HIGH` | Process memory consumption is above threshold (average over a moving window)| `enable_mem_usage_too_high`, `process_monitor_window_size`, `resource_monitor_window_size`, `mem_threshold` | Poor resource management in code design
| `INPUTS_NOT_SYNCHRONIZED` | Data received by node subscribers are not close enough in time | `enable_inputs_not_synchronized`, `sync_threshold` | Latency in the communication
| `WRONG_INPUT_FREQUENCY` | At least one subscriber is receiving data at frequency that is different from the expected one| `enable_wrong_input_frequency`, `max_exceeding_factor` | Wrong parameters setup or anomaly in publisher execution time 
| `WRONG_SEQUENCE_ID` | Subscriber detects a not correct sequential value in the sequence_id msg field (expected value is not met), if present | `enable_wrong_sequence_id` | Temporary communication loss
| `JOINT_POS_OUT_OF_BOUND` | | | 
| `JOINT_VEL_OUT_OF_BOUND` | | | 
| `JOINT_ACC_OUT_OF_BOUND` | | | 
| `JOINT_TORQUE_OUT_OF_BOUND` | | | 
| `REACHED_KINEMATIC_SINGULARITY` | | | 
| `DETECTED_AUTOCOLLISION` | | | 
| `NAN_VALUE` | | | 
| `INF_VALUE` | | | 
| `MOTOR_OVERHEAT` | | | 

A detailed list of all checks is reported in the following. The order reflects their implementation. As soon as one of them is triggered an event is published by the event notifier. 

* App: `WRONG_PROCESS_STATE`
* Layer: App checks + `PROCESS_DIED` (in a different thread)
* PeriodicApp: App checks + `WRONG_PROCESS_FREQUENCY`, `CPU_USAGE_TOO_HIGH`, `MEM_USAGE_TOO_HIGH`
* PeriodicAppPlugin: PeriodicApp checks + `WRONG_SEQUENCE_ID`, `MISSING_INPUT`, `WRONG_INPUT_FREQUENCY`, `INPUTS_NOT_SYNCHRONIZED`

### Supervisor
A centralized component is responsible for overall hardware resource consumption and fallback management. More precisely, through the system resource monitor, the supervisor node continuously checks for critical hardware usage and temperature in order to prevent major failures.