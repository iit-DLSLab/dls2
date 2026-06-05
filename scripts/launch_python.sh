#!/bin/sh
set -eu

if [ "$#" -lt 1 ]; then
    echo "launch_python.sh: missing python periodic app entrypoint" >&2
    exit 2
fi

entrypoint="$1"
shift

case "$entrypoint" in
    /*) target="$entrypoint" ;;
    *)  target="/usr/bin/$entrypoint" ;;
esac

if [ ! -e "$target" ]; then
    echo "launch_python.sh: target '$target' does not exist" >&2
    exit 127
fi

if [ ! -x "$target" ]; then
    echo "launch_python.sh: target '$target' is not executable" >&2
    exit 126
fi

exec "$target" "$@"
