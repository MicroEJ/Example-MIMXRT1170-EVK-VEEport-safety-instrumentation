# Copyright 2025 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

include_guard()

target_sources(${target} PRIVATE
${CMAKE_CURRENT_LIST_DIR}/src/LLBSP_ZEPHYR.c
${CMAKE_CURRENT_LIST_DIR}/src/microej_main.c
${CMAKE_CURRENT_LIST_DIR}/src/microej_time.c
${CMAKE_CURRENT_LIST_DIR}/src/LLMJVM_ZephyrOS.c
)

target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc)
# Source for core testsuite validation
target_sources(${target} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/core_validation.c)