/**************************************************************************//**
 * @file     i2c_master.h
 * @version
 * @brief    I2C MASTER driver header file
 *
 * @copyright
 ******************************************************************************/
/** @defgroup I2C_Master_Driver I2C_Master
 *  @ingroup  peripheral_group
 *  @{
 *  @brief  I2C_Master Driver header information
*/
#ifndef ___I2C_MASTER_H__
#define ___I2C_MASTER_H__

#ifdef __cplusplus
extern "C"
{
#endif


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
/**
 *  @Brief I2C CLOCK Constant Definitions
 */

#define I2C_CLOCK_800K      3       /*!< I2C Master running in 800KHz   */
#define I2C_CLOCK_400K      0       /*!< I2C Master running in 400KHz   */
#define I2C_CLOCK_200K      1       /*!< I2C Master running in 200KHz   */
#define I2C_CLOCK_100K      2       /*!< I2C Master running in 100KHz   */


/**
 *  @Brief I2C Master return ERROR Constant Definitions
 */

/*return status in I2C USER notification. */
#define I2C_STATUS_OK             0           /*!< I2C command return SUCCESS   */
#define I2C_STATUS_ERR_NOACK      1           /*!< I2C command return ERR NOACK */

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 *@brief  Structure for the I2C transfer request.
 */
typedef struct
{
    /*@{*/
    uint8_t     dev_addr;        /*!< I2C device address, 7bits only   */
    uint8_t     bFlag_16bits;    /*!< 1 for register address is 16bits, 0 for register address is 8bits.  */
    uint16_t    reg_addr;        /*!< I2C register address value   */
    /*@}*/
} i2c_slave_data_t;

/**
 * @brief I2C finish routine notify callback for user application.
 *
 * @param[in] statue I2C transfer status.
 *              It must be I2C_STATUS_OK or I2C_STATUS_ERR_NOACK.
 *
 *
 * @details    This callback function is still running in interrupt mode, so this function
 *              should be as short as possible. It can NOT call any block function in this
 *              callback service routine.
 *
 *              This function will be called when I2C master finished the i2c transfer request,
 *              or when there is no ACK during the transfer (error case).
 *
 */
typedef void (* i2cm_proc_cb_t)(uint32_t status);

/**
 * @brief I2C master pre-initial function.
 *
 * @param[in]    SCL_pin    Specifies the SCL pin number.
 *
 * @param[in]    SDA_pin    Specifies the SDA pin number.
 *
 * @details
 *     It is possible that previous i2c transfer uncomplete because of reset or any hardware reason.
 *   For this case, I2C device needs extra 9 SCL clock to reset itself default state.
 *   So the first time after booting, MCU will send 9 SCL clock to reset i2c device.
 *
 *   SCL_pin is SCL pin number. Only pin18, pin20, or pin22 is correct setting pin.
 *   SDA_pin is SDA pin number. Only pin19, pin21, or pin23 is correct setting pin.
 *
 *   This function also will set the pin as open drain mode.
 *   So if system want to enter deeply sleep mode, it should turn these pins to input mode again.
 *
 */

uint32_t i2c_preinit(uint32_t SCL_pin, uint32_t SDA_pin);

/**
 * @brief Set I2C master initialize
 *
 * @param[in]  I2C_SPEED   Set the I2C master bus clock frequency
 *
 * @retval
 *       STATUS_SUCCESS           --- I2C master initial success.
 *       STATUS_INVALID_REQUEST   --- I2C master is not in idle mode.
 *
 * @details
 *       Call this function to initail I2C master, this function like
 *       "driver open" function.
 *
 */

uint32_t i2c_init(uint32_t I2C_SPEED);

/* Notice:
 *  Because some i2c device can not suppport more than 400KHz,
 * it means it will waste huge waiting time for block/polling mode during data transfer.
 *   The driver does NOT use polling mode to check I2C transfer finish, it uses interrupt mode.
 * So the driver is implemented in asynchronous non-block functions for i2c_read/i2c_write transfer,
 * that is, when i2c_write/i2c_read function return does NOT mean transfer finish.
 * i2c_write/i2c_read function should give one function parameter ---
 * "finish callback" hook, this hook will be invoked when the i2c_read/i2c_write
 * operation finished.
 *
 * Please do NOT call i2c_write / i2c_read function in ISR.
 *
 */

/*
 * In this function "Addr" is 7 bits address, it will auto add bit8 for write
 * mode.
 */

/**
 * @brief  I2C write data to slave
 *
 * @param[in]  slave       Specifies the I2C slave address and register address.
 * @param[in]  data        Pointer to buffer with data to write to I2C slave.
 * @param[in]  len         Number of data bytes to transmit, maxmum size is 1024 bytes.
 * @param[in]  endproc_cb  Specifies the interrupt ISR callback function when I2C request completed or aborted.
 *
 * @return   I2C transfer status
 *
 * @retval   STATUS_EBUSY          I2C Master is busy for previous tranfer request..
 * @retval   STATUS_NO_INIT        I2C Master does NOT initial, pleae call i2c_init before this request.
 * @retval   STATUS_INVALID_PARAM  Transfer data size is too large.
 * @retval   STATUS_SUCCESS        This request is in progressing...
 *
 * @details
 *       Notice: This function is non-blocking and returns as soon as the driver
 *    has started the request. During the operation it not allowed to call any I2C Master function again.
 *    Also the data buffer must stay allocated and the contents of data must not be modified.
 *    When request has finished, the callback function "endproc_cb" will be called with parameter I2C_STATUS_OK.
 *
 *       The request is aborted in the following case:
 *           selected slave has not acknowledged the address.
 *           bus error has been detected.
 *    If aborted event happened, he callback function "endproc_cb" will be called with parameter I2C_STATUS_ERR_NOACK.
 *
 *       It is possible that multi-task to call this function at the same time in multitask OS.
 *     Only one request can be served. So caller should check return value.
 *     If return value is STATUS_EBUSY, caller should wait a period and then
 *     retry the request.
 *
 */

uint32_t i2c_write(
    const i2c_slave_data_t  *slave,
    uint8_t *data,
    uint32_t len,
    i2cm_proc_cb_t  endproc_cb
);

/**
 * @brief  I2C read data from slave
 *
 * @param[in]  slave       Specifies the I2C slave address and register address.
 * @param[in]  data        Pointer to buffer with data to receive from I2C slave.
 * @param[in]  len         Number of data bytes to receive, maxmum size is 1024 bytes.
 * @param[in]  endproc_cb  Specifies the interrupt ISR callback function when I2C request completed or aborted.
 *
 * @return   I2C transfer status
 *
 * @retval   STATUS_EBUSY      I2C Master is busy for previous tranfer request..
 * @retval   STATUS_NO_INIT    I2C Master does NOT initial, pleae call i2c_init before this request.
 * @retval   STATUS_INVALID_PARAM  receive data size is too large.
 * @retval   STATUS_SUCCESS    This request is in progressing...
 *
 * @details
 *       Notice: This function is non-blocking and returns as soon as the driver
 *    has started the request. During the operation it not allowed to call any I2C Master function again.
 *    Also the data buffer must stay allocated.
 *    When request has finished, the callback function "endproc_cb" will be called with parameter I2C_STATUS_OK.
 *
 *       The request is aborted in the following case:
 *           selected slave has not acknowledged the address.
 *           bus error has been detected.
 *    If aborted event happened, he callback function "endproc_cb" will be called with parameter I2C_STATUS_ERR_NOACK.
 *
 *       It is possible that multi-task to call this function at the same time in multitask OS.
 *     Only one request can be served. So caller should check return value.
 *     If return value is STATUS_EBUSY, caller should wait a period and then
 *     retry the request.
 *
 */

uint32_t i2c_read(
    const i2c_slave_data_t  *slave,
    uint8_t  *data,
    uint32_t len,
    i2cm_proc_cb_t  endproc_cb
);

/*@}*/ /* end of peripheral_group I2C_Master_Driver */

#ifdef __cplusplus
}
#endif

#endif /* end of ___I2C_MASTER_H__ */

