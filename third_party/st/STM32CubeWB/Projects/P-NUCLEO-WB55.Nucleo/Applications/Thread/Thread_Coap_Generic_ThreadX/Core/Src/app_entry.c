/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : app_entry.c
  * Description        : Entry application source file for STM32WPAN Middleware.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "main.h"
#include "app_entry.h"
#include "app_thread.h"
#include "app_conf.h"
#include "hw_conf.h"
#include "tx_api.h"
#include "stm_logging.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "dbg_trace.h"
#include "shci.h"
#include "otp.h"
#include "advanced_memory_manager.h"
#include "stm32_mm.h"
#include "tx_api.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
typedef __PACKED_STRUCT
{
  uint32_t *next;
  uint32_t *prev;
} TraceEltHeader_t;
typedef __PACKED_STRUCT
{
  uint8_t   buffer[255];
  uint32_t  size;
} TraceElt_t;
typedef struct __attribute__((packed, aligned(4)))
{
  TraceEltHeader_t header;
  TraceElt_t trace;
} TraceEltPacket_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* POOL_SIZE = 2(TL_PacketHeader_t) + 258 (3(TL_EVT_HDR_SIZE) + 255(Payload size)) */
#define POOL_SIZE (CFG_TL_EVT_QUEUE_LENGTH * 4U * DIVC((sizeof(TL_PacketHeader_t) + TL_EVENT_FRAME_SIZE), 4U))
#define POOL_TRACE_SIZE  CFG_AMM_VIRTUAL_APP_TRACE_BUFFER_SIZE + CFG_AMM_VIRTUAL_MEMORY_NUMBER * AMM_VIRTUAL_INFO_ELEMENT_SIZE
#define TX_APP_MEM_POOL_SIZE                     (1024*24)

#define AMM_BCKGND_TASK_STACK_SIZE    (256*7)
#define AMM_BCKGND_TASK_PRIO          (9)
#define AMM_BCKGND_TASK_PREEM_TRES    (0)
#define TRC_BCKGND_TASK_STACK_SIZE    (256*7)
#define TRC_BCKGND_TASK_PRIO          (9)
#define TRC_BCKGND_TASK_PREEM_TRES    (0)
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
uint8_t g_ot_notification_allowed = 0U;
static tListNode TraceBufferList;
static uint32_t TracePool[POOL_TRACE_SIZE];
TX_THREAD TRC_BCKGND_Thread;
TX_SEMAPHORE TRC_BCKGND_Thread_Sem;
TX_THREAD AMM_BCKGND_Thread;
TX_SEMAPHORE AMM_BCKGND_Thread_Sem;

static AMM_VirtualMemoryConfig_t vmConfig[CFG_AMM_VIRTUAL_MEMORY_NUMBER] = {
  /* Virtual Memory #1 */
  { 
    .Id = CFG_AMM_VIRTUAL_APP_TRACE,  
    .BufferSize = CFG_AMM_VIRTUAL_APP_TRACE_BUFFER_SIZE
  }
};

static AMM_InitParameters_t ammInitConfig =
{
  .p_PoolAddr = TracePool,
  .PoolSize = POOL_TRACE_SIZE,
  .VirtualMemoryNumber = CFG_AMM_VIRTUAL_MEMORY_NUMBER,
  .p_VirtualMemoryConfigList = vmConfig
};

CHAR * pStack;

/* USER CODE BEGIN PV */
static UCHAR memory_area[DEMO_BYTE_POOL_SIZE];
/* USER CODE END PV */

/* Global variables ----------------------------------------------------------*/
CHAR                * pointer = TX_NULL;
TX_MUTEX            MtxShciId;
TX_SEMAPHORE        SemShciId;
TX_SEMAPHORE        SemShciNotify;
TX_THREAD           ShciUserEvtProcessId;
static TX_BYTE_POOL byte_pool_shci;

/* Global function prototypes -----------------------------------------------*/
#if (CFG_DEBUG_TRACE != 0)
size_t DbgTraceWrite(int handle, const unsigned char * buf, size_t bufSize);
#endif /* CFG_DEBUG_TRACE != 0 */

/* USER CODE BEGIN GFP */

/* USER CODE END GFP */

/* Private functions prototypes-----------------------------------------------*/
static void ShciUserEvtProcess      ( ULONG thread_input );
static void Config_HSE              ( void );
static void Reset_Device            ( void );
#if ( CFG_HW_RESET_BY_FW == 1 )
static void Reset_IPCC              ( void );
static void Reset_BackupDomain      ( void );
#endif /* CFG_HW_RESET_BY_FW == 1*/
static void System_Init             ( void );
static void SystemPower_Config      ( void );
static void Init_Debug              ( void );
static void appe_Tl_Init            ( void );
static void APPE_SysStatusNot       ( SHCI_TL_CmdStatus_t status );
static void APPE_SysUserEvtRx       ( void * pPayload );
static void APPE_SysEvtReadyProcessing  ( void );
static void APPE_SysEvtError        ( SCHI_SystemErrCode_t ErrorCode );

#if (CFG_HW_LPUART1_ENABLED == 1)
extern void MX_LPUART1_UART_Init    ( void );
#endif /* CFG_HW_LPUART1_ENABLED == 1 */
#if (CFG_HW_USART1_ENABLED == 1)
extern void MX_USART1_UART_Init     ( void );
#endif /* CFG_HW_USART1_ENABLED == 1 */
static void Init_Rtc                ( void );

/* USER CODE BEGIN PFP */
static void Led_Init                ( void );
static void Button_Init             ( void );

/* Section specific to button management using UART */
static void RxUART_Init(void);
static void RxCpltCallback(void);
static void UartCmdExecute(void);

#define C_SIZE_CMD_STRING       256U
#define RX_BUFFER_SIZE          8U

static uint8_t aRxBuffer[RX_BUFFER_SIZE];
static uint8_t CommandString[C_SIZE_CMD_STRING];
static uint16_t indexReceiveChar = 0;
EXTI_HandleTypeDef exti_handle;
#if(CFG_DEBUG_TRACE != 0)
static void writeTrace(char * buffer, uint32_t size);
#endif
static void TRC_BackgroundProcess(void);
static void AMM_WrapperInit (uint32_t * const p_PoolAddr, const uint32_t PoolSize);
static uint32_t * AMM_WrapperAllocate (const uint32_t BufferSize);
static void AMM_WrapperFree (uint32_t * const p_BufferAddr);
static void AMM_BackgroundProcess_Entry(unsigned long thread_input);
static void TRC_BackgroundProcess_Entry(unsigned long thread_input);

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void MX_APPE_Config( void )
{
  /**
   * The OPTVERR flag is wrongly set at power on
   * It shall be cleared before using any HAL_FLASH_xxx() api
   */
  __HAL_FLASH_CLEAR_FLAG( FLASH_FLAG_OPTVERR );

  /**
   * Reset some configurations so that the system behave in the same way
   * when either out of nReset or Power On
   */
  Reset_Device();

  /* Configure HSE Tuning */
  Config_HSE();

  return;
}

void MX_APPE_Init( void )
{
  System_Init();       /**< System initialization */

  SystemPower_Config(); /**< Configure the system Power Mode */
  
  AMM_Init (&ammInitConfig);
  
  LST_init_head (&TraceBufferList); 
  
  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */

  /* USER CODE BEGIN APPE_Init_1 */
  Init_Debug();

#ifdef TX_LOW_POWER
  /**
   * The Standby mode should not be entered before the initialization is over
   * The default state of the Low Power Manager is to allow the Standby Mode so an request is needed here
   */
    
  //UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
  APP_THREAD_ThreadX_LowPowerEnable( LOWPOWER_NONE );
#endif // TX_LOW_POWER
    
  Led_Init();
  Button_Init();
  RxUART_Init();
  /* USER CODE END APPE_Init_1 */
  appe_Tl_Init();	/* Initialize all transport layers */

  /**
   * From now, the application is waiting for the ready event ( VS_HCI_C2_Ready )
   * received on the system channel before starting the Stack
   * This system event is received with APPE_SysUserEvtRx()
   */
/* USER CODE BEGIN APPE_Init_2 */

/* USER CODE END APPE_Init_2 */
   return;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

void tx_application_define(void* first_unused_memory)
{
  UINT ThreadXStatus = TX_THREAD_ERROR;

  /* Here we should declare all the initial ThreadX resources
   * We should have at least one thread to be launched from the scheduler
   * */

  /* Create a byte memory pool from which to allocate the thread stacks, using static memory
   * coming from memory_area array  */
  tx_byte_pool_create(&byte_pool_shci, "byte pool 0", memory_area, DEMO_BYTE_POOL_SIZE);

  tx_mutex_create(&MtxShciId, "MtxShciId", TX_NO_INHERIT);
  tx_semaphore_create(&SemShciId, "SemShciId", 0);              /*< Create the semaphore and make it busy at initialization */
  tx_semaphore_create(&SemShciNotify, "SemShciNotify", 0);      /*< Create the semaphore and make it busy at initialization */

  tx_byte_allocate(&byte_pool_shci, (VOID**) &pointer, DEMO_STACK_SIZE_LARGE, TX_NO_WAIT);
  ThreadXStatus = tx_thread_create(&ShciUserEvtProcessId,
                   	   	   	   	   "ShciUserEvtProcessId",
								   ShciUserEvtProcess,
								   0,
								   pointer,
								   DEMO_STACK_SIZE_LARGE,
								   SHCI_USER_EVT_PROCESS_PRIORITY,
								   SHCI_USER_EVT_PROCESS_PRIORITY,
								   TX_NO_TIME_SLICE,
								   TX_AUTO_START);
  
  if (tx_byte_allocate(&byte_pool_shci, (void **) &pointer, AMM_BCKGND_TASK_STACK_SIZE,TX_NO_WAIT) != TX_SUCCESS)
  {
    Error_Handler();
  }
  
  if (tx_semaphore_create(&AMM_BCKGND_Thread_Sem, "AMM_BCKGND_Thread_Sem", 0)!= TX_SUCCESS )
  {
    Error_Handler();
  }
  
  if (tx_thread_create(&AMM_BCKGND_Thread, "AMM_BCKGND Thread", AMM_BackgroundProcess_Entry, 0,
                         pointer, AMM_BCKGND_TASK_STACK_SIZE,
                         AMM_BCKGND_TASK_PRIO, AMM_BCKGND_TASK_PREEM_TRES,
                         TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    Error_Handler();
  }
  
  if (tx_byte_allocate(&byte_pool_shci, (void **) &pointer, TRC_BCKGND_TASK_STACK_SIZE,TX_NO_WAIT) != TX_SUCCESS)
  {
    Error_Handler();
  }
  if (tx_semaphore_create(&TRC_BCKGND_Thread_Sem, "TRC_BCKGND_Thread_Sem", 0)!= TX_SUCCESS )
  {
    Error_Handler();
  }
  if (tx_thread_create(&TRC_BCKGND_Thread, "TRC_BCKGND Thread", TRC_BackgroundProcess_Entry, 0,
                        pointer, TRC_BCKGND_TASK_STACK_SIZE,
                        TRC_BCKGND_TASK_PRIO, TRC_BCKGND_TASK_PREEM_TRES,
                         TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    Error_Handler();
  }

  if (ThreadXStatus != TX_SUCCESS)
    { APP_THREAD_Error( ERR_THREAD_THREAD_X_FAILED, 1 ); }
}

void Init_Smps( void )
{
#if (CFG_USE_SMPS != 0)
  /**
   *  Configure and enable SMPS
   *
   *  The SMPS configuration is not yet supported by CubeMx
   *  when SMPS output voltage is set to 1.4V, the RF output power is limited to 3.7dBm
   *  the SMPS output voltage shall be increased for higher RF output power
   */
  LL_PWR_SMPS_SetStartupCurrent(LL_PWR_SMPS_STARTUP_CURRENT_80MA);
  LL_PWR_SMPS_SetOutputVoltageLevel(LL_PWR_SMPS_OUTPUT_VOLTAGE_1V40);
  LL_PWR_SMPS_Enable();
#endif /* CFG_USE_SMPS != 0 */

  return;
}

void Init_Exti( void )
{
  /* Enable IPCC(36), HSEM(38) wakeup interrupts on CPU1 */
  LL_EXTI_EnableIT_32_63( LL_EXTI_LINE_36 | LL_EXTI_LINE_38 );
  
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void Init_Debug( void )
{
#if (CFG_DEBUGGER_SUPPORTED == 1)
  /**
   * Keep debugger enabled while in any low power mode
   */
  HAL_DBGMCU_EnableDBGSleepMode();

  /***************** ENABLE DEBUGGER *************************************/
  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_48);
  LL_C2_EXTI_EnableIT_32_63(LL_EXTI_LINE_48);

#else

  GPIO_InitTypeDef gpio_config = {0};

  gpio_config.Pull = GPIO_NOPULL;
  gpio_config.Mode = GPIO_MODE_ANALOG;

  gpio_config.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_Init(GPIOA, &gpio_config);
  __HAL_RCC_GPIOA_CLK_DISABLE();

  gpio_config.Pin = GPIO_PIN_4 | GPIO_PIN_3;
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_Init(GPIOB, &gpio_config);
  __HAL_RCC_GPIOB_CLK_DISABLE();

  HAL_DBGMCU_DisableDBGSleepMode();
  HAL_DBGMCU_DisableDBGStopMode();
  HAL_DBGMCU_DisableDBGStandbyMode();

#endif /* (CFG_DEBUGGER_SUPPORTED == 1) */

#if (CFG_DEBUG_TRACE != 0)
  DbgTraceInit();
#endif /* CFG_DEBUG_TRACE != 0 */

  return;
}


static void Reset_Device( void )
{
#if ( CFG_HW_RESET_BY_FW == 1 )
  Reset_BackupDomain();

  Reset_IPCC();
#endif /* CFG_HW_RESET_BY_FW == 1 */

  return;
}

#if ( CFG_HW_RESET_BY_FW == 1 )
static void Reset_BackupDomain( void )
{
  if ((LL_RCC_IsActiveFlag_PINRST() != FALSE) && (LL_RCC_IsActiveFlag_SFTRST() == FALSE))
  {
    HAL_PWR_EnableBkUpAccess(); /**< Enable access to the RTC registers */

    /**
     *  Write twice the value to flush the APB-AHB bridge
     *  This bit shall be written in the register before writing the next one
     */
    HAL_PWR_EnableBkUpAccess();

    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();
  }

  return;
}

static void Reset_IPCC( void )
{
  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_IPCC);

  LL_C1_IPCC_ClearFlag_CHx(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C2_IPCC_ClearFlag_CHx(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C1_IPCC_DisableTransmitChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C2_IPCC_DisableTransmitChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C1_IPCC_DisableReceiveChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C2_IPCC_DisableReceiveChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  return;
}
#endif /* CFG_HW_RESET_BY_FW == 1 */

static void Config_HSE(void)
{
    OTP_ID0_t * p_otp;

  /**
   * Read HSE_Tuning from OTP
   */
  p_otp = (OTP_ID0_t *) OTP_Read(0);
  if (p_otp)
  {
    LL_RCC_HSE_SetCapacitorTuning(p_otp->hse_tuning);
  }

  return;
}

static void System_Init( void )
{
  Init_Smps();

  Init_Exti();

  Init_Rtc();

  return;
}

static void Init_Rtc( void )
{
  /* Disable RTC registers write protection */
  LL_RTC_DisableWriteProtection(RTC);

  LL_RTC_WAKEUP_SetClock(RTC, CFG_RTC_WUCKSEL_DIVIDER);

  /* Enable RTC registers write protection */
  LL_RTC_EnableWriteProtection(RTC);

  return;
}

static void AMM_WrapperInit (uint32_t * const p_PoolAddr, const uint32_t PoolSize)
{
  UTIL_MM_Init ((uint8_t *)p_PoolAddr, ((size_t)PoolSize * sizeof(uint32_t)));
}
static uint32_t * AMM_WrapperAllocate (const uint32_t BufferSize)
{
  return (uint32_t *)UTIL_MM_GetBuffer (((size_t)BufferSize * sizeof(uint32_t)));
}
static void AMM_WrapperFree (uint32_t * const p_BufferAddr)
{
  UTIL_MM_ReleaseBuffer ((void *)p_BufferAddr);
}
/**
 * @brief  Configure the system for power optimization
 *
 * @note  This API configures the system to be ready for low power mode
 *
 * @param  None
 * @retval None
 */
static void SystemPower_Config(void)
{
  /**
   * Select HSI as system clock source after Wake Up from Stop mode
   */
  LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);

  /* Initialize low power manager */
  UTIL_LPM_Init();

  /* Initialize the CPU2 reset value before starting CPU2 with C2BOOT */
  LL_C2_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);

#if (CFG_USB_INTERFACE_ENABLE != 0)
  /**
   *  Enable USB power
   */
  HAL_PWREx_EnableVddUSB();
#endif /* CFG_USB_INTERFACE_ENABLE != 0 */

  return;
}

static void appe_Tl_Init( void )
{
  TL_MM_Config_t tl_mm_config;
  SHCI_TL_HciInitConf_t SHci_Tl_Init_Conf;

  /**< Reference table initialization */
  TL_Init();

  /**< System channel initialization */
  SHci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&SystemCmdBuffer;
  SHci_Tl_Init_Conf.StatusNotCallBack = APPE_SysStatusNot;
  shci_init(APPE_SysUserEvtRx, (void*) &SHci_Tl_Init_Conf);

  /**< Memory Manager channel initialization */
  tl_mm_config.p_BleSpareEvtBuffer = 0;
  tl_mm_config.p_SystemSpareEvtBuffer = SystemSpareEvtBuffer;
  tl_mm_config.p_AsynchEvtPool = EvtPool;
  tl_mm_config.AsynchEvtPoolSize = POOL_SIZE;
  TL_MM_Init( &tl_mm_config );

  TL_Enable();

  return;
}

static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status )
{
  switch (status)
  {
    case SHCI_TL_CmdBusy:
      tx_mutex_get(&MtxShciId, TX_WAIT_FOREVER  );
      break;

    case SHCI_TL_CmdAvailable:
      tx_mutex_put(&MtxShciId );
      break;

    default:
      break;
  }
  return;
}

/**
 * The type of the payload for a system user event is tSHCI_UserEvtRxParam
 * When the system event is both :
 *    - a ready event (subevtcode = SHCI_SUB_EVT_CODE_READY)
 *    - reported by the FUS (sysevt_ready_rsp == FUS_FW_RUNNING)
 * The buffer shall not be released
 * (eg ((tSHCI_UserEvtRxParam*)pPayload)->status shall be set to SHCI_TL_UserEventFlow_Disable)
 * When the status is not filled, the buffer is released by default
 */
static void APPE_SysUserEvtRx( void * pPayload )
{
  TL_AsynchEvt_t *p_sys_event;
  p_sys_event = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pPayload)->pckt->evtserial.evt.payload);

  switch(p_sys_event->subevtcode)
  {
     case SHCI_SUB_EVT_CODE_READY:
         APPE_SysEvtReadyProcessing();
         break;

     case SHCI_SUB_EVT_ERROR_NOTIF:
         APPE_SysEvtError((SCHI_SystemErrCode_t) (p_sys_event->payload[0]));
         break;

     default:
         break;
  }
  return;
}

/**
 * @brief Notify a system error coming from the M0 firmware
 * @param  ErrorCode  : errorCode detected by the M0 firmware
 *
 * @retval None
 */
static void APPE_SysEvtError( SCHI_SystemErrCode_t ErrorCode )
{
  switch(ErrorCode)
  {
  	case ERR_THREAD_LLD_FATAL_ERROR:
  		APP_DBG("** ERR_THREAD : LLD_FATAL_ERROR\n");
  		break;

  	case ERR_THREAD_UNKNOWN_CMD:
  		APP_DBG("** ERR_THREAD : UNKNOWN_CMD\n");
  		break;

  	default:
  		APP_THREAD_TraceError("** ERR_THREAD : ErroCode=%d \n", ErrorCode);
  		break;
  }
  return;
}

static void APPE_SysEvtReadyProcessing( void )
{
  /* Traces channel initialization */
  TL_TRACES_Init();

  APP_THREAD_Init(&byte_pool_shci);

#ifdef TX_LOW_POWER
  //UTIL_LPM_SetOffMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
  APP_THREAD_ThreadX_LowPowerEnable( LOWPOWER_SLEEPMODE );
#endif // TX_LOW_POWER
  return;
}

/*************************************************************
 *
 * ThreadX WRAPPER FUNCTIONS
 *
*************************************************************/
static void ShciUserEvtProcess(ULONG argument)
{
  UNUSED(argument);

  for(;;)
  {
    /* USER CODE BEGIN SHCI_USER_EVT_PROCESS_1 */

    /* USER CODE END SHCI_USER_EVT_PROCESS_1 */
     tx_semaphore_get(&SemShciNotify, TX_WAIT_FOREVER);
     shci_user_evt_proc();
    /* USER CODE BEGIN SHCI_USER_EVT_PROCESS_2 */

    /* USER CODE END SHCI_USER_EVT_PROCESS_2 */
  }
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */
static void Led_Init( void )
{
#if (CFG_LED_SUPPORTED == 1U)
  /**
   * Leds Initialization
   */

  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

#endif

  return;
}

static void Button_Init( void )
{
#if (CFG_BUTTON_SUPPORTED == 1U)
  /**
   * Button Initialization
   */
  BSP_PB_Init(BUTTON_SW1, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_SW2, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_SW3, BUTTON_MODE_EXTI);
  
  HAL_NVIC_SetPriority((IRQn_Type)BUTTON_SW1_EXTI_IRQn, 0x0E, 0x00);
  HAL_NVIC_SetPriority((IRQn_Type)BUTTON_SW2_EXTI_IRQn, 0x0E, 0x00);
  HAL_NVIC_SetPriority((IRQn_Type)BUTTON_SW3_EXTI_IRQn, 0x0E, 0x00);
#endif

  return;
}

/* USER CODE END FD_LOCAL_FUNCTIONS */

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/
void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY)
  {
    wait += HAL_GetTickFreq();
  }

  while ((HAL_GetTick() - tickstart) < wait)
  {
    /************************************************************************************
     * ENTER SLEEP MODE
     ***********************************************************************************/
    LL_LPM_EnableSleep(); /**< Clear SLEEPDEEP bit of Cortex System Control Register */

    /**
     * This option is used to ensure that store operations are completed
     */
  #if defined ( __CC_ARM )
    __force_stores();
  #endif /* __CC_ARM */

    __WFI();
  }
}

void AMM_RegisterBasicMemoryManager (AMM_BasicMemoryManagerFunctions_t * const p_BasicMemoryManagerFunctions)
{
  p_BasicMemoryManagerFunctions->Init = AMM_WrapperInit;
  p_BasicMemoryManagerFunctions->Allocate = AMM_WrapperAllocate;
  p_BasicMemoryManagerFunctions->Free = AMM_WrapperFree;
}
void AMM_ProcessRequest (void)
{
  tx_semaphore_put(&AMM_BCKGND_Thread_Sem);
}
void AMM_BackgroundProcess_Entry(unsigned long thread_input)
{
  (void)(thread_input);
  while(1)
  {
    tx_semaphore_get(&AMM_BCKGND_Thread_Sem, TX_WAIT_FOREVER);
    AMM_BackgroundProcess();
  }
}
void TRC_BackgroundProcess(void)
{
  TraceEltPacket_t * traceElt = NULL;
  while (LST_is_empty (&TraceBufferList) == FALSE)
  {
    LST_remove_tail (&TraceBufferList, (tListNode**)&traceElt);
    if (traceElt != NULL)
    {
#if(CFG_DEBUG_TRACE != 0)
      DbgTraceWrite(1U, (const unsigned char *) traceElt->trace.buffer, traceElt->trace.size);
#endif /* CFG_DEBUG_TRACE */
      AMM_Free((uint32_t *)traceElt);
    }
  }
}
void TRC_BackgroundProcess_Entry(unsigned long thread_input)
{
  (void)(thread_input);
  while(1)
  {
    tx_semaphore_get(&TRC_BCKGND_Thread_Sem, TX_WAIT_FOREVER);
    TRC_BackgroundProcess();
  }
}
void shci_notify_asynch_evt(void* pdata)
{
  UNUSED(pdata);
  tx_semaphore_put(&SemShciNotify);
  return;
}

void shci_cmd_resp_release(uint32_t flag)
{
  UNUSED(flag);
  tx_semaphore_put(&SemShciId);
  return;
}

void shci_cmd_resp_wait(uint32_t timeout)
{
  UNUSED(timeout);
  tx_semaphore_get(&SemShciId, TX_WAIT_FOREVER);
  return;
}

/* Received trace buffer from M0 */
void TL_TRACES_EvtReceived(TL_EvtPacket_t * hcievt)
{
#if (CFG_DEBUG_TRACE != 0)
  /* Call write/print function using DMA from dbg_trace */
  /* - Cast to TL_AsynchEvt_t* to get "real" payload (without Sub Evt code 2bytes),
     - (-2) to size to remove Sub Evt Code */
  writeTrace((char *) ((TL_AsynchEvt_t *)(hcievt->evtserial.evt.payload))->payload, hcievt->evtserial.evt.plen - 2U);
#endif /* CFG_DEBUG_TRACE != 0 */
  /* Release buffer */
  TL_MM_EvtDone(hcievt);
}

void writeTrace(char * buffer, uint32_t size)
{
  TraceEltPacket_t * traceElt = NULL;
  if(AMM_ERROR_OK == AMM_Alloc (CFG_AMM_VIRTUAL_APP_TRACE, DIVC(sizeof(TraceEltPacket_t), sizeof(uint32_t)), (uint32_t **)&traceElt, NULL))
  {
    if(traceElt != NULL)
    {
      memcpy(traceElt->trace.buffer, (const unsigned char *) buffer, size);
      traceElt->trace.size = size;
      LST_insert_head (&TraceBufferList, (tListNode *)traceElt);
      tx_semaphore_put(&TRC_BCKGND_Thread_Sem);
    }
  }
}
/**
  * @brief  Initialisation of the trace mechanism
  * @param  None
  * @retval None
  */
#if (CFG_DEBUG_TRACE != 0)
void DbgOutputInit(void)
{
#ifdef CFG_DEBUG_TRACE_UART
  MX_USART1_UART_Init();
  return;
#endif /* CFG_DEBUG_TRACE_UART */
}

/**
  * @brief  Management of the traces
  * @param  p_data : data
  * @param  size : size
  * @param  call-back :
  * @retval None
  */
void DbgOutputTraces(uint8_t *p_data, uint16_t size, void (*cb)(void))
{
  HW_UART_Transmit_DMA(CFG_DEBUG_TRACE_UART, p_data, size, cb);

  return;
}


#endif /* CFG_DEBUG_TRACE != 0 */

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */
/**
  * @brief This function manage the Push button action
  * @param  GPIO_Pin : GPIO pin which has been activated
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  switch(GPIO_Pin)
  {
    case BUTTON_SW1_PIN:
        APP_THREAD_SW1_Task();
        break;
        
    case BUTTON_SW2_PIN:
        APP_THREAD_SW2_Task();
        break;
        
    case BUTTON_SW3_PIN: 
        break;
        
    default:
        break;
  }
}

static void RxUART_Init(void)
{
  HW_UART_Receive_IT(CFG_DEBUG_TRACE_UART, aRxBuffer, 1U, RxCpltCallback);
}

static void RxCpltCallback(void)
{
  char buffer[255];
  uint32_t sizeReceived = 9;
#ifdef TX_LOW_POWER
  APP_THREAD_ThreadX_LowPowerEnable(0);
#endif // TX_LOW_POWER
  /* Filling buffer and wait for '\r' char */
  if (indexReceiveChar < C_SIZE_CMD_STRING)
  {
    if (aRxBuffer[0] == '\r')
    {
      strcpy(buffer, "received ");
      strcat(buffer, (const char *)CommandString);
      buffer[sizeReceived + indexReceiveChar] = '\n';
      writeTrace(buffer, sizeReceived + indexReceiveChar + 1);
#ifdef TX_LOW_POWER        
      APP_THREAD_ThreadX_LowPowerEnable(1);
#endif // TX_LOW_POWER
      UartCmdExecute();

      /* Clear receive buffer and character counter*/
      indexReceiveChar = 0;
      memset(CommandString, 0, C_SIZE_CMD_STRING);
    }
    else
    {
      CommandString[indexReceiveChar++] = aRxBuffer[0];
    }
  }

  /* Once a character has been sent, put back the device in reception mode */
  HW_UART_Receive_IT(CFG_DEBUG_TRACE_UART, aRxBuffer, 1U, RxCpltCallback);
}

static void UartCmdExecute(void)
{
  uint8_t size = 7; /* size of "SWX OK\n" msg*/
  /* Parse received CommandString */
  if(strcmp((char const*)CommandString, "SW1") == 0)
  {
    writeTrace("SW1 OK\n", size);
    exti_handle.Line = EXTI_LINE_4;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if (strcmp((char const*)CommandString, "SW2") == 0)
  {
    writeTrace("SW2 OK\n", size);
    exti_handle.Line = EXTI_LINE_0;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if (strcmp((char const*)CommandString, "SW3") == 0)
  {
    writeTrace("SW3 OK\n", size);
    exti_handle.Line = EXTI_LINE_1;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else
  {
    APP_DBG("NOT RECOGNIZED COMMAND : %s", CommandString);
  }
}
/* USER CODE END FD_WRAP_FUNCTIONS */
