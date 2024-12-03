/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*!
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 *
 * \file
 *
 * This file is the app configuration file which is pre included.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_

/*! *********************************************************************************
 * 	Board Configuration
 ********************************************************************************** */
/* Enable HS Clock to support 2Mbps PHY mode setting */
#define gBleUseHSClock2MbpsPhy_c 1

/* By default: disable App Console and Debug console as UART0 and UART1 are used for HCI and SPINEL communication */
#define gUartAppConsole_d 0
#ifndef gUartDebugConsole_d
#define gUartDebugConsole_d 0
#endif

#ifndef gUartHwFlowControl_d
#define gUartHwFlowControl_d 1
#endif

/*! *********************************************************************************
 * 	App Configuration
 ********************************************************************************** */
/*! Number of connections supported by the application */
#ifndef gAppMaxConnections_c
#define gAppMaxConnections_c 2
#endif

/* Use FRO32K instead of XTAL32K in active and power down modes - XTAL32K no longer required */
#ifndef gClkUseFro32K
#define gClkUseFro32K 0
#endif

/* Stress test - Start DMA transfer to simulate Application activity */
#define gStressTesting_d 0

/* Switch CPU clock to 48MHz FRO at startup - 32MHz (FRO or XTAL) default */
#define gPWR_CpuClk_48MHz 1

/*! *********************************************************************************
 * 	Framework Configuration
 ********************************************************************************** */
/* enable NVM to be used as non volatile storage management by the host stack */
#define gAppUseNvm_d 0

/* If set, enables Kmod data saving in PDM (requires PDM library) */
#define gRadioUsePdm_d 1

/* gUsePdm_d is not synonymous to gAppUseNvm_d because PDM is used by Radio driver independantly from NVM */
#define gUsePdm_d (gRadioUsePdm_d)

/* Enable/disables the coexistence between wireless protocols from different ICs */
#ifndef gMWS_UseCoexistence_d
#define gMWS_UseCoexistence_d 0
#endif

/* Enable/disables the coexistence between wireless protocols from different ICs */
#ifndef gMWS_UseOverridableTimings
#ifndef gMWS_UseCoexistence_d
#define gMWS_UseCoexistence_d 0
#endif

#ifndef gMWS_Coex_Model_d
#define gMWS_Coex_Model_d gMWS_Coex_Status_Prio_d
#endif

/* Priority/Status signal shall be set until after T2 sampling time for priority */
#ifndef gMWS_CoexPrioSignalTime_d
#define gMWS_CoexPrioSignalTime_d 15 /* us */
#endif

#ifndef gMWS_CoexConfirmWaitTime_d
#define gMWS_CoexConfirmWaitTime_d 55 /* us */
#endif
#endif

/*Serial manager is used by both HCI and RCP */
#define gSerialManagerMaxInterfaces_c 2

/* Enable/Disable PANIC catch */
#define gUsePanic_c 1

/* Debug only */
#define gLoggingActive_d 0
#define gLoggingWithExtraTs 0

/*! *********************************************************************************
 * 	HKB Definition
 ********************************************************************************** */
#define gSerialMgrRxBufSize_c 320

/* Defines Tx Queue Size for Serial Manager */
#define gSerialMgrTxQueueSize_c 5

/* Defines Size for Serial Manager Task*/
#define gSerialTaskStackSize_c 700

/* Enable/Disable UART usage */
#define gSerialMgrUseUart_c 1

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#define AppPoolsDetails_c                                                                 \
    _block_size_ 32 _number_of_blocks_ 5 _eol_ _block_size_ 64 _number_of_blocks_ 5 _eol_ \
        _block_size_ 128 _number_of_blocks_ 5 _eol_ _block_size_ 512 _number_of_blocks_ 5 _eol_

/* Enables / Disables the precision timers platform component */
#if defined(gMWS_UseCoexistence_d) && (gMWS_UseCoexistence_d)
#define gTimestamp_Enabled_d 1
#else
#define gTimestamp_Enabled_d 0
#endif

/* Enable the SDK OSA component */
#define USE_SDK_OSA 1

/* Enable output on USART1 on pins connected to on-board FTDI */
#define USART1_FTDI

/*! *********************************************************************************
 * 	RTOS Configuration
 ********************************************************************************** */
/* Defines number of OS events used */
#define osNumberOfEvents 6

/*! *********************************************************************************
 * 	BLE Stack Configuration
 ********************************************************************************** */
#define gBleControllerLocalNameSupport 1

#define gUseHciTransportUpward_d 1

/* Enable dual mode operation */
#define gEnableBleInactivityTimeNotify 1

/*! *********************************************************************************
 * 	NVM Module Configuration - gonly radio data are stored in the PDM
 ********************************************************************************** */
#if gUsePdm_d
#define gPdmMemPoolId_c 1
#endif

#if gUsePdm_d
#define gPdmNbSegments 63 /* number of sectors contained in PDM storage */

#define PdmInternalPoolsDetails_c                                                                            \
    _block_size_ 512 _number_of_blocks_ 2 _pool_id_(gPdmMemPoolId_c) _eol_ _block_size_(gPdmNbSegments * 12) \
        _number_of_blocks_ 1 _pool_id_(gPdmMemPoolId_c) _eol_
#else
#define PdmInternalPoolsDetails_c
#endif

/*! *********************************************************************************
 * 	Memory Pools Configuration
 ********************************************************************************** */

#if gUsePdm_d /* Radio drivers uses PDM but no NVM over PDM */
#define PoolsDetails_c AppPoolsDetails_c PdmInternalPoolsDetails_c
#else
#define PoolsDetails_c AppPoolsDetails_c
#endif

#endif /* _APP_PREINCLUDE_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
