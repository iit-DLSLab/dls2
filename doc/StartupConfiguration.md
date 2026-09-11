# Startup configuration reference

This page describes all entries in [default_startup.yaml](../modules/main/src/default_startup.yaml). Run the default configuration with `dls --startup`, or use a custom file with:

```sh
dls --startup=/path/to/startup.yaml
```

The defaults below are the values in `default_startup.yaml`. 

## Layers and services

| Entry | Type | Default | Description |
| --- | --- | --- | --- |
| `layers` | List of strings | `[hardware, estimation, log]` | Layers to launch. Use `hardware`, `estimation`, `control`, `console`, or `log`. Use `run_supervisor` to control whether the supervisor is launched. Include the layers needed to load your applications, unless they are already running. |
| `run_servers` | Boolean | `true` | Launch the DDS discovery servers. Omitted means `true`; `false` skips launching them and assumes servers are already running. |
| `run_supervisor` | Boolean | `true` | Launch the supervisor. Omitted means `true`; `false` skips launching it. Applications loaded through the supervisor require it to be available. |

## Applications to load

Each entry below is a list of application names, for example `hardwares: [gazebo_sim]`. All default to `[]`, and omitting a list loads no applications of that type. Names are passed to the corresponding loading command.

| Entry | Description | Loading layer / component | Command |
| --- | --- | --- | --- |
| `hardwares` | Hardware interface applications, including simulation hardware. | `hardware` | `loadHardware` |
| `xeno_hardwares` | Xenomai hardware interface applications. | `hardware` | `loadXenoHardware` |
| `controllers` | Controller applications. | `control` | `loadControl` |
| `python_controllers` | Python controller applications. | `control` | `loadControl` |
| `motion_generators` | Motion generator applications. | `control` | `loadControl` |
| `estimators` | Estimator applications. | `estimation` | `loadEstimation` |
| `python_periodic_apps` | Python periodic applications. | Supervisor | `loadPythonPeriodicApp` |
| `generic_app_plugins` | Generic application plugins. | Supervisor | `loadAppPlugin` |
| `generic_periodic_app_plugins` | Generic periodic application plugins. | Supervisor | `loadAppPlugin` |

Loading an application brings it to the `idle` state. Use `activate` to request activation during startup.

## Robot and simulation

| Entry | Type | Default | Description |
| --- | --- | --- | --- |
| `robot_name` | String | `""` | Robot name used by the control layer and passed to the simulation's `loadModel` command. Set it to the robot you want to use. |
| `robot_spawning_height` | Number | `0.366` | Robot spawning height in metres, passed to `loadModel`. If omitted from a custom file, the startup code uses **1.0 m**, rather than the value in the default file. |
| `load_model` | Boolean | `true` | Request `loadModel` from loaded applications that expose it, using `robot_name` and `robot_spawning_height`. Omitted means `true`; set to `false` to skip automatic model loading. |

## Activation

| Entry | Type | Default | Description |
| --- | --- | --- | --- |
| `activate` | List of strings | `[]` | Names of applications to activate after they reach `idle`. Each name must match an application loaded through one of the application lists above. An empty list leaves applications idle. |

Return to [Getting started](../../docs/GettingStarted.md#startup-routine) for the startup procedure.
