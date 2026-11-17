# ddstopic

Inspect DLS2 DDS topics. Connects as a Discovery super client, waits 2 s for
discovery, then runs the command. The default domain is 3.

```bash
ddstopic list [domain]                          # discovered topics
ddstopic participants [domain]                  # discovered participants
ddstopic hz <topic> [domain] [--csv <file>] [--time <seconds>] [--stamp]   # rate of a topic
```

`hz` prints once per second the rate, standard deviation, max and min period
over the last 1000 messages. It runs until CTRL+C, or for `--time` seconds.
Use the DDS topic name as shown by `ddstopic list` (e.g. `blind_state`, no
leading `/`).

With `--stamp`, `hz` also prints the rate and standard deviation from the
publisher timestamps and the total number of lost messages. The message must
have `unsigned long sequence_id` and a timestamp in nanoseconds, either in a
`DlsHeader header` (`unsigned long long timestamp`) or at the top level
(`double timestamp`), as DLS2 messages do, otherwise `hz` stops with an error.
Messages are lost when the `sequence_id` increases by more than 1 from the
previous received one, modulo `dls::MAX_SEQUENCE_ID` (1000) where `dls::Writer`
wraps it; a publisher restart counts as lost the ids it skipped.

## Logging and plotting the rate

`--csv <file>` logs every received message as `time_s,period_s,rate_hz`
(time from the first message). With `--stamp` there are also
`pub_period_s` (period between publisher timestamps), `lost` (`sequence_id`s
skipped before the message), `sequence_id` and `latency_s` (from the publisher
timestamp to the reception on the system clock: publisher and ddstopic need the
same clock, so the same machine or synchronized clocks). The rows are written once per
second.

`ddstopic_plot` prints mean, std dev, min and max rate for each CSV and plots
the period in ms per message and over the last `-n` messages (default 50), with
its mean in ms and Hz, in a window; `-o` also saves it to a file. Needs
`matplotlib` with a GUI backend. If the CSV has publisher timestamps it also
prints the publisher rate and the lost messages, and plots the period between
publisher timestamps and the latency in the same way, and the cumulative number
of lost messages.

```bash
ddstopic hz imu --csv imu.csv --time 30 --stamp
ddstopic_plot imu.csv joints.csv                # [-n 50] [-o plot.png]
```
