#!/usr/bin/env python3
"""ddstopic_plot: plot the rate logged by `ddstopic hz <topic> --csv <file>`.

ddstopic writes one row per received message with its time and the period from
the previous one. For each CSV (one per topic) this script prints mean,
standard deviation and min/max of the rate, and plots the rate of every message
together with the rate averaged over the last N messages.

    ddstopic_plot [-n 50] [-o plot.png] <file.csv> [file.csv...]

Example:
    ddstopic hz imu --csv imu.csv        # stop with CTRL+C
    ddstopic_plot imu.csv joints.csv
"""

import argparse
import csv
import statistics
import sys
from pathlib import Path


def parse_args():
    parser = argparse.ArgumentParser(description="Plot the rate logged by `ddstopic hz --csv`.")
    parser.add_argument("files", nargs="+", type=Path, help="CSV files written by ddstopic")
    parser.add_argument("-n", "--rolling", type=int, default=50, help="messages in the rolling rate (default 50)")
    parser.add_argument("-o", "--output", help="also save the plot to this file")
    args = parser.parse_args()
    if args.rolling < 1:
        parser.error("rolling must be at least 1")
    return args


def load(path):
    """Return (times, periods) of the messages logged in path."""
    times, periods = [], []
    with path.open(newline="") as f:
        for row in csv.DictReader(f):
            times.append(float(row["time_s"]))
            periods.append(float(row["period_s"]))
    return times, periods


def rolling_hz(times, periods, n):
    """Return (time, rate) averaged over the last n periods, for each message from the n-th on."""
    points = []
    window = sum(periods[:n])
    for i in range(n - 1, len(periods)):
        if i >= n:
            window += periods[i] - periods[i - n]
        if window > 0:
            points.append((times[i], n / window))
    return points


def print_stats(data):
    print(f"{'file':<40} {'msgs':>8} {'mean [Hz]':>12} {'std [Hz]':>12} {'min [Hz]':>12} {'max [Hz]':>12}")
    for name, (times, periods) in data.items():
        rates = [1.0 / p for p in periods if p > 0]
        if len(rates) < 2:
            print(f"{name:<40} {len(periods):>8} not enough messages logged")
            continue
        mean = len(periods) / sum(periods)
        print(f"{name:<40} {len(periods):>8} {mean:>12.3f} {statistics.stdev(rates):>12.3f}"
              f" {min(rates):>12.3f} {max(rates):>12.3f}")


def plot(data, n, output):
    try:
        import matplotlib.pyplot as plt
    except ImportError:
        sys.exit("matplotlib not available: install it with `apt install python3-matplotlib`")

    data = {name: d for name, d in data.items() if len(d[1]) > 1}
    if not data:
        sys.exit("nothing to plot")
    fig, axes = plt.subplots(len(data), 1, sharex=True, squeeze=False, figsize=(10, 3 * len(data)))
    for ax, (name, (times, periods)) in zip(axes[:, 0], data.items()):
        mean = len(periods) / sum(periods)
        x, y = zip(*((t, 1.0 / p) for t, p in zip(times, periods) if p > 0))
        ax.plot(x, y, linewidth=0.5, alpha=0.3, color="tab:gray", label="per message")
        if rolling := rolling_hz(times, periods, n):
            x, y = zip(*rolling)
            ax.plot(x, y, linewidth=1.2, color="tab:blue", label=f"last {n} messages")
        ax.axhline(mean, color="tab:red", linestyle="--", label=f"mean {mean:.2f} Hz")
        ax.set_title(name)
        ax.set_ylabel("rate [Hz]")
        ax.ticklabel_format(axis="y", useOffset=False)
        # A steady rate would otherwise zoom the axis on rounding noise
        low, high = ax.get_ylim()
        ax.set_ylim(min(low, 0.99 * mean), max(high, 1.01 * mean))
        ax.grid(True, alpha=0.3)
        ax.legend(loc="upper right")
    axes[-1, 0].set_xlabel("time [s]")
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
