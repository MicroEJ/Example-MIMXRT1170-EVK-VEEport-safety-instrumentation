@echo off
SETLOCAL ENABLEEXTENSIONS

REM BAT
REM Copyright 2021-2025 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

REM 'build.bat' implementation for MicroEJ native Windows builds.

REM 'build.bat' is responsible for producing the executable file.

REM Save application current directory and jump one level above scripts
SET CURRENT_DIRECTORY=%CD%
CD "%~dp0\..\"

REM Sets the project variables
CALL "scripts\set_project_env.bat"

IF %ERRORLEVEL% NEQ 0 (
	ECHO error level %ERRORLEVEL%
	EXIT /B %ERRORLEVEL%
)

REM Build the executable for the board set in environment variables
west build -p -b %BOARD%

IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

COPY build\zephyr\zephyr.elf "%CURRENT_DIRECTORY%\application.out"

IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)

REM Restore application directory
CD "%CURRENT_DIRECTORY%"

IF %ERRORLEVEL% NEQ 0 (
	EXIT /B %ERRORLEVEL%
)