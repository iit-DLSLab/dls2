#!/usr/bin/env bash

set -e
echo "Setup ROS2 environment for DLS2"

ros_distro="${DLS_ROS_DISTRO:-jazzy}"
ros_prefix="/opt/ros/${ros_distro}"

echo "Sourcing ROS 2 setup file from: ${ros_prefix}"
if [ -r "${ros_prefix}/setup.bash" ]; then
	source "${ros_prefix}/setup.bash"
elif [ -r "${ros_prefix}/setup.sh" ]; then
	source "${ros_prefix}/setup.sh"
else
	echo "Cannot read ROS 2 setup file in: ${ros_prefix}" >&2
	exit 1
fi

if ! command -v ros2 >/dev/null 2>&1; then
	echo "ros2 command not found after sourcing ${ros_prefix}/setup.*" >&2
	echo "Check that ROS 2 ${ros_distro} is installed and provides ${ros_prefix}/bin/ros2." >&2
	exit 1
fi

servers_path="${DLS_SERVERS_PATH:-/usr/include/dls2/util/messaging/servers.yaml}"

echo "Reading DLS servers from: ${servers_path}"

if [ ! -r "${servers_path}" ]; then
	echo "Cannot read DLS servers file: ${servers_path}" >&2
	exit 1
fi

ros_discovery_server="$(
	awk '
		{
			line = $0
			sub(/[[:space:]]*#.*/, "", line)
			if (line ~ /^[[:space:]]*ip:[[:space:]]*/) {
				ip = line
				sub(/^[[:space:]]*ip:[[:space:]]*/, "", ip)
				gsub(/["'\''[:space:]]/, "", ip)
			}
			if (line ~ /^[[:space:]]*port:[[:space:]]*/ && ip != "") {
				port = line
				sub(/^[[:space:]]*port:[[:space:]]*/, "", port)
				gsub(/["'\''[:space:]]/, "", port)
				if (server != "") {
					server = server ";"
				}
				server = server ip ":" port
				ip = ""
			}
		}
		END {
			print server
		}
	' "${servers_path}"
)"

if [ -z "${ros_discovery_server}" ]; then
	echo "No active discovery servers found in: ${servers_path}" >&2
	exit 1
fi

echo "exporting ROS_DISCOVERY_SERVER=${ros_discovery_server}"
export ROS_DISCOVERY_SERVER="${ros_discovery_server}"

echo "exporting ROS_SUPER_CLIENT=TRUE"
export ROS_SUPER_CLIENT=TRUE

echo "Restarting ROS 2 daemon..."
ros2 daemon stop
ros2 daemon start
