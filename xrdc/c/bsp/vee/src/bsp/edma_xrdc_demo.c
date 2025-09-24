/*
 * Copyright 2019-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * This code has been modified by MicroEJ Corp.
 */

/*******************************************************************************
* Includes
******************************************************************************/
#include "edma_xrdc_demo.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cpuload.h"

#include "task.h"
#include "npavee.h"

/*******************************************************************************
 * Macros and defines
 ******************************************************************************/

#define EDMA                        DMA0
#define DMAMUX                      DMAMUX0
#define DMA0_DMA16_DriverIRQHandler DMA_CH_0_16_DriverIRQHandler

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

edma_handle_t g_EDMA_Handle;
volatile bool g_Transfer_Done = false;
AT_NONCACHEABLE_SECTION_INIT(uint32_t srcAddr[BUFF_LENGTH]) = {0x01};

/*******************************************************************************
 * Private functions
 ******************************************************************************/

/*************************************
 * eDMA Code
 *************************************/

/*
 * @brief User callback function for eDMA transfer.
 */
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    } 
}

/*
 * @brief This function makes the eDMA transfer.
 */
void DEMO_MakeEDMATransfer(void)
{
    edma_transfer_config_t transferConfig;
    edma_config_t userConfig;

    /* Setting the destination buffer to 0 */
    *(volatile uint32_t *)DEST_ADDR = 0x00;

    /* Logging the value of the destination buffer */
    PRINTF("Destination Buffer:");
    PRINTF("\t%d\n", *(volatile uint32_t *)DEST_ADDR);

    /* Invalidate the cache, so new read will read from memory directly */
    DCACHE_InvalidateByRange(DEST_ADDR, sizeof(uint32_t));

    /* Print source buffer */
    PRINTF("eDMA memory to memory transfer example begin.\r\n\r\n");
    /* Configure DMAMUX ( Direct Memory Access Multiplexer) */
    DMAMUX_Init(DMAMUX);
#if defined(FSL_FEATURE_DMAMUX_HAS_A_ON) && FSL_FEATURE_DMAMUX_HAS_A_ON
    DMAMUX_EnableAlwaysOn(DMAMUX, 0, true);
#else
    DMAMUX_SetSource(DMAMUX, 0, 63);
#endif /* FSL_FEATURE_DMAMUX_HAS_A_ON */
    DMAMUX_EnableChannel(DMAMUX, 0);
    /* Configure EDMA one shot transfer */
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(EDMA, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, EDMA, 0);
    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
    EDMA_PrepareTransfer(&transferConfig, 
                         srcAddr, sizeof(srcAddr[0]), 
                         DEST_ADDR, sizeof(*(volatile uint32_t *)DEST_ADDR),
                         sizeof(srcAddr[0]), sizeof(srcAddr), kEDMA_MemoryToMemory);
    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
    EDMA_StartTransfer(&g_EDMA_Handle);

    /* Wait for eDMA transfer finish */
    PRINTF("eDMA memory to memory transfer ongoing...\n");

    /* Logging the status of the EDMA transfer */
    if (g_Transfer_Done){
        PRINTF("\neDMA memory to memory transfer finish.\n");
        g_Transfer_Done = false;
    } else {
        PRINTF("\neDMA memory to memory transfer did not finish.\n");
    }

    /* Logging the value of the destination buffer */
    PRINTF("Destination Buffer:");
    PRINTF("\t%d\n", *(volatile uint32_t *)DEST_ADDR);

    /* Logging the eDMA Status Flags */
    DEMO_CheckEDMAStatusFlags();

    EDMA_AbortTransfer(&g_EDMA_Handle);
}

/*
 * @brief Functions handling the eDMA Channel Status Flag and eventually the Error Status Flag.
 */
void DEMO_CheckEDMAStatusFlags(void){

    /* Checking the Channel Status Flags */
    uint16_t channelStatusFlag = EDMA_GetChannelStatusFlags(EDMA, 0);
    PRINTF("\neDMA Channel 0 Status Flag = 0x%08X\n", channelStatusFlag);
    if (channelStatusFlag & kEDMA_DoneFlag){
        PRINTF(" -> DONE flag, set while transfer finished, CITER value exhausted\n");
    }
    if (channelStatusFlag & kEDMA_ErrorFlag){
        PRINTF(" -> eDMA error flag, an error occurred in a transfer\n");

        /* Checking the Error Status Flags */
        uint32_t errorFlags = EDMA_GetErrorStatusFlags(EDMA);
        PRINTF(" -> Error Status Flag = 0x%08X\n", EDMA_GetErrorStatusFlags(EDMA));
        
        if (errorFlags & kEDMA_DestinationBusErrorFlag){
            PRINTF(" -> Destination bus error\n");
        }
        if (errorFlags & kEDMA_SourceBusErrorFlag){
            PRINTF(" -> Source bus error\n");
        }
        if (errorFlags & kEDMA_ScatterGatherErrorFlag){
            PRINTF(" -> Scatter/Gather address error (not 32-byte aligned)\n");
        }
        if (errorFlags & kEDMA_NbytesErrorFlag){
            PRINTF(" -> NBYTES/CITER configuration error\n");
        }
        if (errorFlags & kEDMA_DestinationOffsetErrorFlag){
            PRINTF(" -> Destination offset not aligned with destination size\n");
        }
        if (errorFlags & kEDMA_DestinationAddressErrorFlag){
            PRINTF(" -> Destination address not aligned with destination size\n");
        }
        if (errorFlags & kEDMA_SourceOffsetErrorFlag){
            PRINTF(" -> Source offset not aligned with source size\n");
        }
        if (errorFlags & kEDMA_SourceAddressErrorFlag){
            PRINTF(" -> Source address not aligned with source size\n");
        }
        if (errorFlags & kEDMA_ErrorChannelFlag){
            PRINTF(" -> Error channel number of canceled channel\n");
        }
        if (errorFlags & kEDMA_ChannelPriorityErrorFlag){
            PRINTF(" -> Channel priority is not unique\n");
        }
        if (errorFlags & kEDMA_TransferCanceledFlag){
            PRINTF(" -> Transfer canceled\n");
        }
    }
    if (channelStatusFlag & kEDMA_InterruptFlag){
        PRINTF(" -> eDMA interrupt flag, set while an interrupt occurred of this channel\n");
    }

    EDMA_ClearChannelStatusFlags(EDMA, 0, channelStatusFlag);
}

/*************************************
 * XRDC Code
 *************************************/

/*
 * @brief Function assigning the Master Domain for the CM7
 */
void DEMO_AssignDomainCM7(void)
{
    xrdc2_master_domain_assignment_t assignment;

    XRDC2_GetDefaultMasterDomainAssignment(&assignment);

    /* Defining privileges to the CM7 */
    assignment.lock          = false;
    assignment.privilegeAttr = kXRDC2_ForceUser;
    assignment.secureAttr    = kXRDC2_ForceSecure;
    assignment.domainId      = DEMO_CORE_DOMAIN;

    /* The XID input is not used for domain assignment hit. */
    assignment.mask  = 0x3FUL;
    assignment.match = 0UL;
    XRDC2_SetMasterDomainAssignment(DEMO_XRDC2, kXRDC2_Master_M7_AXI, 0, &assignment);

    /* The XID input is not used for domain assignment hit. */
    assignment.mask  = 0x3FUL;
    assignment.match = 0UL;
    XRDC2_SetMasterDomainAssignment(DEMO_XRDC2, kXRDC2_Master_M7_AHB, 0, &assignment);
}

/*
 * @brief Function assigning the Master Domain for the eDMA
 */
void DEMO_AssignDomainEDMA(void)
{
    xrdc2_master_domain_assignment_t assignment;

    XRDC2_GetDefaultMasterDomainAssignment(&assignment);

    /* Defining privileges to the eDMA */
    assignment.lock          = false;
    assignment.privilegeAttr = kXRDC2_ForcePrivilege;
    assignment.secureAttr    = kXRDC2_ForceNonSecure;
    assignment.domainId      = DEMO_EDMA_DOMAIN;

    /* The XID and HMASTER are not used for domain assignment hit. 
       Only CHANNEL_ID is used for hit generation (it indicates the DMA channel index) */
    assignment.mask  = 0x3FF;       // Ignore HMASTER and XID
    assignment.match = (0 << 10);   // CHANNEL_ID = 0
    XRDC2_SetMasterDomainAssignment(DEMO_XRDC2, kXRDC2_Master_M7_EDMA, 0, &assignment);
}

/*
 * @brief Function that sets the MicroEJ VEE memory accssible for CM7 and eDMA
 */
void DEMO_SetMemoryAccessible(void)
{
    PRINTF("\r\n\r\nXRDC access setting to the memory :\r\n");
    PRINTF("CM7\t\tr, w\r\n");
    PRINTF("eDMA\t\tr, w\r\n\n");

    xrdc2_mem_access_config_t memAccessConfig;

    XRDC2_GetMemAccessDefaultConfig(&memAccessConfig);

    memAccessConfig.startAddr                = DEMO_XRDC2_MEM_START_ADDR;
    memAccessConfig.endAddr                  = DEMO_XRDC2_MEM_END_ADDR;
    memAccessConfig.policy[DEMO_CORE_DOMAIN] = kXRDC2_AccessPolicyAlt3;
    memAccessConfig.policy[DEMO_EDMA_DOMAIN] = kXRDC2_AccessPolicyAlt6;

    XRDC2_SetMemAccessConfig(DEMO_XRDC2, DEMO_XRDC2_MEM, &memAccessConfig);
}

/*
 * @brief Functions that sets the MicroEJ VEE memory accssible by CM7 and not accessible by eDMA
 */
void DEMO_SetMemoryUnaccessible(void)
{
    PRINTF("\r\n\r\nXRDC access setting to the memory :\r\n");
    PRINTF("CM7\t\tr, w\r\n");
    PRINTF("eDMA\t\tnone\r\n\n");

    xrdc2_mem_access_config_t memAccessConfig;

    XRDC2_GetMemAccessDefaultConfig(&memAccessConfig);

    memAccessConfig.startAddr                = DEMO_XRDC2_MEM_START_ADDR;
    memAccessConfig.endAddr                  = DEMO_XRDC2_MEM_END_ADDR;
    memAccessConfig.policy[DEMO_CORE_DOMAIN] = kXRDC2_AccessPolicyAlt3;
    memAccessConfig.policy[DEMO_EDMA_DOMAIN] = kXRDC2_AccessPolicyAlt3;

    XRDC2_SetMemAccessConfig(DEMO_XRDC2, DEMO_XRDC2_MEM, &memAccessConfig);
}

/*
 * @brief Functions that sets all the memory regions accessible by all core/peripherals
 */
void DEMO_SetAllMemAccessible(void)
{
    PRINTF("\r\n\r\nXRDC access setting to the memory :\r\n");
    PRINTF("CM7\t\tr, w\r\n");
    PRINTF("eDMA\t\tr, w\r\n\n");

    uint8_t domain;
    xrdc2_mem_access_config_t memAccessConfig;
    XRDC2_GetMemAccessDefaultConfig(&memAccessConfig);

    for (domain = 0; domain < FSL_FEATURE_XRDC2_DOMAIN_COUNT; domain++)
    {
        memAccessConfig.policy[domain] = kXRDC2_AccessPolicyAll;
    }

    /* CAAM */
    memAccessConfig.startAddr = 0x00280000U;
    memAccessConfig.endAddr   = 0x0028FFFFU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_CAAM_Region0, &memAccessConfig);

    /* FLEXSPI1 */
    memAccessConfig.startAddr = 0x2F800000U;
    memAccessConfig.endAddr   = 0x3FFFFFFFU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_FLEXSPI1_Region0, &memAccessConfig);

    /* FLEXSPI2 */
    memAccessConfig.startAddr = 0x60000000U;
    memAccessConfig.endAddr   = 0x7FFFFFFFU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_FLEXSPI2_Region0, &memAccessConfig);

    /* M4 LMEM */
    memAccessConfig.startAddr = 0x20200000U;
    memAccessConfig.endAddr   = 0x2023ffffU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_M4LMEM_Region0, &memAccessConfig);

    /* M7 OCRAM */
    memAccessConfig.startAddr = 0x20360000U;
    memAccessConfig.endAddr   = 0x203fffffU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_M7OC_Region0, &memAccessConfig);

    /* SEMC */
    memAccessConfig.startAddr = 0x80000000U;
    memAccessConfig.endAddr   = 0xBFFFFFFFU;
    XRDC2_SetMemAccessConfig(DEMO_XRDC2, kXRDC2_Mem_SEMC_Region0, &memAccessConfig);
}

/*
 * @brief Function that initializes the hardware
 */
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

/*******************************************************************************
 * edma_xrdc_demo.h functions
 ******************************************************************************/

 /*
 * @brief See the header file for the function documentation.
 *
 * This function initializes and configures the XRDC2 for the demonstration. It also sets the VEE data to 0.
 */
void DEMO_Init_XRDC2(void){
    /* Assigning Bus Masters to XRDC Domains */
    DEMO_AssignDomainCM7();
    DEMO_AssignDomainEDMA();

    DEMO_SetAllMemAccessible();
    XRDC2_SetGlobalValid(DEMO_XRDC2, true);

    /* Checking if the CM7 Bus Master has a domain assigned */
    if (XRDC2_GetCurrentMasterDomainId(DEMO_XRDC2) != DEMO_CORE_DOMAIN)
    {
        PRINTF("ERROR: Domain set failed because XRDC2_GetCurrentMasterDomainId(DEMO_XRDC2) != DEMO_CORE_DOMAIN\r\n");
        while (1)
            ;
    }

    /* Setting the vee data to 0 */
    *(volatile uint32_t *)DEST_ADDR = 0x00;
}

/*
 * @brief See the header file for the function documentation.
 *
 * This function is the SNI used to retrieve the VEE data defined in the C world.
 */
bool Java_com_microej_sampleapplication_Main_getVeeData(){
	return *(volatile uint32_t *)DEST_ADDR;
}

/*
 * @brief See the header file for the function documentation.
 *
 * This function is responsible for configuring the XRDC and launching the eDMA transfers to show the XRDC
 * protection capabilities.
 */
void demo_task_scheduler(void){

    /* Configures the XRDC2 for the demonstration. It also sets the VEE data to 0. */
    PRINTF("XRDC2/eDMA example started\r\n");
    DEMO_Init_XRDC2();

    /* Blocks the execution for 5 seconds */
    const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;
    vTaskDelay(xDelay);

    /* Configures the XRDC to set the MicroEJ VEE data unaccessible by eDMA */
    DEMO_SetMemoryUnaccessible();

    /* Makes the EDMA memory to memory transfer */
    DEMO_MakeEDMATransfer();

    /* Blocks the execution for 5 seconds */
    vTaskDelay(xDelay);

    /* Configures the XRDC to set the MicroEJ VEE data accessible by eDMA */
    DEMO_SetMemoryAccessible();

    /* Makes the EDMA memory to memory transfer */
    DEMO_MakeEDMATransfer();

    /* Blocks the execution for two seconds */
    const TickType_t xDelay2 = 2000 / portTICK_PERIOD_MS;
    vTaskDelay(xDelay2);

    /* Launches the task that Deinits the XRDC and logs the finished demo log */
    PRINTF("\r\nXRDC2/eDMA example Success\r\n");
    XRDC2_SetGlobalValid(DEMO_XRDC2, false);
    XRDC2_Deinit(DEMO_XRDC2);

    vTaskDelete(NULL);
}