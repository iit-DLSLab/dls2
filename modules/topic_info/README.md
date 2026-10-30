# ddstopic

Inspect DLS2 DDS topics. Connects as a Discovery super client, waits 2 s for
discovery, then runs the command. The default domain is 3.

```bash
ddstopic list [domain]                          # discovered topics
ddstopic participants [domain]                  # discovered participants
ddstopic hz <topic> [domain] [--csv <file>] [--time <seconds>]   # rate of a topic
```

`hz` prints once per second the rate, standard deviation, max and min period
over the last 1000 messages. It runs until CTRL+C, or for `--time` seconds.
Use the DDS topic name as shown by `ddstopic list` (e.g. `blind_state`, no
leading `/`).

## Logging and plotting the rate

`--csv <file>` logs every received message as `time_s,period_s,rate_hz`
(time from the first message). The rows are written once per second.

`ddstopic_plot` prints mean, std dev, min and max rate for each CSV and plots
the rate per message and over the last `-n` messages (default 50) in a window;
`-o` also saves it to a file. Needs `matplotlib` with a GUI backend.

```bash
ddstopic hz imu --csv imu.csv --time 30
ddstopic_plot imu.csv joints.csv                # [-n 50] [-o plot.png]
```
