ls
REM Copyright 2025 MicroEJ Corp. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be found with this software.

@echo off
REM Copy files to the copy_test folder inside xrdc
set DESTINATION="nxpvee-mimxrt1170-evk"

REM Copy each file individually
copy "c\bsp\vee\scripts\armgcc\CMakeLists.txt" "%DESTINATION%\bsp\vee\scripts\armgcc\"
copy "c\bsp\vee\scripts\armgcc\config.cmake" "%DESTINATION%\bsp\vee\scripts\armgcc\"
copy "c\bsp\vee\src\bsp\edma_xrdc_demo.c" "%DESTINATION%\bsp\vee\src\bsp\"
copy "c\bsp\vee\src\bsp\edma_xrdc_demo.h" "%DESTINATION%\bsp\vee\src\bsp\"
copy "c\bsp\vee\src\main\npavee.c" "%DESTINATION%\bsp\vee\src\main\"

echo Files copied to %DESTINATION%
pause