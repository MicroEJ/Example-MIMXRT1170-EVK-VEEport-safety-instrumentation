# Copyright 2025 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

#!/bin/bash
# Copy files to the mimxrt1170-vee-port folder inside xrdc
DESTINATION="nxpvee-mimxrt1170-evk"

# Ensure destination directories exist
mkdir -p "$DESTINATION/bsp/vee/scripts/armgcc"
mkdir -p "$DESTINATION/bsp/vee/src/bsp"
mkdir -p "$DESTINATION/bsp/vee/src/main"

# Copy each file individually
cp "c/bsp/vee/scripts/armgcc/CMakeLists.txt" "$DESTINATION/bsp/vee/scripts/armgcc/"
cp "c/bsp/vee/scripts/armgcc/config.cmake" "$DESTINATION/bsp/vee/scripts/armgcc/"
cp "c/bsp/vee/src/bsp/edma_xrdc_demo.c" "$DESTINATION/bsp/vee/src/bsp/"
cp "c/bsp/vee/src/bsp/edma_xrdc_demo.h" "$DESTINATION/bsp/vee/src/bsp/"
cp "c/bsp/vee/src/main/npavee.c" "$DESTINATION/bsp/vee/src/main/"

echo "Files copied to $DESTINATION"