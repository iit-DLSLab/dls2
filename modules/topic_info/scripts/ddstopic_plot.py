#!/usr/bin/env python3
"""ddstopic_plot: plot the rate logged by `ddstopic hz <topic> --csv <file>`.

ddstopic writes one row per received message with its time and the period from
the previous one. For each CSV (one per topic) this script prints mean,
standard deviation and min/max of the rate, and plots the period of every
message in ms together with the period averaged over the last N messages and the
mean, in ms and Hz.

With `ddstopic hz --stamp` the CSV also has the period between the publisher
timestamps, the latency from the publisher timestamp to the reception and the
sequence_ids lost before each message: these are plotted below the received
period, the publisher period and the latency in the same way.

    ddstopic_plot [-n 50] [-o plot.png] <file.csv> [file.csv...]

Example:
    ddstopic hz imu --csv imu.csv --stamp        # stop with CTRL+C
    ddstopic_plot imu.csv joints.csv
"""

import argparse
import csv
import itertools
import statistics
import sys
from pathlib import Path


def parse_args():
    parser = argparse.ArgumentParser(description="Plot the rate logged by `ddstopic hz --csv`.")
    parser.add_argument("files", nargs="+", type=Path, help="CSV files written by ddstopic")
    parser.add_argument("-n", "--rolling", type=int, default=50, help="messages in the rolling period (default 50)")
    parser.add_argument("-o", "--output", help="also save the plot to this file")
    args = parser.parse_args()
    if args.rolling < 1:
        parser.error("rolling must be at least 1")
    return args


def load(path):
    """Return (times, periods, pub_periods, lost, latencies) of the messages logged in path.

    pub_periods, lost and latencies are None if the CSV does not have them (no --stamp).
    """
    times, periods, pub_periods, lost, latencies = [], [], [], [], []
    with path.open(newline="") as f:
        reader = csv.DictReader(f)
        fields = reader.fieldnames or []
        stamped = "pub_period_s" in fields
        has_latency = "latency_s" in fields
        for row in reader:
            times.append(float(row["time_s"]))
            periods.append(float(row["period_s"]))
            if stamped:
                pub_periods.append(float(row["pub_period_s"]))
                lost.append(int(row["lost"]))
            if has_latency:
                latencies.append(float(row["latency_s"]))
    return (times, periods, pub_periods if stamped else None, lost if stamped else None,
            latencies if has_latency else None)


def rolling_period(times, periods, n):
    """Return (time, period) averaged over the last n periods, for each message from the n-th on."""
    points = []
    window = sum(periods[:n])
    for i in range(n - 1, len(periods)):
        if i >= n:
            window += periods[i] - periods[i - n]
        points.append((times[i], window / n))
    return points


def plot_period(ax, title, times, periods, n, ylabel="period [ms]", hz=True):
    """Plot the period of every message, over the last n messages and its mean, in ms."""
    mean = sum(periods) / len(periods)
    ax.plot(times, [1e3 * p for p in periods], linewidth=1.0, alpha=1.0, color="tab:green", label="per message")
    if rolling := rolling_period(times, periods, n):
        x, y = zip(*rolling)
        ax.plot(x, [1e3 * p for p in y], linewidth=2.2, color="tab:blue", label=f"last {n} messages")
    rate = f" ({1.0 / mean:.2f} Hz)" if hz and mean > 0 else ""
    ax.axhline(1e3 * mean, color="tab:red", linestyle="--", linewidth=2.0, label=f"mean {1e3 * mean:.3f} ms{rate}")
    ax.set_title(title)
    ax.set_ylabel(ylabel)
    ax.ticklabel_format(axis="y", useOffset=False)
    # A steady period would otherwise zoom the axis on rounding noise
    low, high = ax.get_ylim()
    ax.set_ylim(min(low, 0.99e3 * mean), max(high, 1.01e3 * mean))
    ax.grid(True, alpha=0.3)
    ax.legend(loc="upper right")


def print_stats(data):
    print(f"{'file':<40} {'msgs':>8} {'mean [Hz]':>12} {'std [Hz]':>12} {'min [Hz]':>12} {'max [Hz]':>12}"
          f" {'pub [Hz]':>12} {'lost':>8} {'latency [ms]':>12}")
    for name, (times, periods, pub_periods, lost, latencies) in data.items():
        rates = [1.0 / p for p in periods if p > 0]
        if len(rates) < 2:
            print(f"{name:<40} {len(periods):>8} not enough messages logged")
            continue
        mean = len(periods) / sum(periods)
        line = (f"{name:<40} {len(periods):>8} {mean:>12.3f} {statistics.stdev(rates):>12.3f}"
                f" {min(rates):>12.3f} {max(rates):>12.3f}")
        if pub_periods is not None and sum(pub_periods) > 0:
            line += f" {len(pub_periods) / sum(pub_periods):>12.3f} {sum(lost):>8}"
        if latencies:
            line += f" {1e3 * sum(latencies) / len(latencies):>12.3f}"
        print(line)


def plot(data, n, output):
    try:
        import matplotlib.pyplot as plt
    except ImportError:
        sys.exit("matplotlib not available: install it with `apt install python3-matplotlib`")

    data = {name: d for name, d in data.items() if len(d[1]) > 1}
    if not data:
        sys.exit("nothing to plot")
    rows = sum(1 + (pub_periods is not None) * 2 + (latencies is not None)
               for _, _, pub_periods, _, latencies in data.values())
    fig, axes = plt.subplots(rows, 1, sharex=True, squeeze=False, figsize=(10, 3 * rows))
    axes[-1, 0].set_xlabel("time [s]")
    axes = iter(axes[:, 0])
    for name, (times, periods, pub_periods, lost, latencies) in data.items():
        plot_period(next(axes), f"{name}: received", times, periods, n)
        if pub_periods is None:
            continue

        # Jitter here comes from the publisher, the rest of the received one from the transport
        plot_period(next(axes), f"{name}: publisher timestamps", times, pub_periods, n)
        if latencies is not None:
            plot_period(next(axes), f"{name}: latency, publisher timestamp to reception", times, latencies, n,
                        ylabel="latency [ms]", hz=False)

        ax = next(axes)
        ax.step(times, list(itertools.accumulate(lost)), where="post", linewidth=2.2, color="tab:red", label=f"total {sum(lost)}")
        drops = [(t, count) for t, count in zip(times, lost) if count > 0]
        if drops:
            ax.plot(*zip(*drops), "x", markersize=9, markeredgewidth=2, color="tab:orange", label="lost before the message")
        ax.set_title(f"{name}: lost sequence_ids")
        ax.set_ylabel("lost messages")
        ax.set_ylim(0, max(1, 1.1 * sum(lost)))
        ax.grid(True, alpha=0.3)
        ax.legend(loc="upper left")
    fig.tight_layout()

    if output:
        fig.savefig(output, dpi=120)
        print(f"Plot saved to {output}")
    plt.show()


def main():
    args = parse_args()
    data = {path.stem: load(path) for path in args.files}
    print_stats(data)
    plot(data, args.rolling, args.output)


if __name__ == "__main__":
    main()
