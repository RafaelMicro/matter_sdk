/*
 *  Copyright (c) 2023, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file implements the OpenThread Border Router application.
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include <assert.h>

#include "lwip/mld6.h"
#include "lwip/nd6.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dns.h"
#include "lwip/prot/iana.h"
#include "lwip/tcpip.h"
#include "lwip/udp.h"

#include "board.h"
#include "pin_mux.h"

#include "addons_cli.h"
#include "app_ot.h"
#include "br_rtos_manager.h"
#include "ot_lwip.h"

#include "openthread-system.h"
#include <openthread-core-config.h>
#include <openthread/mdns_server.h>
#include <openthread/nat64.h>

#include <openthread/cli.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>
#include <openthread/udp.h>
#include "common/code_utils.hpp"

#if defined(OT_APP_BR_ETH_EN) && defined(OT_NXP_PLATFORM_RW612)
#include <openthread/platform/entropy.h>
#endif

#ifdef OT_APP_BR_ETH_EN
#include "ethernetif.h"
#include "fsl_enet.h"
#include "fsl_phy.h"
#if defined(OT_NXP_PLATFORM_RT1170)
#include "fsl_iomuxc.h"
#include "fsl_phyrtl8211f.h"
#elif defined(OT_NXP_PLATFORM_RT1060)
#include "fsl_iomuxc.h"
#include "fsl_phyksz8081.h"
#elif defined(OT_NXP_PLATFORM_RW612)
#include "fsl_phyksz8081.h"
#include "fsl_reset.h"
#endif
#include "fsl_silicon_id.h"
#endif

#ifdef OT_APP_BR_WIFI_EN
#include "wm_net.h"
#include "wpl.h"
#endif

#if OT_APP_BR_LWIP_HOOKS_EN
#include LWIP_HOOK_FILENAME
#endif

/* -------------------------------------------------------------------------- */
/*                                 Definitions                                */
/* -------------------------------------------------------------------------- */

#ifndef OT_MAIN_TASK_PRIORITY
#define OT_MAIN_TASK_PRIORITY 1
#endif

#ifndef OT_MAIN_TASK_SIZE
#define OT_MAIN_TASK_SIZE ((configSTACK_DEPTH_TYPE)8192 / sizeof(portSTACK_TYPE))
#endif

#ifndef OT_WIFI_CFG_TASK_PRIORITY
#define OT_WIFI_CFG_TASK_PRIORITY 3
#endif

#ifndef OT_WIFI_CFG_TASK_SIZE
#define OT_WIFI_CFG_TASK_SIZE ((configSTACK_DEPTH_TYPE)(4 * 1024) / sizeof(portSTACK_TYPE))
#endif

#if configAPPLICATION_ALLOCATED_HEAP
uint8_t __attribute__((section(".heap"))) ucHeap[configTOTAL_HEAP_SIZE];
#endif

#ifndef USE_OT_MDNS
#define USE_OT_MDNS 1
#endif

/* Ethernet configuration. */
#if defined(OT_NXP_PLATFORM_RT1170)
#define EXAMPLE_CLOCK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
#define EXAMPLE_PHY_OPS &phyrtl8211f_ops
#define EXAMPLE_NETIF_INIT_FN ethernetif1_init
#define EXAMPLE_PHY_ADDRESS BOARD_ENET1_PHY_ADDRESS
#define EXAMPLE_ENET ENET_1G
#elif defined(OT_NXP_PLATFORM_RT1060)
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)
#define EXAMPLE_PHY_OPS &phyksz8081_ops
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS
#define EXAMPLE_ENET ENET
#elif defined(OT_NXP_PLATFORM_RW612)
#define EXAMPLE_CLOCK_FREQ CLOCK_GetMainClkFreq()
#define EXAMPLE_PHY_OPS &phyksz8081_ops
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS
#define EXAMPLE_ENET ENET
#define MAC_HW_ADDR_LEN 6
#define NXP_OUI_LEN 3
#endif

#if defined(WIFI_SSID) && (!defined(WIFI_PASSWORD))
#define WIFI_PASSWORD ""
#endif

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */
static TaskHandle_t sMainTask = NULL;

#ifdef OT_APP_BR_ETH_EN
static phy_handle_t phyHandle;
#ifdef OT_NXP_PLATFORM_RT1170
static phy_rtl8211f_resource_t g_phy_resource;
#else
static phy_ksz8081_resource_t g_phy_resource;
#endif
static struct netif sExtNetif;
#endif

static SemaphoreHandle_t sMainStackLock;
static struct netif     *sExtNetifPtr;

static otInstance          *sInstance = NULL;
static netif_ext_callback_t sNetifCallback;

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

#ifdef OT_APP_BR_ETH_EN
static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data);
static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData);
static void     appConfigEnetHw();
static void     appConfigEnetIf();
#if defined(OT_NXP_PLATFORM_RW612)
static void appGenerateMacAddr(uint8_t (*macAddr)[MAC_HW_ADDR_LEN]);
#endif
#endif

#ifdef OT_APP_BR_WIFI_EN
static void wifiLinkCB(bool up);
static void appConfigWifiIf();
#endif

static void appOtInit();
static void appBrInit();
static void mainloop(void *aContext);
static void NetifExtCb(struct netif *netif, netif_nsc_reason_t reason, const netif_ext_callback_args_t *args);

/* -------------------------------------------------------------------------- */
/*                              Public functions prototypes                   */
/* -------------------------------------------------------------------------- */

extern void otAppCliInit(otInstance *aInstance);
extern void otSysRunIdleTask(void);

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

#ifdef OT_APP_BR_ETH_EN
static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    const status_t s = ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
    return s;
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    const status_t s = ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
    return s;
}

static void appConfigEnetHw()
{
    /* Enet pins */
    BOARD_InitENETPins();

    /* Enet clock */
#if defined(OT_NXP_PLATFORM_RT1170)
    gpio_pin_config_t             gpio_config   = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    const clock_sys_pll1_config_t sysPll1Config = {
        .pllDiv2En = true,
    };
    CLOCK_InitSysPll1(&sysPll1Config);
    clock_root_config_t rootCfg = {.mux = 4, .div = 4}; /* Generate 125M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet2, &rootCfg);

    IOMUXC_GPR->GPR5 |= IOMUXC_GPR_GPR5_ENET1G_RGMII_EN_MASK;

    /* Reset phy */
    GPIO_PinInit(GPIO11, 14, &gpio_config);
    /* For a complete PHY reset of RTL8211FDI-CG, this pin must be asserted low for at least 10ms. And
     * wait for a further 30ms(for internal circuits settling time) before accessing the PHY register */
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO11, 14, 1);
    SDK_DelayAtLeastUs(30000, CLOCK_GetFreq(kCLOCK_CpuClk));

    EnableIRQ(ENET_1G_MAC0_Tx_Rx_1_IRQn);
    EnableIRQ(ENET_1G_MAC0_Tx_Rx_2_IRQn);
#elif defined(OT_NXP_PLATFORM_RT1060)
    gpio_pin_config_t             gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    /* Pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO1, 9, 1);
#elif defined(OT_NXP_PLATFORM_RW612)
    /* tddr_mci_flexspi_clk 320MHz */
    CLOCK_InitTddrRefClk(kCLOCK_TddrFlexspiDiv10);
    CLOCK_EnableClock(kCLOCK_TddrMciFlexspiClk); /* 320MHz */
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 1U};
    /* Set 50MHz output clock required by PHY. */
    CLOCK_EnableClock(kCLOCK_TddrMciEnetClk);

    RESET_PeripheralReset(kENET_IPG_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kENET_IPG_S_RST_SHIFT_RSTn);

    GPIO_PortInit(GPIO, 0U);
    GPIO_PortInit(GPIO, 1U);
    GPIO_PinInit(GPIO, 0U, 21U, &gpio_config); /* ENET_RST */
    gpio_config.pinDirection = kGPIO_DigitalInput;
    gpio_config.outputLogic  = 0U;
    GPIO_PinInit(GPIO, 1U, 23U, &gpio_config); /* ENET_INT */

    GPIO_PinWrite(GPIO, 0U, 21U, 0U);
    SDK_DelayAtLeastUs(1000000, CLOCK_GetCoreSysClkFreq());
    GPIO_PinWrite(GPIO, 0U, 21U, 1U);
#endif

    /* MDIO Init */
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
#ifdef OT_NXP_PLATFORM_RW612
    (void)CLOCK_EnableClock(s_enetExtraClock[ENET_GetInstance(EXAMPLE_ENET)]);
#endif
    ENET_SetSMI(EXAMPLE_ENET, EXAMPLE_CLOCK_FREQ, false);

    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
}

static void appConfigEnetIf()
{
    ethernetif_config_t enet_config = {.phyHandle   = &phyHandle,
                                       .phyAddr     = EXAMPLE_PHY_ADDRESS,
                                       .phyOps      = EXAMPLE_PHY_OPS,
                                       .phyResource = &g_phy_resource,
                                       .srcClockHz  = EXAMPLE_CLOCK_FREQ};

    sExtNetifPtr = &sExtNetif;

#ifndef OT_NXP_PLATFORM_RW612
    /* Set MAC address. */
    SILICONID_ConvertToMacAddr(&enet_config.macAddress);
#else
    /* Set random generated MAC address. */
    appGenerateMacAddr(&enet_config.macAddress);
#endif

    netifapi_netif_add(sExtNetifPtr, NULL, NULL, NULL, &enet_config, EXAMPLE_NETIF_INIT_FN, tcpip_input);
    netif_add_ext_callback(&sNetifCallback, &NetifExtCb);
    netifapi_netif_set_up(sExtNetifPtr);

    LOCK_TCPIP_CORE();
    netif_create_ip6_linklocal_address(sExtNetifPtr, 1);
    UNLOCK_TCPIP_CORE();

    netifapi_dhcp_start(sExtNetifPtr);
}
#if defined(OT_NXP_PLATFORM_RW612)
static void appGenerateMacAddr(uint8_t (*macAddr)[MAC_HW_ADDR_LEN])
{
    uint8_t randGenId[MAC_HW_ADDR_LEN - NXP_OUI_LEN];
    (void)otPlatEntropyGet(randGenId, sizeof(randGenId));
    /* Set NXP OUI. */
    macAddr[0][0] = 0x54;
    macAddr[0][1] = 0x27;
    macAddr[0][2] = 0x8d;

    /* Set with random generated id. */
    macAddr[0][3] = randGenId[0];
    macAddr[0][4] = randGenId[1];
    macAddr[0][5] = randGenId[2];
}
#endif // #if defined(OT_NXP_PLATFORM_RW612)
#endif

#ifdef OT_APP_BR_WIFI_EN
static void wifiLinkCB(bool up)
{
    otCliOutputFormat("Wi-fi link is now %s\r\n", up ? "up" : "down");
}

#ifdef WIFI_SSID
static void appConfigwifiIfTask(void *args)
{
    OT_UNUSED_VARIABLE(args);
    wpl_ret_t ret;

    ret = WPL_Join("my_net");
    if (ret != WPLRET_SUCCESS)
    {
        otCliOutputFormat("WPL_Join() to '%s' / '%s' failed with code %d\r\n", WIFI_SSID, WIFI_PASSWORD, ret);
        VerifyOrExit(ret != WPLRET_SUCCESS);
    }

    vTaskSuspend(NULL);

exit:
#if INCLUDE_uxTaskGetStackHighWaterMark == 1
    otCliOutputFormat("\r\n\t%s's stack watter mark: %dw\r\n", pcTaskGetName(NULL), uxTaskGetStackHighWaterMark(NULL));
#endif
    return;
}
#endif /* WIFI_SSID */

static void appConfigWifiIf()
{
    wpl_ret_t ret;

    ret = WPL_Init();
    if (ret != WPLRET_SUCCESS)
    {
        otCliOutputFormat("WPL_Init() failed with code %d\r\n", ret);
        VerifyOrExit(ret != WPLRET_SUCCESS);
    }

    netif_add_ext_callback(&sNetifCallback, &NetifExtCb);

    ret = WPL_Start(&wifiLinkCB);
    if (ret != WPLRET_SUCCESS)
    {
        otCliOutputFormat("WPL_Start() failed with code %d\r\n", ret);
        VerifyOrExit(ret != WPLRET_SUCCESS);
    }

#ifdef WIFI_SSID
    ret = WPL_AddNetwork(WIFI_SSID, WIFI_PASSWORD, "my_net");
    if (ret != WPLRET_SUCCESS)
    {
        otCliOutputFormat("WPL_AddNetwork() failed with code %d\r\n", ret);
        VerifyOrExit(ret != WPLRET_SUCCESS);
    }

    if (xTaskCreate(&appConfigwifiIfTask, "wifi-cfg", OT_WIFI_CFG_TASK_SIZE, NULL, OT_WIFI_CFG_TASK_PRIORITY, NULL) !=
        pdPASS)
    {
        otCliOutputFormat("appConfigwifiIfTask creation failed with code %d\r\n", ret);
    }
#endif /* WIFI_SSID */

    sExtNetifPtr = net_get_sta_handle();

exit:
    return;
}

#endif

static void appOtInit()
{
#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
    size_t   otInstanceBufferLength = 0;
    uint8_t *otInstanceBuffer       = NULL;
#endif

    otSysInit(0, NULL);

#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
    // Call to query the buffer size
    (void)otInstanceInit(NULL, &otInstanceBufferLength);

    // Call to allocate the buffer
    otInstanceBuffer = (uint8_t *)pvPortMalloc(otInstanceBufferLength);
    assert(otInstanceBuffer);

    // Initialize OpenThread with the buffer
    sInstance = otInstanceInit(otInstanceBuffer, &otInstanceBufferLength);
#else
    sInstance = otInstanceInitSingle();
#endif

#if OPENTHREAD_ENABLE_DIAG
    otDiagInit(sInstance);
#endif
    /* Init the CLI */
    otAppCliInit(sInstance);
    otAppCliAddonsInit(sInstance);
}

static void appBrInit()
{
#ifdef OT_APP_BR_ETH_EN
    appConfigEnetHw();
#endif

    otPlatLwipInit(sInstance, appOtLockOtTask);
    otPlatLwipAddThreadInterface();
    otSetStateChangedCallback(sInstance, otPlatLwipUpdateState, NULL);

#ifdef OT_APP_BR_WIFI_EN
    appConfigWifiIf();
#endif

#ifdef OT_APP_BR_ETH_EN
    appConfigEnetIf();
#endif

    BrInitServices(sInstance, sExtNetifPtr, otPlatLwipGetOtNetif());

    otMdnsServerSetHostName(sInstance, "NXPBR-xxxx.local.");
    otMdnsServerStart(sInstance);
}

void NetifExtCb(struct netif *netif, netif_nsc_reason_t reason, const netif_ext_callback_args_t *args)
{
    if ((reason & (LWIP_NSC_IPV6_SET | LWIP_NSC_IPV6_ADDR_STATE_CHANGED)) && netif == sExtNetifPtr)
    {
        for (int i = (LWIP_IPV6_NUM_ADDRESSES - 1); i >= 0; i--)
        {
            if (ip6_addr_isvalid(netif_ip6_addr_state(sExtNetifPtr, i)))
            {
                const ip6_addr_t *tmpAddr;
                tmpAddr = netif_ip6_addr(sExtNetifPtr, i);

                if (ip6_addr_islinklocal(tmpAddr) || ip6_addr_isuniquelocal(tmpAddr) || ip6_addr_isglobal(tmpAddr))
                {
                    const ip_addr_t *lwipAddr = netif_ip_addr6(sExtNetifPtr, i);
                    otIp6Address     externalNetifAddress;
                    memcpy(externalNetifAddress.mFields.m8, ip_2_ip6(lwipAddr),
                           sizeof(externalNetifAddress.mFields.m8));
#if OPENTHREAD_CONFIG_MDNS_SERVER_ENABLE
                    otMdnsServerAddAddress(sInstance, &externalNetifAddress);
#endif
                }
            }
        }
    }
#if OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE
    if ((reason & LWIP_NSC_IPV4_ADDRESS_CHANGED) && netif == sExtNetifPtr)
    {
        const ip4_addr_t *ip4_addr;
        otIp4Cidr         aCidr;
        otError           error;

        ip4_addr = netif_ip4_addr(sExtNetifPtr);
        if (!ip4_addr_isany(ip4_addr))
        {
            aCidr.mAddress.mFields.m32 = ip4_addr->addr;
            aCidr.mLength              = 32U;
            error                      = otNat64SetIp4Cidr(sInstance, &aCidr);
            if (error != OT_ERROR_NONE)
            {
                otCliOutputFormat("otNat64SetIp4Cidr failed: %s\r\n", otThreadErrorToString(error));
            }
        }
    }
#endif /* OPENTHREAD_CONFIG_NAT64_TRANSLATOR_ENABLE */
}

static void mainloop(void *aContext)
{
    OT_UNUSED_VARIABLE(aContext);

    appOtInit();
    appBrInit();

    otSysProcessDrivers(sInstance);
    while (!otSysPseudoResetWasRequested())
    {
        /* Aqquired the task mutex lock and release after OT processing is done */
        appOtLockOtTask(true);
        otTaskletsProcess(sInstance);
        otSysProcessDrivers(sInstance);
        appOtLockOtTask(false);

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    otInstanceFinalize(sInstance);
    vTaskDelete(NULL);
}

void appOtLockOtTask(bool bLockState)
{
    if (bLockState)
    {
        /* Aqquired the task mutex lock */
        xSemaphoreTakeRecursive(sMainStackLock, portMAX_DELAY);
    }
    else
    {
        /* Release the task mutex lock */
        xSemaphoreGiveRecursive(sMainStackLock);
    }
}

void appOtStart(int argc, char *argv[])
{
    sMainStackLock = xSemaphoreCreateRecursiveMutex();
    assert(sMainStackLock != NULL);

    xTaskCreate(mainloop, "ot", OT_MAIN_TASK_SIZE, NULL, OT_MAIN_TASK_PRIORITY, &sMainTask);
    vTaskStartScheduler();
}

void otTaskletsSignalPending(otInstance *aInstance)
{
    (void)aInstance;
    xTaskNotifyGive(sMainTask);
}

void otSysEventSignalPending(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(sMainTask, &xHigherPriorityTaskWoken);
    /* Context switch needed? */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

#if OPENTHREAD_CONFIG_HEAP_EXTERNAL_ENABLE
void *otPlatCAlloc(size_t aNum, size_t aSize)
{
    size_t total_size = aNum * aSize;
    void  *ptr        = pvPortMalloc(total_size);
    if (ptr)
    {
        memset(ptr, 0, total_size);
    }

    return ptr;
}

void otPlatFree(void *aPtr)
{
    vPortFree(aPtr);
}
#endif

#if OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_APP
void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...)
{
    va_list ap;

    va_start(ap, aFormat);
    otCliPlatLogv(aLogLevel, aLogRegion, aFormat, ap);
    va_end(ap);
}
#endif
