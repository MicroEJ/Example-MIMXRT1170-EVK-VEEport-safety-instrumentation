@echo off

REM BAT
REM Copyright 2021-2025 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'run.bat' implementation for MicroEJ Windows native builds.

REM 'run.bat' is responsible for running the executable file.

REM Save application current directory and jump one level above scripts
SET CURRENT_DIRECTORY=%CD%
CD "%~dp0\..\"

REM Sets the project variables
CALL "scripts\set_project_env.bat"

IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

IF "%1"=="" (
	set APPLICATION_FILE="application.out"
) ELSE (
	For %%A in ("%1") do (
	    set APPLICATION_FILE=%%~nxA
	)
)

IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

REM Flashes the device using Jlink
west flash %FLASH_CMD%

IF %ERRORLEVEL% NEQ 0 (
	exit /B %ERRORLEVEL%
)

CD "%CURRENT_DIRECTORY%"