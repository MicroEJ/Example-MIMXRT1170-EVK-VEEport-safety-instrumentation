# Copyright 2025 MicroEJ Corp. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be found with this software.

cmake_minimum_required(VERSION 3.20.0)

# Configure and fetch zephyr cmake dependencies
# load default options
include(${CMAKE_CURRENT_LIST_DIR}/options.cmake)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(zephyr-vee)

set(target app)

target_sources(${target} PRIVATE src/main.c)

add_subdirectory(port/core)

# add sources
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)


target_compile_options(${target}
		PUBLIC
		-fno-stack-protector
		-fdollars-in-identifiers
		-fno-strict-aliasing
		-Winline
		-fwrapv
		-Werror-implicit-function-declaration
		-D_XOPEN_SOURCE=700
		-D_GNU_SOURCE
		-D__iceTea__RELEASE
)



add_library(microEJRuntime STATIC IMPORTED)
set_target_properties(microEJRuntime PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/microejruntime.a)
add_library(microEJApp STATIC IMPORTED)
set_target_properties(microEJApp PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/lib/microejapp.o)

target_link_libraries(${target} PRIVATE microEJRuntime microEJApp ${CMAKE_CURRENT_LIST_DIR}/lib/microejruntime.a)

