/** @file bsp_uart.h
 * @license
 * @description
 */

#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#ifdef __cplusplus
extern "C" {
#endif
//=============================================================================
//                  Constant Definition
//=============================================================================
/* Notice: This define is not hardware register bit setting, so don't be confused with
 * real hardware bit setting.
 */
#define CMSPAR             (1<<2)    /*sticky, hidden defined, not original linux*/
#define PARENB             (1<<1)    /*partity enabled, hidden defined, not original linux */
#define PARODD             (1<<0)    /*partity ODD, hidden defined, not original linux */

#define  UART_BAUDRATE_Baud2400         (1667)
#define  UART_BAUDRATE_Baud4800          (833)
#define  UART_BAUDRATE_Baud9600          (417)
#define  UART_BAUDRATE_Baud14400         (278)
#define  UART_BAUDRATE_Baud19200         (208)
#define  UART_BAUDRATE_Baud28800         (139)
#define  UART_BAUDRATE_Baud38400         (104)
#define  UART_BAUDRATE_Baud57600          (69)
#define  UART_BAUDRATE_Baud76800          (52)
#define  UART_BAUDRATE_Baud115200         (35)
#define  UART_BAUDRATE_Baud500000          (8)
#define  UART_BAUDRATE_Baud1000000         (4)
#define  UART_BAUDRATE_Baud2000000         (2)

typedef void (*uart_isr_cb)(void);


/**
 * @brief UART PARITY Type Definitions
 *
 */
typedef enum
{
    UART_PARITY_NONE  =  0,               /*!< NONE PARITY  \hideinitializer */
    UART_PARITY_EVEN  = (PARENB),         /*!< EVEN PARITY  \hideinitializer */
    UART_PARITY_ODD   = (PARENB | PARODD), /*!< ODD PARITY  \hideinitializer */
    UART_PARITY_SPACE = (CMSPAR | PARENB),        /*!< PARITY SPACE  \hideinitializer */
    UART_PARITY_MARK  = (CMSPAR | PARENB | PARODD) /*!< PARITY MARK     \hideinitializer */
} bsp_uart_parity_t;

/**
 * @brief Types of UART flow control modes.
 *
 */
typedef enum
{
    UART_HWFC_DISABLED = 0,   /*!< HW flow control disabled.  \hideinitializer */
    UART_HWFC_ENABLED         /*!< HW flow control enabled.  \hideinitializer */
} bsp_uart_hwfc_t;


/**
 * @brief Types of UART stop bit modes.
 *
 */
typedef enum
{
    UART_STOPBIT_ONE  = 0,       /*!< 1 STOP BIT  \hideinitializer */
    UART_STOPBIT_TWO             /*!< 2 STOP BIT  \hideinitializer */
} bsp_uart_stopbit_t;

typedef enum
{
    UART_BAUDRATE_2400    = UART_BAUDRATE_Baud2400,    /*!< 2400 baud. \hideinitializer */
    UART_BAUDRATE_4800    = UART_BAUDRATE_Baud4800,    /*!< 4800 baud. \hideinitializer */
    UART_BAUDRATE_9600    = UART_BAUDRATE_Baud9600,    /*!< 9600 baud. \hideinitializer */
    UART_BAUDRATE_14400   = UART_BAUDRATE_Baud14400,   /*!< 14400 baud. \hideinitializer */
    UART_BAUDRATE_19200   = UART_BAUDRATE_Baud19200,   /*!< 19200 baud. \hideinitializer */
    UART_BAUDRATE_28800   = UART_BAUDRATE_Baud28800,   /*!< 28800 baud. \hideinitializer */
    UART_BAUDRATE_38400   = UART_BAUDRATE_Baud38400,   /*!< 38400 baud. \hideinitializer */
    UART_BAUDRATE_57600   = UART_BAUDRATE_Baud57600,   /*!< 57600 baud. \hideinitializer */
    UART_BAUDRATE_76800   = UART_BAUDRATE_Baud76800,   /*!< 76800 baud. \hideinitializer */
    UART_BAUDRATE_115200  = UART_BAUDRATE_Baud115200,  /*!< 115200 baud. \hideinitializer */
    UART_BAUDRATE_500000  = UART_BAUDRATE_Baud500000,  /*!< 500000 baud. \hideinitializer */
    UART_BAUDRATE_1000000 = UART_BAUDRATE_Baud1000000, /*!< 1000000 baud. \hideinitializer */
    UART_BAUDRATE_2000000 = UART_BAUDRATE_Baud2000000  /*!< 2000000 baud. \hideinitializer */
} bsp_uart_baudrate_t;


typedef enum
{
    UART_DATA_BITS_5 = 0,  /*!< 5-bits. \hideinitializer */
    UART_DATA_BITS_6,      /*!< 6-bits. \hideinitializer */
    UART_DATA_BITS_7,      /*!< 7-bits. \hideinitializer */
    UART_DATA_BITS_8       /*!< 8-bits. \hideinitializer */
} bsp_uart_databits_t;
//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct bsp_uart_config
{
    bsp_uart_baudrate_t baud_rate;
    bsp_uart_databits_t word_length;
    bsp_uart_stopbit_t  stop_bits;
    bsp_uart_hwfc_t     hwfc;
    bsp_uart_parity_t   parity;
    uint32_t        irq_priority; // 0: ignore
} bsp_uart_config_t;
//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================
int bsp_uart_drv_init(uint32_t target_id, bsp_uart_config_t *pConfig, uart_isr_cb isr_cb);
int bsp_uart_drv_recv(uint32_t target_id, uint8_t *p_data, uint32_t *pLength);
int bsp_uart_drv_send(uint32_t target_id, uint8_t *p_data, uint32_t length);
#ifdef __cplusplus
}
#endif

#endif
