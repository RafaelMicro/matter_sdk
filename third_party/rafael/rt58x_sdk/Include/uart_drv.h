
/**************************************************************************//**
 * @file     uart_drv.h
 * @version
 * @brief    uart driver API
 *
 * @copyright
 ******************************************************************************/
/** @defgroup    UART_Driver   UART
 *  @ingroup     peripheral_group
 *  @brief  UART driver header file
 *  @{
 *  UART 16550 with driver with special local dma setting driver.
*/
#ifndef ___UART_DRV_H__
#define ___UART_DRV_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
/* Notice: This define is not hardware register bit setting, so don't be confused with
 * real hardware bit setting.
 */
#define CMSPAR             (1<<2)    /*sticky, hidden defined, not original linux*/
#define PARENB             (1<<1)    /*partity enabled, hidden defined, not original linux */
#define PARODD             (1<<0)    /*partity ODD, hidden defined, not original linux */

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 * @brief UART PARITY Type Definitions
 *
 */
typedef enum
{
    UART_PARITY_NONE  =  0,                     /*!< NONE PARITY  */
    UART_PARITY_EVEN  = (PARENB),               /*!< EVEN PARITY   */
    UART_PARITY_ODD   = (PARENB | PARODD),      /*!< ODD PARITY   */
    UART_PARITY_SPACE = (CMSPAR | PARENB),      /*!< PARITY SPACE  */
    UART_PARITY_MARK  = (CMSPAR | PARENB | PARODD) /*!< PARITY MARK     */
} uart_parity_t;

/**
 * @brief Types of UART flow control modes.
 *
 */
typedef enum
{
    UART_HWFC_DISABLED = 0,   /*!< HW flow control disabled.   */
    UART_HWFC_ENABLED         /*!< HW flow control enabled.    */
} uart_hwfc_t;


/**
 * @brief Types of UART stop bit modes.
 *
 */
typedef enum
{
    UART_STOPBIT_ONE  = 0,       /*!< 1 STOP BIT  */
    UART_STOPBIT_TWO             /*!< 2 STOP BIT  */
} uart_stopbit_t;

/**
 * @brief Types of UART baudrate modes.
 *
 */
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

/**
 * @brief Baudrates supported by UART.
 *
 */
typedef enum
{
    UART_BAUDRATE_2400    = UART_BAUDRATE_Baud2400,    /*!< 2400 baud.  */
    UART_BAUDRATE_4800    = UART_BAUDRATE_Baud4800,    /*!< 4800 baud.  */
    UART_BAUDRATE_9600    = UART_BAUDRATE_Baud9600,    /*!< 9600 baud.  */
    UART_BAUDRATE_14400   = UART_BAUDRATE_Baud14400,   /*!< 14400 baud. */
    UART_BAUDRATE_19200   = UART_BAUDRATE_Baud19200,   /*!< 19200 baud. */
    UART_BAUDRATE_28800   = UART_BAUDRATE_Baud28800,   /*!< 28800 baud. */
    UART_BAUDRATE_38400   = UART_BAUDRATE_Baud38400,   /*!< 38400 baud. */
    UART_BAUDRATE_57600   = UART_BAUDRATE_Baud57600,   /*!< 57600 baud.  */
    UART_BAUDRATE_76800   = UART_BAUDRATE_Baud76800,   /*!< 76800 baud.  */
    UART_BAUDRATE_115200  = UART_BAUDRATE_Baud115200,  /*!< 115200 baud. */
    UART_BAUDRATE_500000  = UART_BAUDRATE_Baud500000,  /*!< 500000 baud. */
    UART_BAUDRATE_1000000 = UART_BAUDRATE_Baud1000000, /*!< 1000000 baud. */
    UART_BAUDRATE_2000000 = UART_BAUDRATE_Baud2000000  /*!< 2000000 baud. */
} uart_baudrate_t;

/**
 * @brief Data bits for one character supported by UART.
 *
 */
typedef enum
{
    UART_DATA_BITS_5 = 0,  /*!< 5-bits.  */
    UART_DATA_BITS_6,      /*!< 6-bits.  */
    UART_DATA_BITS_7,      /*!< 7-bits.  */
    UART_DATA_BITS_8       /*!< 8-bits.  */
} uart_databits_t;


/**
* @brief UART EVENT
*
* @details UART_EVENT_RX_TIMEOUT is not error, it just signal the event
*        "during receive when idle time is detected between consecutive characters."
*/

#define UART_EVENT_TX_DONE             (1UL << 0)  ///< Send completed; however UART may still transmit data
#define UART_EVENT_RX_DONE             (1UL << 1)  ///< Receive completed
#define UART_EVENT_RX_OVERFLOW         (1UL << 2)  ///< Receive data overflow
#define UART_EVENT_RX_TIMEOUT          (1UL << 3)  ///< Receive character timeout (optional)
#define UART_EVENT_RX_BREAK            (1UL << 4)  ///< Break detected on receive
#define UART_EVENT_RX_FRAMING_ERROR    (1UL << 5)  ///< Framing error detected on receive
#define UART_EVENT_RX_PARITY_ERROR     (1UL << 6)  ///< Parity error detected on receive
#define UART_EVENT_CTS                 (1UL << 7)  ///< CTS state changed (optional)

#define UART_EVENT_RX_RECV             (1UL << 8)  ///< Receive data 

#define UART_POWER_OFF                 (0)         ///< Power off the uart device
#define UART_POWER_ON                  (1)         ///< Power on the uart device


/**
 * @Brief Structure for UART status.
 */
typedef struct _uart_status
{
    uint32_t tx_busy          : 1;        /*!< Transmitter busy flag  */
    uint32_t rx_busy          : 1;        /*!< Receiver busy flag     */
    uint32_t tx_underflow     : 1;        /*!< Transmit data underflow detected (cleared on start of next send operation) */
    uint32_t rx_overflow      : 1;        /*!< Receive data overflow detected (cleared on start of next receive operation */
    uint32_t rx_break         : 1;        /*!< Break detected on receive (cleared on start of next receive operation)     */
    uint32_t rx_framing_error : 1;        /*!< Framing error detected on receive (cleared on start of next receive operation)  */
    uint32_t rx_parity_error  : 1;        /*!< Parity error detected on receive (cleared on start of next receive operation)   */
    uint32_t reserved0        : 1;
    uint32_t msr_cts_state    : 1;        /*!< modem CTS status  */
    uint32_t reserved1        : 23;
} uart_status;


/**
 * @Brief Structure for UART configuration.
 *
 */
typedef struct
{
    void               *p_context;          /*!< Context passed to interrupt handler.  */
    uart_baudrate_t     baudrate;           /*!< Baudrate configuration.  */
    uart_databits_t     databits;           /*!< Data bits for one character configuration.  */
    uart_stopbit_t      stopbit;            /*!< Stopbit configuration.  */
    uart_parity_t       parity;             /*!< Parity configuration.   */
    uart_hwfc_t         hwfc;               /*!< Flow control configuration.  */
    uint8_t             interrupt_priority; /*!< Interrupt priority.          */
} uart_config_t;


/**
 * @brief UART interrupt event handler.
 *
 * @param[in] event      Signal for UART Event.
 *
 * @param[in] p_context  Context passed to interrupt handler, set on initialization.
 */
typedef void (*uart_event_handler_t)(uint32_t event,
                                     void *p_context);


/**************************************************************************************************
*    GLOBAL PROTOTYPES
*************************************************************************************************/
/**
 * @brief Function for initializing the UART driver.
 *
 *  This function configures and enables UART.
 *  Call this function will auto power-on the uart device.
 *
 * @param[in] uart_id       identifier for uart device, 0 for uart0, 1 for uart1
 * @param[in] p_config      Pointer to the structure with initial configuration.
 * @param[in] event_handler Event handler provided by the user. If not provided driver works in
 *                          blocking mode.
 *
 * @retval    STATUS_SUCCESS           If initialization was successful.
 * @retval    STATUS_INVALID_REQUEST   If driver is already initialized.
 * @retval    STATUS_INVALID_PARAM     Wrong parameter, for example uart_id > maximum real uart device number
 *                                      or p_config is NULL.
 */
uint32_t uart_init(uint32_t uart_id,
                   uart_config_t const *p_config,
                   uart_event_handler_t  event_handler);



/**
* @brief Function for uninitializing the UART driver.
*
*   Call this function will also turn off the clock of the uart device
* If the uart device has wakeup feature, please don't call this function when sleep
*
* @param[in] uart_id       identifier for uart device
*
* @retval    STATUS_SUCCESS           If uninitialization was successful.
* @retval    STATUS_INVALID_PARAM     Wrong parameter, uart_id is invalid number.
*
*/
uint32_t uart_uninit(uint32_t uart_id);

/**
* @brief Function for power on/off the UART device.
*
*   Call this function with enable=0 will turn off the clock the uart device
* If the uart device has wakeup feature, please don't power off the uart device when sleep
*
* @param[in] uart_id       identifier for uart device
* @param[in] enable        0 for power off the uart device clock, 1 for power on the uart device clock
*
*
* @retval    STATUS_SUCCESS           If power on/off was successful.
* @retval    STATUS_INVALID_PARAM     Wrong parameter, uart_id is invalid number.
*/
uint32_t uart_power(uint32_t uart_id, uint32_t enable);



/**
 * @brief Function for sending data over UART.
 *
 * If an event handler was provided in uart_init() call, this function
 * returns immediately and the handler is called when the transfer is done.
 * Otherwise, the transfer is performed in blocking mode, i.e. this function
 * returns when the transfer is finished.
 *
 *   During the operation it is not allowed to call this function or any other data
 * transfer function again in non-blocking mode. Also the data buffers must stay
 * allocated and the contents of data must not be modified.
 *
 *   Status of the transmitter can be monitored by calling the function
 * "uart_status_get" and checking the tx_busy flag which indicates if transmission
 * is still in progress.
 *
 * @param[in] identifier for uart device,
 * @param[in] p_data     Pointer to data.
 * @param[in] length     Number of bytes to send. Maximum possible length is 4095
 *
 * @retval    STATUS_SUCCESS           If transmit was successful. For non-block mode,
 *                                      data is transmiting.
 * @retval    STATUS_INVALID_PARAM     Wrong parameter, like uart_id is invalid number
 *                                      length size is too large or zero.
 *
 * @retval    STATUS_NO_INIT           uart device did NOT configure yet.
 *
 * @retval    STATUS_EBUSY             uart device is already in previous transmitting.
 *
 *
 */
uint32_t  uart_tx(uint32_t uart_id,
                  uint8_t const *p_data,
                  uint32_t length);


/**
 * @brief Function for checking if UART is currently transmitting.
 *
 * @param[in]  uart_id  identifier for uart device.
 *
 * @retval true  If UART is transmitting.
 * @retval false If UART is not transmitting.
 */
bool uart_tx_in_progress(uint32_t uart_id);

/**
 * @brief Function for checking if UART is currently transmitting or receiving.
 *
 * @param[in]  uart_id  identifier for uart device.
 *
 * @retval true  If UART is transmitting and receiving completed.
 * @retval false If UART is transmitting and receiving incomplete.
 */
bool uart_trx_complete(uint32_t uart_id);

/**
 * @brief Function for aborting any ongoing transmission.
 *
 * @param[in] identifier for uart device,
 */
void uart_tx_abort(uint32_t uart_id);

/**
 * @brief Function for receiving data over UART.
 *
 * If an event handler was provided in the uart_init() call, this function
 * returns immediately and the handler is called when the transfer is done.
 * Otherwise, the transfer is performed in blocking mode, i.e. this function
 * returns when the transfer is finished.
 *
 *   During the operation it is not allowed to call this function or any other data
 * receive function again in non-blocking mode. Also the data buffers must stay
 * allocated and the contents of data must not be modified.
 *
 *   Status of the receiver can be monitored by calling the function "uart_status_get"
 * and checking tge rx_busy flag which indicates if reception is still in progress.
 *
 * @param[in] uart_id    identifier for uart device.
 * @param[in] p_data     Pointer to data.
 * @param[in] length     Number of bytes to receive. Maximum possible length is 4095
 *
 *
 * @retval    STATUS_SUCCESS           Receive request was successful. For non-block mode,
 *                                      data is in receiving state.
 * @retval    STATUS_INVALID_PARAM     Wrong parameter, like uart_id is invalid number
 *                                      length size is too large or zero.
 *
 * @retval    STATUS_NO_INIT           uart device did NOT configure yet.
 *
 * @retval    STATUS_EBUSY             uart device is  in previous receiving state.
 *
 */
uint32_t  uart_rx(uint32_t uart_id,
                  uint8_t  *p_data,
                  uint32_t  length);



/**
 * @brief Function for testing the receiver state in blocking mode.
 *
 * @param[in] uart_id    identifier for uart device.
 *
 * @retval true  If the receiver has at least one byte of data to get.
 * @retval false If the receiver is empty.
 */
bool uart_rx_ready(uint32_t uart_id);

/**
 * @brief Function for aborting any ongoing reception.
 *
 * @param[in] uart_id    identifier for uart device.
 */
void uart_rx_abort(uint32_t uart_id);

/**
 * @brief Function for reading uart status.
 *
 * @param[in] uart_id    identifier for uart device.
 *
 *   Calling this function will return tx_busy, rx_busy, rx_overflow, rx_break,
 *  rx_framing_error, and rx_parity_error.
 *  It also return modem CTS status (uart1 only), too.
 *
 *   RX line status flag will be cleared in next function "uart_rx".
 *
 * @retval    Mask of reported errors. (if Invalid uart_id, it will get the status of all zero)
 */
uart_status uart_status_get(uint32_t uart_id);


/**
 * @brief Function for set/clear break signal
 *
 * @param[in] uart_id    identifier for uart device.
 * @param[in] state      0 for clear break, 1 for set break.
 *
 *   Calling uart_set_break with state=1 will cause uart TXD to low, it must call
 * uart_set_break with state=0 before next transmit another data.
 *
 * @retval    STATUS_SUCCESS           Set/Clear break request was successful.
 *
 * @retval    STATUS_INVALID_PARAM     Wrong parameter, uart_id is invalid number
 *
 * @retval    STATUS_INVALID_REQUEST   uart device clock should be turn on before this request.
 *
 * @retval    STATUS_EBUSY             uart device is in transmitting state.
 */

uint32_t uart_set_break(uint32_t uart_id, uint32_t state);

/**
 * @brief Function for set rts signal
 * (Only available for UART1, hardware flow control enable)
 *
 * @param[in] uart_id    identifier for uart device.
 * @param[in] state      0 for clear rts, 1 for set rts.
 *
 * @retval    STATUS_SUCCESS           Set/Clear RTS request was successful.
 *
 * @retval    STATUS_INVALID_PARAM     Wrong parameter, uart_id is invalid number
 *
 * @retval    STATUS_INVALID_REQUEST   uart device clock should be turn on before this request.
 */
uint32_t uart_set_modem_status(uint32_t uart_id, uint32_t state);


/**
 * @brief Function for get byte from rx fifo
 *
 * @param[in] uart_id    identifier for uart device.
 *
 *    Only calling this function when there is data in RX FIFO.
 *    User call use function uart_rx_ready(uint32_t uart_id) to know whether data in RX FIFO or not.
 *    If there is no data in RX FIFO, calling this function will return unknow data.
 *
 * @retval
 *            data received in RX FIFO
 *
 */
uint8_t uart_rx_getbytes(uint32_t uart_id);

#ifdef __cplusplus
}
#endif

#endif /* end of ___UART_DRV_H__ */

/*@}*/ /* end of peripheral_group UART_Driver */
