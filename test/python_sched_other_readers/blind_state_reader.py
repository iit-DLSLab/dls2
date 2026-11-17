#!/usr/bin/env python3
"""Lightweight rt/blind_state subscriber.

A single Fast DDS participant (dls2_py Reader) polled by a periodic loop.
Scheduling policy and period are read from a dls2 scheduler yaml
(sched_policy, period [ms], sched_priority) and applied before any DDS
thread is created, so every thread of the process inherits them.
It is not a dls2 PeriodicApp: no state machine and no activation needed.

The DDS participant is a discovery CLIENT of one server of the dls2 servers
yaml (DLS_SERVERS_PATH, 'signals' entry by default), UDPv4 only. Traffic is
separated by discovery server, the DDS domain id is left to its default.
"""

from __future__ import annotations

import argparse
import os
import signal
import sys
import tempfile
import time

import yaml


# same lookup as dls::DDSParticipant
DEFAULT_SERVERS_PATH = "/opt/dls2/include/dls2/util/messaging/servers.yaml"

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--id", required=True, help="node name, used in prints")
parser.add_argument("--sched-config", required=True, help="dls2 scheduler yaml")
parser.add_argument(
    "--servers",
    default=os.environ.get("DLS_SERVERS_PATH", DEFAULT_SERVERS_PATH),
    help="dls2 discovery servers yaml (default: $DLS_SERVERS_PATH or %(default)s)",
)
parser.add_argument("--server", default="signals", help="discovery server entry of the servers yaml")
parser.add_argument("--topic", default="rt/blind_state", help="topic to subscribe to")
parser.add_argument("--report-period", type=float, default=1.0, help="stats print period [s], 0 disables")
parser.add_argument("--status-file", default=None, help="file periodically rewritten with '<samples> <seq> <stamp>'")
args = parser.parse_args()

STATUS_PERIOD_S = 0.5


def apply_scheduler(config_file: str) -> float:
    """Set the policy of this process from the yaml, return the period in seconds."""
    with open(config_file) as stream:
        config = yaml.safe_load(stream)

    # same default as dls2 SchedulerUtils
    policy = config.get("sched_policy", "SCHED_DEADLINE")
    if policy == "SCHED_OTHER":
        os.sched_setscheduler(0, os.SCHED_OTHER, os.sched_param(0))
    elif policy == "SCHED_FIFO":
        os.sched_setscheduler(0, os.SCHED_FIFO, os.sched_param(int(config.get("sched_priority", 45))))
    else:
        sys.exit(f"[{args.id}] unsupported sched_policy '{policy}' in {config_file}: use SCHED_OTHER or SCHED_FIFO")

    period_s = float(config["period"]) / 1000.0
    if period_s <= 0.0:
        sys.exit(f"[{args.id}] period must be positive in {config_file}")
    return period_s


def resolve_server(servers_file: str, name: str) -> tuple[str, int]:
    with open(servers_file) as stream:
        servers = yaml.safe_load(stream)
    if name not in servers:
        sys.exit(f"[{args.id}] no '{name}' server in {servers_file}")
    return str(servers[name]["ip"]), int(servers[name]["port"])


def write_participant_profile(server_ip: str, server_port: int) -> str:
    """Write the profile dls2_py Reader loads, return its path."""
    profile = f"""<?xml version="1.0" encoding="UTF-8" ?>
<profiles xmlns="http://www.eprosima.com">
    <transport_descriptors>
        <transport_descriptor>
            <transport_id>udp_only</transport_id>
            <type>UDPv4</type>
        </transport_descriptor>
    </transport_descriptors>
    <participant profile_name="{PROFILE_NAME}">
        <rtps>
            <name>{args.id}</name>
            <userTransports>
                <transport_id>udp_only</transport_id>
            </userTransports>
            <useBuiltinTransports>false</useBuiltinTransports>
            <builtin>
                <!-- participant ids (port pairs) probed before failing, as dls::DDSParticipant (default 100) -->
                <mutation_tries>250</mutation_tries>
                <discovery_config>
                    <discoveryProtocol>CLIENT</discoveryProtocol>
                    <discoveryServersList>
                        <locator>
                            <udpv4>
                                <address>{server_ip}</address>
                                <port>{server_port}</port>
                            </udpv4>
                        </locator>
                    </discoveryServersList>
                </discovery_config>
            </builtin>
        </rtps>
    </participant>
</profiles>
"""
    fd, path = tempfile.mkstemp(prefix=f"{args.id}_", suffix=".xml")
    with os.fdopen(fd, "w") as stream:
        stream.write(profile)
    return path


# dls2_py Reader loads the profile "disc_server_client_domain_<its domain arg>":
# the arg only names the profile, it does not set the DDS domain.
PROFILE_KEY = "blind_state_reader"
PROFILE_NAME = f"disc_server_client_domain_{PROFILE_KEY}"

period_s = apply_scheduler(args.sched_config)
server_ip, server_port = resolve_server(args.servers, args.server)

types_path = os.environ.get("BLIND_STATE_READER_TYPES_PATH")
if types_path and types_path not in sys.path:
    sys.path.insert(0, types_path)

import BlindState  # noqa: E402

from dls2_py.reader import Reader  # noqa: E402


class BlindStateReaderNode:
    def __init__(self) -> None:
        profile_path = write_participant_profile(server_ip, server_port)
        os.environ["DLS_DDS_PARTICIPANT_CONFIG"] = profile_path
        try:
            self.reader = Reader(
                PROFILE_KEY,
                BlindState.BlindStatePubSubType(),
                BlindState.BlindState(),
                args.topic,
            )
        finally:
            os.remove(profile_path)
        self.should_stop = False

        self._run_count = 0
        self._last_report_time = time.monotonic()
        self._last_report_samples = 0
        self._last_report_runs = 0
        self._last_status_time = 0.0

    def spin(self) -> None:
        # Keep the phase and skip expired slots after an overrun, like
        # SchedulerUtils::executeEndLoopTask does for SCHED_OTHER/SCHED_FIFO.
        next_time = time.monotonic()
        while not self.should_stop:
            self.run()
            next_time += period_s
            now = time.monotonic()
            if now > next_time:
                next_time += period_s * ((now - next_time) // period_s + 1)
            time.sleep(max(0.0, next_time - time.monotonic()))
        if args.status_file:
            self._write_status()

    def run(self) -> None:
        self._run_count += 1

        now = time.monotonic()
        if args.status_file and now - self._last_status_time >= STATUS_PERIOD_S:
            self._write_status()
            self._last_status_time = now

        if args.report_period <= 0.0:
            return

        elapsed = now - self._last_report_time
        if elapsed < args.report_period:
            return

        # The listener takes every sample from the DDS callback thread, so its
        # counter is the real receive count, independent of the loop rate.
        samples = self.reader.listener.sample_count
        msg = self.reader.getData()
        print(
            f"[{args.id}] pid={os.getpid()} "
            f"rx={(samples - self._last_report_samples) / elapsed:.1f}Hz "
            f"loop={(self._run_count - self._last_report_runs) / elapsed:.1f}Hz "
            f"total={samples} seq={msg.sequence_id()} stamp={msg.timestamp():.6f}",
            flush=True,
        )
        self._last_report_time = now
        self._last_report_samples = samples
        self._last_report_runs = self._run_count

    def _write_status(self) -> None:
        # read by the launcher summary: replace atomically so it never sees a partial line
        msg = self.reader.getData()
        tmp_path = args.status_file + ".tmp"
        with open(tmp_path, "w") as status:
            status.write(
                f"{self.reader.listener.sample_count} "
                f"{msg.sequence_id()} {msg.timestamp():.6f}\n"
            )
        os.replace(tmp_path, args.status_file)


def main() -> None:
    node = BlindStateReaderNode()

    def stop(_signum, _frame):
        node.should_stop = True

    signal.signal(signal.SIGINT, stop)
    signal.signal(signal.SIGTERM, stop)

    print(
        f"[{args.id}] subscribed to {args.topic} ({args.server} server {server_ip}:{server_port}), "
        f"loop period {period_s * 1000:.0f} ms",
        flush=True,
    )
    node.spin()
    print(f"[{args.id}] stopping, received {node.reader.listener.sample_count} samples", flush=True)


if __name__ == "__main__":
    main()
