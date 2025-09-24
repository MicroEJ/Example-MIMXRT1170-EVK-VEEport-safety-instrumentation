#!/bin/bash
#
# BASH
#
# Copyright 2023-2025 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

# 'run.sh' implementation for MicroEJ Linux builds.

# 'run.sh' is responsible for producing the executable file.

# Exit on failure & fail on unset variable
set -euo pipefail

# Save application current directory
current_dir=$(pwd)


# Get the absolute path of the directory that contains the script
origin=$(cd "$(dirname "$0")" && pwd) || exit

cd ${origin}
run_env="${origin}"/set_project_env.sh
if [ -f "${run_env}" ]; then
    . "${run_env}"
else
    echo -e "\n" "ERROR: Missing set_project_env.sh script." "\n"
    exit 1
fi

west flash "${WEST_FLASH_PARAM}"

cd "$current_dir"
