@echo off

REM BAT
REM Copyright 2021-2025 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'set_project_env.bat' implementation for MicroEJ Windows builds.

REM 'set_project_env' is responsible for
REM - setting project variables for 'build.bat' and 'run.bat' 

REM Set "flash_linkserver" for Linkserver probe or "flash_jlink" for J-Link probe
SET CHOSEN_PROBE=flash_linkserver

IF "%CHOSEN_PROBE%" == "flash_jlink" (
    SET FLASH_CMD=-r jlink
)
IF "%CHOSEN_PROBE%" == "flash_linkserver" (
    SET FLASH_CMD=-r linkserver
)
ECHO "Flash command: %FLASH_CMD%"

SET BOARD=mimxrt1170_evk/mimxrt1176/cm7
ECHO "Board: %BOARD%"