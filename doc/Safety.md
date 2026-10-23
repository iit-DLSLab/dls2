Each app has a `RobustEventNotifier` entity acting as asynchronous event notifier and a DDSWriter acting as a periodic status notifier.
Both of them share the same goal: inform the supervisor about the current node situation in terms of logical health and resource consumption.

The safety layer is composed by a set of sanity checks that are performed in a separate monitoring thread for each object extending App class. The types of checks are strictly related to the functionalities of the node, meaning that a node extending from PeriodicAppPlugin will have a more complex set of checks rather than a node extending the App class only. Concerning the order, the sequence of checks reflects the object hierarchy, meaning that relevant concepts more related to the App class (e.g. internal state machine status) are firstly checked while quantities associated with PeriodicAppPlugin features (e.g. expected input frequency) are lastly checked. Each check is associated with an asynchronous event that is triggered as soon as the failure is detected and relevant data related to it are periodically published by the status notifier. All checks can be enabled/disabled or tuned modifying the `safety_layer.yaml` parameter file accordingly in order to shape the desired behavior of the safety layer (e.g. thresholds tuning).
This yaml file location can be overridden by the settable `DLS_SAFETY_LAYER_PATH` env variable.
An additional check on `RobustEventNotifier` avoids spamming the same type of event (msg is published again after only after a tunable cool-down period).
An example of how to trigger async events can be found in [safety_plugin.cpp](../../modules/supervisor/test/src/safety_plugin.cpp).

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
| `VIRTUAL_ENDSTOP_ENFORCING` | The HAL virtual endstop is braking at least one joint (predicted position limit violation) and overrides its high-level command; reports position, velocity, limit side, anchor and virtual torque per joint | `virtual_joint_limit_min_velocity`, `virtual_joint_limit_torque_scale` | High-level controller driving a joint toward its position limit too fast
| `MOTOR_FAULT` | At least one motor reports a `fault_info` different from the configured no-fault string; reports the `fault_info` per joint. An empty `fault_info` (not reported, e.g. in simulation) is not a fault | `robot_interface.safety.motor_no_fault_info` (empty or missing disables the check) | Motor drive fault
| `MOTOR_NOT_OPERATIONAL` | At least one motor status word does not have all the operational bits set; reports the status word per joint. If the low-level interface fails to start with such motors, the HAL publishes it for a while before going down | `MotorStatusWords::operational_mask`, set by the low-level interface (0 disables the check), `robot_interface.safety.startup_failure_notify_duration` | Motor drive not powered, faulted or not enabled
| `NODES_NOT_DISCOVERED` | At least one node expected by the low-level interface (e.g. an EtherCAT slave) was not discovered when it started; reports the ids of the discovered nodes. If the low-level interface fails to start with such nodes, the HAL publishes it for a while before going down | `robot_interface.safety.startup_failure_notify_duration` | Node not powered or disconnected from the bus, master not running
| `NODES_DETACHED` | The low-level interface detected that a node was detached while running (e.g. EtherCAT slaves data no longer updated); latched until the HAL restarts, it switches the safety state to `EMERGENCY_STOP` (zero torque), see [Nodes detached while running](#nodes-detached-while-running) | Low-level interface specific (pegasus_hal: `ecat_data_timeout`) | Node disconnected from the bus or powered off while running

A detailed list of all checks is reported in the following. The order reflects their implementation. As soon as one of them is triggered an event is published by the event notifier. 

* App: `WRONG_PROCESS_STATE`
* Layer: App checks + `PROCESS_DIED` (in a different thread)
* PeriodicApp: App checks + `WRONG_PROCESS_FREQUENCY`, `CPU_USAGE_TOO_HIGH`, `MEM_USAGE_TOO_HIGH`
* PeriodicAppPlugin: PeriodicApp checks + `WRONG_SEQUENCE_ID`, `MISSING_INPUT`, `WRONG_INPUT_FREQUENCY`, `INPUTS_NOT_SYNCHRONIZED`

If you want to get all messages from the log layer (safety layer ones included) in your terminal be sure to set DLS_VERBOSE=1 env variable.

### Nodes detached while running
When a node of the low-level bus is disconnected or powered off while the HAL is running, the HAL raises the `nodes_detached` anomaly, publishes `NODES_DETACHED` and switches the safety state to `EMERGENCY_STOP`, which commands zero torque (and zero position/velocity) to the joints. The anomaly is latched: the HAL must be restarted once the node is reconnected.

Zero torque is used instead of the damping recovery on purpose: after a detach the measured state is no longer updated, so damping (torque = -gain * measured velocity) would keep applying a constant torque computed from the last, frozen velocities to the motors still on the bus.

In pegasus_hal the detach is detected by `EtherCatTransport::check()`: when a slave is detached, the EtherCAT master detects the working counter mismatch and stops updating the data of all the slaves. The measured data of every motor, IMU and power board change at every sample on a working bus (at most 2 consecutive identical samples, i.e. 2 ms, measured over 3 min), so a slave whose data do not change for `ecat_data_timeout` (`low_level_config.yaml`, 0.1 s) is considered detached. Since the data of all the slaves freeze together, the detached node cannot be identified; the EtherCAT master logs the working counter warning. A frozen client (no data received from the master) is detected the same way.

### Supervisor
A centralized component is responsible for overall hardware resource consumption and fallback management. More precisely, through the system resource monitor, the supervisor node continuously checks for critical hardware usage and temperature in order to prevent major failures.