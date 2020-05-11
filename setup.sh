#!/usr/bin/env bash

# ==============================================================================
# Test for lock file
# ==============================================================================
# If the lock file exists, this script has been run in the current copy of the
# repository. Do not run again
project_root=$(git rev-parse --show-toplevel)
if [ -e $project_root/.setup.lock ]; then
	exit 0
fi

# ==============================================================================
# Githooks
# ==============================================================================
echo -- Installing githooks
rm -r .git/hooks
ln -sf ../githooks .git/hooks

# ==============================================================================
# Create Lock File
# ==============================================================================
echo "This file tracks whether the setup.sh script has been run" > $project_root/.setup.lock
