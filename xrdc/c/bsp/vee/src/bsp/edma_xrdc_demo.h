/*
 * Copyright 2019-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * This code has been modified by MicroEJ Corp.
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fsl_edma.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_xrdc2.h"

#include "fsl_dmamux.h"
#include "fsl_debug_console.h"

#include "fsl_common.h"
#include "fsl_cache.h"

/*******************************************************************************
 * Constants
 ******************************************************************************/

/*
 * @brief Identifies the XRDC2
 */
#define DEMO_XRDC2 XRDC2_D0

/*
 * @brief XRDC2 Domain IDs assigned to CM7 and eDMA.
 *
 * Each bus master is associated with a domain ID.
 * Valid domain IDs comprised from 0 to (FSL_FEATURE_XRDC2_DOMAIN_COUNT -1 ).
 */
#define DEMO_CORE_DOMAIN 2
#define DEMO_EDMA_DOMAIN 3

/*
 * @brief Defines the size of the buffer used during the EDMA transfer.
 */
#define BUFF_LENGTH   1U

/*
 * @brief Defines the XRDC2 memory region address range.  
 *
 * Memory M4 LMEM region is used in this example. M7 accesses this region
 * through the address 0x20200000 ~ 0x2023ffff.
 */
#define DEMO_XRDC2_MEM            kXRDC2_Mem_M4LMEM_Region0
#define DEMO_XRDC2_MEM_START_ADDR 0x20200000
#define DEMO_XRDC2_MEM_END_ADDR   0x2023ffff

/*
 * @brief Defines the address of the VEE Port data
 */
#define DEST_ADDR                 0x20200000

/*******************************************************************************
 * Functions that must be implemented
 ******************************************************************************/

/*
 * @brief This function is the SNI used to retrieve the VEE data defined in the C world.
 *
 * @param[in] void
 */
bool Java_com_microej_sampleapplication_Main_getVeeData(void);

/*
 * @brief This function is responsible for configuring the XRDC and launching the eDMA transfers to show the XRDC
 * protection capabilities.
 *
 * @param[in] void
 */
void demo_task_scheduler(void);

#endif /* _APP_H_ */
