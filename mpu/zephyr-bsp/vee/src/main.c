/*
 * C
 *
 * Copyright 2014-2025 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "microej_main.h"
#include "LLMJVM.h"
#include "sni.h"
#ifdef CONFIG_ARCH_POSIX
#include <nsi_main.h>
#endif

#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/shell/shell.h>

#include <zephyr/app_memory/mem_domain.h>
#include <zephyr/arch/arm/cortex_m/memory_map.h>
#include <zephyr/app_memory/app_memdomain.h>
#include <zephyr/sys/libc-hooks.h> /* for z_libc_partition */
#include <zephyr/app_memory/partitions.h>

// Initializes the memory domain structures
#ifdef CONFIG_USERSPACE
struct k_mem_domain authorized_domain;
struct k_mem_domain unauthorized_domain;
#endif

// Define a partition shared between the MICROEJ VEE thread and the authorized native thread.
K_APPMEM_PARTITION_DEFINE(vee_partition);

// Stack size used by the threads
#define NATIVE_STACK_SIZE 1024
#define VEE_STACK_SIZE 10000

// Scheduling priority used by the threads
#define PRIORITY 7

// Initializes the native threads stacks
struct k_thread authorized_thread;
K_THREAD_STACK_DEFINE(authorized_thread_stack, NATIVE_STACK_SIZE);
struct k_thread unauthorized_thread;
K_THREAD_STACK_DEFINE(unauthorized_thread_stack, NATIVE_STACK_SIZE);

// Initializes the VEE thread stack
struct k_thread vee_thread;
K_THREAD_STACK_DEFINE(vee_stack, VEE_STACK_SIZE);

// Defines the global variables used by the VEE Java application
K_APP_DMEM(vee_partition) volatile bool vee_data = false;

// SNI that returns the value of the shared is_corrupted variable
bool Java_com_microej_example_mpu_Main_getVeeData(){
	return vee_data;
}

// SNI that returns the value of the protected is_corrupted variable
void Java_com_microej_example_mpu_Main_resetVeeData(){
	vee_data = false;
}

// Defines the code of the native thread
void thread_code(){
	vee_data = true;
}

// Creates a native thread that has access to the VEE data
k_tid_t create_authorized_thread(){
	/* 
	 * Defines the partitions for the native thread corrupting shared data
	 */
	struct k_mem_partition *native_parts[] = {
		&vee_partition,
#if Z_LIBC_PARTITION_EXISTS
		&z_libc_partition
#endif
	};

	/*
	 * Creates the native thread corrupting shared data, inits the memory domain 
	 * and add partitions to that domain then add the thread
	 * to the domain
	 */
	k_tid_t native_tid = k_thread_create(&authorized_thread, 
		authorized_thread_stack, NATIVE_STACK_SIZE, 
		thread_code, NULL, NULL, NULL, 
		PRIORITY, K_USER, K_FOREVER);
	printf("AUTHORIZED NATIVE THREAD created\n");

	int ret = k_mem_domain_init(&authorized_domain, ARRAY_SIZE(native_parts), native_parts);
	__ASSERT(ret == 0, "k_mem_domain_init() on memory domain failed %d", ret);
	printf("z_libc_partition partition added to Authorized memory domain\n");
	printf("MICROEJ VEE partition added to Authorized memory domain\n");
		
	ret = k_mem_domain_add_thread(&authorized_domain, native_tid);
	__ASSERT(ret == 0, "k_mem_domain_add_thread() failed %d", ret);
	printf("AUTHORIZED NATIVE THREAD added to authorized memory domain\n");

	// Sets the name of the native thread
	ret = k_thread_name_set(native_tid, "AUTHORIZED NATIVE THREAD");
	__ASSERT(ret == 0, "k_thread_name_set() failed %d", ret);

	return native_tid;
}

// Creates a native thread that corrupts protected data
k_tid_t create_unauthorized_thread(){
	/* 
	 * Defines the partitions for the native thread corrupting protected data
	 */
	struct k_mem_partition *native_parts[] = {
#if Z_LIBC_PARTITION_EXISTS
		&z_libc_partition,
#endif
	};

	/*
	 * Creates the native thread corrupting protected data, inits the memory domain 
	 * and add partitions to that domain then add the thread
	 * to the domain
	 */
	k_tid_t native_tid = k_thread_create(&unauthorized_thread, 
		unauthorized_thread_stack, NATIVE_STACK_SIZE, 
		thread_code, NULL, NULL, NULL, 
		PRIORITY, K_USER, K_FOREVER);
	printf("UNAUTHORIZED NATIVE THREAD created\n");

	int ret = k_mem_domain_init(&unauthorized_domain, ARRAY_SIZE(native_parts), native_parts);
	__ASSERT(ret == 0, "k_mem_domain_init() on memory domain failed %d", ret);
	printf("z_libc_partition partition added to Unauthorized memory domain\n");

	ret = k_mem_domain_add_thread(&unauthorized_domain, native_tid);
	__ASSERT(ret == 0, "k_mem_domain_add_thread() failed %d", ret);
	printf("UNAUTHORIZED NATIVE THREAD added to unauthorized memory domain\n");

	// Sets the name of the native thread
	ret = k_thread_name_set(native_tid, "UNAUTHORIZED NATIVE THREAD");
	__ASSERT(ret == 0, "k_thread_name_set() failed %d", ret);

	return native_tid;
}

// Creates the VEE thread in supervisor mode
k_tid_t create_vee_thread(){
	k_tid_t vee_tid = k_thread_create(&vee_thread, vee_stack, VEE_STACK_SIZE,
					microej_main, NULL, NULL, NULL, PRIORITY, 0, 
					K_FOREVER);
	printf("MICROEJ VEE thread created\n");

	// Sets the name of the VEE thread
	k_thread_name_set(vee_tid, "MICROEJ VEE THREAD");

	return vee_tid;
}

int main(void) {

	// Creates the threads
	k_tid_t t_id_authorized = create_authorized_thread();
	k_tid_t t_id_unauthorized = create_unauthorized_thread();
	k_tid_t t_id_vee = create_vee_thread();

	/*
	 * Launching the threads
	 */
	printf("MICROEJ VEE thread started\n");
	k_thread_start(t_id_vee);
	
	// Waits for the VEE thread to run 
	k_sleep(K_MSEC(4000));
	
	printf("AUTHORIZED NATIVE THREAD that corrupts VEE data started\n");
	k_thread_start(t_id_authorized);

	// Waits for the VEE thread to run 
	k_sleep(K_MSEC(4000));
	
	printf("UNAUTHORIZED NATIVE THREAD that corrupts VEE data started\n");
	k_thread_start(t_id_unauthorized);

#ifdef CONFIG_ARCH_POSIX
	nsi_exit(0);
#endif
	return 0;
}
