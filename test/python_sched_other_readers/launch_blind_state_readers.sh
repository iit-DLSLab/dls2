#!/bin/bash
# Launch N lightweight python subscribers of rt/blind_state, scheduled with the
# policy and period of a dls2 scheduler yaml (SCHED_OTHER by default).
# Ctrl-C stops all of them.

# Sourced, every 'exit' below (and the cleanup trap) would close the caller's
# shell, i.e. the docker terminal.
if [[ "${BASH_SOURCE[0]}" != "$0" ]]; then
    echo "run this script (./$(basename -- "${BASH_SOURCE[0]}") ...), do not source it" >&2
    return 1
fi

set -u

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
DLS2_ROOT=$(cd -- "$SCRIPT_DIR/../.." && pwd)
WORKSPACE_ROOT=$(cd -- "$DLS2_ROOT/../.." && pwd)
NODE_SCRIPT="$SCRIPT_DIR/blind_state_reader.py"

NUM_NODES=""
SCHED_CONFIG="$SCRIPT_DIR/plugin/config/scheduler.yaml"
# same lookup as dls::DDSParticipant
SERVERS_FILE="${DLS_SERVERS_PATH:-/opt/dls2/include/dls2/util/messaging/servers.yaml}"
SERVER_NAME=signals
TOPIC="rt/blind_state"
PREFIX="blind_state_reader"
STAGGER_S=0
REPORT_PERIOD_S=1.0
SUMMARY_PERIOD_S=2
LOG_DIR=""
LOG_TO_TERMINAL=0
DRY_RUN=0

usage() {
    cat <<EOF
Usage: $(basename "$0") -n <num_nodes> [options]

  -n <num>      number of python nodes to launch (required)
  -c <yaml>     scheduler config shared by all nodes
                (default: $SCHED_CONFIG)
  -S <yaml>     dls2 discovery servers yaml, e.g. servers_config/real_robot_servers.yaml
                (default: \$DLS_SERVERS_PATH or $SERVERS_FILE)
  -k <name>     discovery server entry of the servers yaml (default: $SERVER_NAME)
  -t <topic>    topic name (default: $TOPIC)
  -x <prefix>   node name prefix, names are <prefix>_<i> (default: $PREFIX)
  -s <sec>      delay between two launches (default: $STAGGER_S)
  -r <sec>      per-node stats print period, 0 disables (default: $REPORT_PERIOD_S)
  -w <sec>      integer period of the "which reader received data" summary,
                0 disables (default: $SUMMARY_PERIOD_S)
  -l <dir>      log directory (default: a new temp dir)
  -f            print node output on this terminal instead of log files
  -D            dry run: print the commands and exit
  -h            show this help

Environment overrides:
  DLS2_PYTHON                     python interpreter
  BLIND_STATE_READER_TYPES_PATH   directory containing BlindState.py
EOF
}

while getopts ":n:c:S:k:t:x:s:r:w:l:fDh" opt; do
    case "$opt" in
        n) NUM_NODES=$OPTARG ;;
        c) SCHED_CONFIG=$OPTARG ;;
        S) SERVERS_FILE=$OPTARG ;;
        k) SERVER_NAME=$OPTARG ;;
        t) TOPIC=$OPTARG ;;
        x) PREFIX=$OPTARG ;;
        s) STAGGER_S=$OPTARG ;;
        r) REPORT_PERIOD_S=$OPTARG ;;
        w) SUMMARY_PERIOD_S=$OPTARG ;;
        l) LOG_DIR=$OPTARG ;;
        f) LOG_TO_TERMINAL=1 ;;
        D) DRY_RUN=1 ;;
        h) usage; exit 0 ;;
        :) echo "option -$OPTARG requires an argument" >&2; usage >&2; exit 2 ;;
        *) echo "unknown option -$OPTARG" >&2; usage >&2; exit 2 ;;
    esac
done

if ! [[ "$NUM_NODES" =~ ^[1-9][0-9]*$ ]]; then
    echo "-n <num_nodes> must be a positive integer" >&2
    usage >&2
    exit 2
fi
if ! [[ "$SUMMARY_PERIOD_S" =~ ^[0-9]+$ ]]; then
    echo "-w <sec> must be a non-negative integer" >&2
    exit 2
fi
if [ ! -f "$SCHED_CONFIG" ]; then
    echo "scheduler config '$SCHED_CONFIG' does not exist" >&2
    exit 2
fi
SCHED_CONFIG=$(cd -- "$(dirname -- "$SCHED_CONFIG")" && pwd)/$(basename -- "$SCHED_CONFIG")
if [ ! -f "$SERVERS_FILE" ]; then
    echo "servers config '$SERVERS_FILE' does not exist (set -S or DLS_SERVERS_PATH)" >&2
    exit 2
fi
SERVERS_FILE=$(cd -- "$(dirname -- "$SERVERS_FILE")" && pwd)/$(basename -- "$SERVERS_FILE")
SCHED_POLICY=$(sed -n 's/^[[:space:]]*sched_policy[[:space:]]*:[[:space:]]*\([A-Z_]*\).*/\1/p' "$SCHED_CONFIG" | head -n 1)
# no sched_policy key means the dls2 default, SCHED_DEADLINE
if [ "${SCHED_POLICY:-SCHED_DEADLINE}" != "SCHED_OTHER" ]; then
    echo "warning: $SCHED_CONFIG sets ${SCHED_POLICY:-SCHED_DEADLINE (default)}, not SCHED_OTHER" >&2
fi
if ! [[ "$PREFIX" =~ ^[A-Za-z_][A-Za-z0-9_]*$ ]]; then
    echo "-x <prefix> must be a valid identifier (it is used in file names)" >&2
    exit 2
fi

prepend_path() {
    local var_name=$1
    local new_path=$2
    [ -d "$new_path" ] || return
    local current_value="${!var_name:-}"
    if [ -z "$current_value" ]; then
        printf -v "$var_name" '%s' "$new_path"
    elif [[ ":$current_value:" != *":$new_path:"* ]]; then
        printf -v "$var_name" '%s:%s' "$new_path" "$current_value"
    fi
    export "$var_name"
}

# ---- python environment ---------------------------------------------------
PYTHON_EXECUTABLE="${DLS2_PYTHON:-}"
if [ -z "$PYTHON_EXECUTABLE" ]; then
    if [ -x /usr/lib/dls2/python/bin/python3 ]; then
        PYTHON_EXECUTABLE=/usr/lib/dls2/python/bin/python3
    else
        PYTHON_EXECUTABLE=$(command -v python3)
    fi
fi
export COLCON_PYTHON_EXECUTABLE="${COLCON_PYTHON_EXECUTABLE:-$PYTHON_EXECUTABLE}"

DLS_SETUP_SCRIPT="$DLS2_ROOT/modules/plugin/python/scripts/setup.sh"
if [ -f "$DLS_SETUP_SCRIPT" ]; then
    export COLCON_TRACE="${COLCON_TRACE:-}"
    export AMENT_TRACE_SETUP_FILES="${AMENT_TRACE_SETUP_FILES:-}"
    # colcon setup files sourced by setup.sh are not nounset-safe
    set +u
    # shellcheck disable=SC1090
    source "$DLS_SETUP_SCRIPT"
    set -u
fi

if [ -z "${BLIND_STATE_READER_TYPES_PATH:-}" ]; then
    for candidate in \
        /usr/lib/python3/dist-packages/dls_messages \
        "$WORKSPACE_ROOT/build/python_dist_packages/dls_messages" \
        "$WORKSPACE_ROOT/build/dls2-barebone/transport_interfaces/transports/fastdds/dls2_msgs/include/dls_messages/dds"; do
        if [ -f "$candidate/BlindState.py" ]; then
            BLIND_STATE_READER_TYPES_PATH="$candidate"
            break
        fi
    done
fi
if [ -z "${BLIND_STATE_READER_TYPES_PATH:-}" ]; then
    echo "BlindState.py not found: set BLIND_STATE_READER_TYPES_PATH" >&2
    exit 1
fi
export BLIND_STATE_READER_TYPES_PATH
prepend_path PYTHONPATH "$BLIND_STATE_READER_TYPES_PATH"

if [ "$LOG_TO_TERMINAL" -eq 0 ] && [ "$DRY_RUN" -eq 0 ]; then
    if [ -z "$LOG_DIR" ]; then
        LOG_DIR=$(mktemp -d -t blind_state_readers_logs.XXXXXX)
    else
        mkdir -p "$LOG_DIR"
    fi
fi

APP_IDS=()
for ((i = 0; i < NUM_NODES; i++)); do
    app_id="${PREFIX}_${i}"
    APP_IDS+=("$app_id")
done

# Each node applies the yaml policy to itself before creating any DDS thread.
LAUNCH_PREFIX=()
# setsid detaches every node from the terminal session and process group:
# terminal signals (Ctrl-C, SIGHUP) and anything a node signals to its own
# group can no longer reach the shell running this script. Ctrl-C is caught
# by the trap below and forwarded to the nodes.
USE_SETSID=0
if command -v setsid >/dev/null 2>&1; then
    LAUNCH_PREFIX+=(setsid)
    USE_SETSID=1
fi

node_cmd() {
    printf '%q ' "${LAUNCH_PREFIX[@]}" "$PYTHON_EXECUTABLE" "$NODE_SCRIPT" \
        --id "$1" --sched-config "$SCHED_CONFIG" --servers "$SERVERS_FILE" \
        --server "$SERVER_NAME" --topic "$TOPIC" --report-period "$REPORT_PERIOD_S"
}

# ip:port of the $SERVER_NAME discovery server, as the nodes will resolve it
server_address() {
    "$PYTHON_EXECUTABLE" - "$SERVERS_FILE" "$SERVER_NAME" <<'EOF' 2>/dev/null || echo "unknown"
import sys, yaml
server = yaml.safe_load(open(sys.argv[1]))[sys.argv[2]]
print(f"{server['ip']}:{server['port']}")
EOF
}

echo "python      : $PYTHON_EXECUTABLE"
echo "types path  : $BLIND_STATE_READER_TYPES_PATH"
echo "scheduler   : $SCHED_CONFIG (${SCHED_POLICY:-SCHED_DEADLINE})"
echo "topic       : $TOPIC"
echo "servers     : $SERVERS_FILE ($SERVER_NAME discovery server $(server_address))"
echo "nodes       : $NUM_NODES (${APP_IDS[0]} .. ${APP_IDS[-1]})"
[ -n "$LOG_DIR" ] && echo "logs        : $LOG_DIR"

if [ "$DRY_RUN" -eq 1 ]; then
    for app_id in "${APP_IDS[@]}"; do
        echo "$(node_cmd "$app_id")"
    done
    exit 0
fi

# ---- launch & cleanup -----------------------------------------------------
PIDS=()
# every node rewrites $STATUS_DIR/<app_id> with '<samples> <seq> <stamp>'
STATUS_DIR=$(mktemp -d -t blind_state_readers_status.XXXXXX)

# With setsid every node leads its own process group: signal the whole group.
signal_node() {
    if [ "$USE_SETSID" -eq 1 ]; then
        kill "-$1" -- "-$2" 2>/dev/null
    else
        kill "-$1" "$2" 2>/dev/null
    fi
}

cleanup() {
    trap - INT TERM EXIT
    echo
    echo "stopping ${#PIDS[@]} nodes..."
    for pid in "${PIDS[@]}"; do
        signal_node TERM "$pid"
    done
    # give the apps time to run their shutdown, then force kill
    for _ in $(seq 50); do
        local alive=0
        for pid in "${PIDS[@]}"; do
            kill -0 "$pid" 2>/dev/null && alive=1
        done
        [ "$alive" -eq 0 ] && break
        sleep 0.1
    done
    for pid in "${PIDS[@]}"; do
        signal_node KILL "$pid"
    done
    wait 2>/dev/null
    rm -rf "$STATUS_DIR"
    exit 0
}

# Reap nodes that died, and dump the log of the first one so the cause of a
# crash is visible without digging in the log dir.
EXITED=()
FIRST_EXIT_SHOWN=0
check_exits() {
    local i rc
    for i in "${!APP_IDS[@]}"; do
        [ -n "${EXITED[$i]:-}" ] && continue
        kill -0 "${PIDS[$i]}" 2>/dev/null && continue
        wait "${PIDS[$i]}" 2>/dev/null
        rc=$?
        EXITED[$i]=$rc
        if [ "$FIRST_EXIT_SHOWN" -eq 0 ]; then
            FIRST_EXIT_SHOWN=1
            echo "${APP_IDS[$i]} exited first (rc=$rc)"
            if [ "$LOG_TO_TERMINAL" -eq 0 ]; then
                echo "---- last lines of $LOG_DIR/${APP_IDS[$i]}.log ----"
                tail -n 20 "$LOG_DIR/${APP_IDS[$i]}.log"
                echo "----"
            fi
        fi
    done
}

# Received samples so far for every node:
#   -  no status yet (node still starting)
#   X  node exited
print_summary() {
    local i total seq stamp cell row="" in_row=0 exited=0
    echo "[summary t=${SECONDS}s] received samples per node (${PREFIX}_<i>), '-' not subscribed yet, 'X' exited"
    for i in "${!APP_IDS[@]}"; do
        total=""
        read -r total seq stamp 2>/dev/null < "$STATUS_DIR/${APP_IDS[$i]}"
        [[ "$total" =~ ^[0-9]+$ ]] || total="-"
        cell=$total
        if [ -n "${EXITED[$i]:-}" ]; then
            cell="$total X"
            exited=$((exited + 1))
        fi
        row+=$(printf '%4d: %-10s' "$i" "$cell")
        in_row=$((in_row + 1))
        if [ "$in_row" -eq 8 ]; then
            echo "$row"
            row=""
            in_row=0
        fi
    done
    [ -n "$row" ] && echo "$row"
    [ "$exited" -gt 0 ] && echo "      $exited/$NUM_NODES nodes exited"
}
trap cleanup INT TERM EXIT

for app_id in "${APP_IDS[@]}"; do
    if [ "$LOG_TO_TERMINAL" -eq 1 ]; then
        "${LAUNCH_PREFIX[@]}" "$PYTHON_EXECUTABLE" "$NODE_SCRIPT" \
            --id "$app_id" --sched-config "$SCHED_CONFIG" --servers "$SERVERS_FILE" \
            --server "$SERVER_NAME" --topic "$TOPIC" \
            --report-period "$REPORT_PERIOD_S" --status-file "$STATUS_DIR/$app_id" \
            < /dev/null &
    else
        "${LAUNCH_PREFIX[@]}" "$PYTHON_EXECUTABLE" "$NODE_SCRIPT" \
            --id "$app_id" --sched-config "$SCHED_CONFIG" --servers "$SERVERS_FILE" \
            --server "$SERVER_NAME" --topic "$TOPIC" \
            --report-period "$REPORT_PERIOD_S" --status-file "$STATUS_DIR/$app_id" \
            < /dev/null > "$LOG_DIR/$app_id.log" 2>&1 &
    fi
    PIDS+=("$!")
    echo "started $app_id (pid $!)"
    [ "$STAGGER_S" != "0" ] && sleep "$STAGGER_S"
done

[ "$LOG_TO_TERMINAL" -eq 0 ] && echo "follow with: tail -f $LOG_DIR/*.log"
echo "check policy with: ps -L -o pid,tid,cls,rtprio,comm -p $(IFS=,; echo "${PIDS[*]}")"
echo "Ctrl-C to stop all nodes"

# return when all nodes exited (or on Ctrl-C via the trap)
next_summary=$((SECONDS + SUMMARY_PERIOD_S))
while :; do
    check_exits
    [ "${#EXITED[@]}" -eq "$NUM_NODES" ] && break
    if [ "$SUMMARY_PERIOD_S" -gt 0 ] && [ "$SECONDS" -ge "$next_summary" ]; then
        print_summary
        next_summary=$((SECONDS + SUMMARY_PERIOD_S))
    fi
    sleep 1
done
[ "$SUMMARY_PERIOD_S" -gt 0 ] && print_summary
echo "all nodes exited"
