/**************************************************************************//**
 * @file     qspi.h
 * @version
 * @brief    QSPI driver header file
 *
 * @copyright
 ******************************************************************************/
/** @defgroup QSPI_Driver QSPI
 *  @ingroup  peripheral_group
 *  @{
 *  @brief  QSPI Driver header information
 *
 *  QSPI and SPI is the same hardware, the only
 * difference is QSPI driver designed for Flash device.
 *
 * QSPI flash has specific transfer protocol for read/write behavior.
 * QSPI can use 1, 2, 4 data pins for transfer mode.
 * This driver is master mode for QSPI-flash.
 *
 *   However, SPI device only has 2 data pins, MISO and MOSI.
 * This driver can be master or slave for SPI device.
 *
 *   ONLY QSPI0 has 4 data pin data interface. QSPI1 is standard SPI inferface.
 *   So if there is one QSPI-flash device connected with the qspi port (QSPI0), then
 * the qspi port should set master mode.
 *
 *   QSPI0 also supports multiple CS, if you want to use this option, please
 * enable the define in project_config.h
 *
 *   #define SUPPORT_QSPI0_MULTI_CS             1
 *
 *   And if you enable this "SUPPORT_QSPI0_MULTI_CS" option, you should assign
 * select_slave_device member for each spi/qspi request.
 *
 *   Notice: Only QSPI0 supports multiple chip-select. For QSPI1, select_slave_device
 * must be SPI_SELECT_SLAVE_0.
 *
*/
#ifndef ___QSPI_MASTER_H__
#define ___QSPI_MASTER_H__

#include  "project_config.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/

/**
 *  @brief SPI/QSPI Constant Definitions
 */


#define   QSPI_NORMAL_SPI           0         /*!< SPI normal mode   */
#define   QSPI_DUAL_SPI             2         /*!< QSPI 2bits mode   */
#define   QSPI_QUAD_SPI             3         /*!< QSPI 4bits mode   */


#define   QSPI_CLK_32M              (0)                                             /*!< SPI running 32MHz   */
#define   QSPI_CLK_16M              (QSPI_MST_CLKDIV_EN|QSPI_MST_CLKDIV_16MHZ)      /*!< SPI running 16MHz   */
#define   QSPI_CLK_8M               (QSPI_MST_CLKDIV_EN|QSPI_MST_CLKDIV_8MHZ)       /*!< SPI running  8MHz   */
#define   QSPI_CLK_4M               (QSPI_MST_CLKDIV_EN|QSPI_MST_CLKDIV_4MHZ)       /*!< SPI running  4MHz   */
#define   QSPI_CLK_2M               (QSPI_MST_CLKDIV_EN|QSPI_MST_CLKDIV_2MHZ)       /*!< SPI running  2MHz   */
#define   QSPI_CLK_1M               (QSPI_MST_CLKDIV_EN|QSPI_MST_CLKDIV_1MHZ)       /*!< SPI running  1MHz   */

#define   QSPI_STATUS_TRANSFER_COMPLETE     (1<<0)
#define   QSPI_STATUS_TRANSFER_SHORT            (1<<1)



#define   SPI_LSB_ORDER             (0)       /*!< SPI Least significant bit shifted out first.   */
#define   SPI_MSB_ORDER             (1)       /*!< SPI Most significant bit shifted out first.    */

#define   SPI_SELECT_SLAVE_0        (0)       /*!< Select Slave 0     */
#define   SPI_SELECT_SLAVE_1        (1)       /*!< Select Slave 1     */
#define   SPI_SELECT_SLAVE_2        (2)       /*!< Select Slave 2     */
#define   SPI_SELECT_SLAVE_3        (3)       /*!< Select Slave 3     */

#define   SPI_CHIPSEL_ACTIVE_LOW    (0)       /*!< Active low for Chip Select, slave selected in CS in low    */
#define   SPI_CHIPSEL_ACTIVE_HIGH   (1)       /*!< Active high for Chip Select,slave selected in CS in high   */


#define   SPI_SLAVE_MODE            (0)       /*!< SPI controller will be set as slave mode.    */
#define   SPI_MASTER_MODE           (1)       /*!< SPI controller will be set as master mode.   */


/**
 *  @Brief SPI operation state return Constant Definitions
 */

#define   QSPI_STATE_UNINIT         0         /*!< SPI controller in uninitial state.   */
#define   QSPI_STATE_IDLE           1         /*!< SPI controller in idle state.        */
#define   QSPI_STATE_TRANSFER       2         /*!< SPI controller in CPU transfer state.  */
#define   QSPI_STATE_SETUP          4         /*!< SPI controller in setting controller state.   */
#define   QSPI_STATE_WRITE_DMA      8         /*!< SPI controller in DMA write (TX only) transfer state.   */
#define   QSPI_STATE_READ_DMA       16        /*!< SPI controller in DMA write/read transfer  state.   */


/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 * @brief QSPI finish routine callback for user application.
 *
 * @param qspi_id   QSPI id.  0 for QSPI0, 1 for QSPI 1
 *
 * @param statue    QSPI transfer status.
 *
 *   status is always QSPI_STATUS_TRANSFER_COMPLETE
 */
typedef void (*qspi_proc_cb_t)(uint32_t qspi_id, uint32_t status);


/**
 *  @brief  qspi_transfer_mode_t
 *
 *         Structure for the spi controller mode setting.
 *
 *
 *   SPI_BIT_ORDER: for select MSB or LSB shifted out first.
 *
 *   SPI_CPHA/SPI_CPOL: for SPI data sample timing and CLK active low or active high.
 *
 *   SPI_CS_POL:    for select chip active low or active high.
 *
 *   SPI_MASTER: for select the controller to be master or slave.
 *
 *
 *   SPI_CS: for the controller SPI0, it can support 4 spi slave devices.
 *           For slave device, this setting is the pin connected to SPI host.
 *
 *   SPI_CLK: the master output clk frequency.
 *            For slave device, this setting will be ignored.
 *
 */

typedef struct
{
    uint8_t   SPI_BIT_ORDER:  1;           /*!< SPI_LSB_ORDER or SPI_MSB_ORDER   */

    uint8_t   SPI_CPOL:       1;           /*!< for SPI CLK active low or active high   */
    uint8_t   SPI_CPHA:       1;           /*!< for SPI data sample timing in CLK low or high   */

    uint8_t   SPI_CS:         2;           /*!< For SPI0 master mode, controller can select 4 slave devices to send SPI signal   */
    uint8_t   SPI_CS_POL:     1;           /*!< Chip select is active high or active low in operation. SPI_CHIPSEL_ACTIVE_LOW or SPI_CHIPSEL_ACTIVE_HIGH    */

    uint8_t   SPI_MASTER:     1;           /*!< Select for SPI_MASTER_MODE or SPI_SLAVE_MODE */

    uint8_t   reserved:       1;

    uint16_t  SPI_CLK;                     /*!< For host mode, the controller will output CLK by this setting. For slave mode, this value will be ignored  */

} qspi_transfer_mode_t;


/**
 *   @brief  qspi_block_request_t
 *
 *  QSPI request is "read data","write data" or "command only".
 *  QSPI flash device can NOT read and write at the same time.
 *
 *    If you want to write data to flash, then you should set cmd_buf and write_buf
 *  to correct buffer you want to send, and set read_buf to NULL.
 *
 *    If you want to read data from flash, then you should set cmd_buf and read_buf
 *  to correct buffer you want to send/receive, and set write_buf to NULL.
 *
 *    Notice cmd_buf, write_buf, and read_buf must be 4 bytes-alignemnt for non-dma
 *  function qspi_flash_transfer(...)
 *
 *
 *   memmber "data_transfer_mode" is used for data transfer mode, it could be NORMAL/DUAL/QUAD
 *   Please notice: there is no DUAL write data command in flash.
 *
 *
 *   This programming model is based on qspi flash. Please refer some flash datasheet to know more
 *   flash command instruction,
 *
 */
typedef struct
{
    uint8_t    *cmd_buf;            /*!< pointer to command buffer for QSPI transfer, it must be 4 bytes alignment   */
    uint8_t    *write_buf;          /*!< pointer to write buffer for QSPI transfer, host will send this buffter to spi slave device. It must be 4 bytes alignment   */
    uint8_t    *read_buf;           /*!< pointer to read buffer for QSPI transfer, host will receive data from slave device. It must be 4 bytes alignment   */

    uint16_t   cmd_length;          /*!< command buffer length for QSPI transfer  */
    uint16_t   write_length;        /*!< write buffer length for data to send to spi slave device.  */
    uint16_t   read_length;         /*!< read buffer length for data to receive from spi slave device   */

    uint8_t    data_transfer_mode  : 2;     /*!< QSPI_NORMAL_SPI / QSPI_DUAL_SPI / QSPI_QUAD_SPI    */

#if (MODULE_ENABLE(SUPPORT_QSPI0_MULTI_CS))
    uint8_t    select_slave_device : 2;     /*!< For SPI0 master mode, controller can select 4 slave devices to send SPI signal   */
#endif

} qspi_block_request_t;


/**
 *  @brief  spi_block_request_t
 *
 *     spi_block_request_t is used for standard SPI device model.
 *     It means that SPI hardware will send data and receive data at the same time.
 *     If system doesn't care receive data, it can set read_buf to NULL, then system will
 *   only send data to other spi device.
 *     If system doesn't care sending data, it can set write_buf to NULL, then system will
 *   only receive data from other spi device.
 *
 *     write_buf is the buffer address for spi device TX.
 *   For master mode, the spi port will output the buffer data in is MOSI/SO.
 *   For slave mode, the spi port will output the buffer data in MISO/SI.
 *
 *     read_buf is the buffer address for spi device RX.
 *   For master mode, the spi port will receive input data from MISO/SI to the buffer.
 *   For slave mode, the spi port  will receive input data from MOSI/SO to the buffer.
 *
 */
typedef struct
{
    uint8_t    *write_buf;         /*!< pointer to TX buffer for SPI transfer, it could be NULL for RX only.  */
    uint8_t    *read_buf;          /*!< pointer to RX buffer for SPI transfer, it could be NULL for TX only.   */

    uint16_t   length;             /*!< transfer length.  \hideinitializer */

#if (MODULE_ENABLE(SUPPORT_QSPI0_MULTI_CS))
    uint8_t    select_slave_device  : 2;     /*!< For SPI0 master mode, controller can select 4 slave devices to send SPI signal   */
#endif

} spi_block_request_t;


/**************************************************************************************************
*    GLOBAL PROTOTYPES
*************************************************************************************************/
/**
 * @brief initial QSPI controller  function.
 *
 * @param[in]    qspi_id    Specifies the controller id.  0 for QSPI0/ 1 for QSPI1.
 *
 * @param[in]    spi_mode   Specifies the controller transfer operation mode.
 *
 * @return
 * @retval    STATUS_ERROR            ---  This controller is in busying state for transfer
 * @retval    STATUS_INVALID_PARAM    ---  Wrong parameter
 * @retval    STATUS_SUCCESS          ---  This controller setting is successful
 *
 * @details
 *
 *    If there is only one spi device connected SPI0(QSPI0), then the "select polarity"
 *  set in parameter spi_mode->SPI_CS_POL, and the chip select is set in parameter
 *  spi_mode->SPI_CS by default. Then each data transfer, system will using these settings
 *  to transfer.
 */

extern uint32_t qspi_init(uint32_t qspi_id, const qspi_transfer_mode_t *spi_mode);


#if (MODULE_ENABLE(SUPPORT_QSPI0_MULTI_CS))

/**
 * @brief initial QSPI slave device polarity
 *
 * @param[in]    slave_id         Specifies slave id.
 *
 * @param[in]    slave_polarity   Specifies the slave select polarity.
 *
 * @return
 * @retval   STATUS_ERROR            ---  This controller is in busying state for transfer
 * @retval   STATUS_SUCCESS          ---  This controller setting is successful
 *
 * @details
 *       This function is only used for SPI0 controller, when SPI0(QSPI0) connects multiple spi devices.
 *     SPI0(QSPI0) can use chip_select to enable device between maximum 4 spi slave, and each slave device
 *     has itself "select polarity" (CS active low or active high) to start/stop the spi transfer.
 *     Using this API to set each spi device "select polarity" independently.
 *
 *       Please call API after qspi_init and before any SPI0(QSPI0) transfer.
 *
 *       If there is only one spi device for SPI0, then this option can be set in qspi_init function by
 *     value SPI_CS_POL and SPI_CS. Then each transfer will use this default setting.
 *
 */

extern uint32_t qspi0_set_device_polarity(uint32_t slave_id, uint32_t slave_polarity);


#endif

/**
 * @brief QSPI transfer function.
 *
 * @param[in]    qspi_id    Specifies the controller id.  0 for QSPI0/ 1 for QSPI1.
 *
 * @param[in]    req        Specifies the transfer request.
 *
 *
 * @return
 * @retval    STATUS_INVALID_REQUEST  SPI controller is busy or uninitial.
 * @retval    STATUS_SUCCESS          SPI transfer complete
 *
 * @details
 *
 *    QSPI is master mode only. This function is using PIO (program I/O mode),
 * that is, using cpu to move data between ram and spi FIFO.
 *
 *    qspi_flash_block_request_t request buffer start address must be 4-bytes
 * alignment. (Because the transfer will use 32-Bit transfer mode)!
 *
 *   Please do NOT call qspi_transfer function in ISR. Because this function
 * is BLOCK function. It returns after the request finish.
 *
 */
extern uint32_t qspi_transfer(uint32_t qspi_id, const qspi_block_request_t *req);


/*
 *  This function is used for to change SPI_CLK..
 *
 *  @param[in]    qspi_id    Specifies the controller id.  0 for QSPI0/ 1 for QSPI1.
 *
 *  @param[in]    SPI_CLK    Specifies the transfer clock.
 *
 *  @return
 *  @retval       STATUS_INVALID_REQUEST  SPI controller is busy or uninitial.
 *  @retval       STATUS_SUCCESS          SPI transfer complete
 *
 *  @details
 *
 *     This setting is only useful for master mode. Slave device's spi clock dependes on Master spi clock.
 *     If system only connects one SPI devices, then set the SPI clock in "member qspi_transfer_mode_t.SPI_CLK"
 *     is enough. Transfer will use that default setting clock. This function is designed for if there are
 *     multiple spi devices, with different operation speed, connected with QSPI0.
 */

extern uint32_t qspi_change_clk(uint32_t qspi_id, uint16_t   SPI_CLK);


/**
 * @brief QSPI transfer data to spi slave device by using DMA
 *
 *
 * @param[in]    qspi_id    Specifies the controller id.  0 for QSPI0/ 1 for QSPI1.
 *
 * @param[in]    req        Specifies the transfer request.
 *
 * @param[in]    finish_proc_cb  Specifies the interrupt ISR callback function when write data  request completed.
 *
 *
 * @return
 * @retval    STATUS_INVALID_REQUEST  ---  QSPI Master is busy for previous tranfer request..
 * @retval    STATUS_SUCCESS  ---  This request is in progressing...
 *
 * @details
 *      qspi_write_dma function is only for QSPI master only.
 *
 *      This function designed for write data to QSPI flash device, like QSPI flash command 0x32/0x02.
 *      In QSPI flash command, there is only two phase for "write command", command phase and write data phase.
 *      Fill QSPI command to command buffer, and fill write data to write_buf.
 *      And set read_buf to NULL.
 *      This function is non-block function, that is, when this function reutrns, it just means the operation
 *      is start in progressing.... when the request finishs, the notify callback function "finish_proc_cb" will be called
 *
 */
extern uint32_t qspi_write_dma(
    uint32_t qspi_id,
    const qspi_block_request_t *req,
    qspi_proc_cb_t finish_proc_cb
);

/**
 * @brief QSPI read data from spi slave device by using DMA
 *
 *
 * @param[in]    qspi_id    Specifies the controller id.  0 for QSPI0/ 1 for QSPI1.
 *
 * @param[in]    req        Specifies the transfer request.
 *
 * @param[in]    finish_proc_cb  Specifies the interrupt ISR callback function when read data request completed.
 *
 *
 * @return
 * @retval    STATUS_INVALID_REQUEST  QSPI Master is busy for previous tranfer request..
 * @retval    STATUS_SUCCESS          This request is in progressing...
 *
 * @details
 *      qspi_read_dma function is only for QSPI master only.
 *
 *      This function designed for read data from QSPI flash device, like QSPI flash command 0x3B/0x6B/0x0B.
 *      In QSPI flash command, there is only two phase for "read command", command phase and read data phase.
 *      Fill QSPI command to command buffer, and set write data to NULL.
 *      And set read_buf to pointer of receied data.
 *      This function is non-block function, that is, when this function reutrns, it just means the operation
 *      is start in progressing.... when the request finishs, the notify callback function "finish_proc_cb" will be called
 *
 */

extern uint32_t qspi_read_dma(
    uint32_t qspi_id,
    const qspi_block_request_t *req,
    qspi_proc_cb_t finish_proc_cb
);

/**
 * @brief SPI transfer data
 *
 * @param[in]    qspi_id    Specifies the controller id.  0 for QSPI0/ 1 for QSPI1.
 *
 * @param[in]    req        Specifies the spi_block_request_t transfer request.
 *
 * @param[in]    finish_proc_cb  Specifies the interrupt ISR callback function when transfer data completed.
 *
 *
 * @return
 * @retval    STATUS_INVALID_REQUEST  ---  SPI controller is busy for previous tranfer request..
 * @retval    STATUS_SUCCESS  ---  This request is in progressing...
 *
 * @details
 *     The function spi_transfer transfers data via SPI by using DMA to transfer.
 *   It synchronously sends data to the SPI transmitter and receives data from the SPI receiver.
 *
 *   The parameter spi_block_request_t is the data structure of transfer data, it includes:
 *     write_buf is a pointer to the buffer with data to send.
 *     read_buf  is a pointer to the buffer which receives data.
 *     length    specifies the number of "bytes" to transfer.
 *
 *   The transfer length should be less than 65536 bytes.
 *
 *   Calling the function spi_transfer only starts the transfer operation.
 * The function is non-blocking and returns as soon as the driver has started
 * the operation (driver will configures DMA for continuous transfer).
 *
 *   When in slave mode, the operation is only registered and started
 * when the master starts the transfer.
 *
 *   During the operation it is not allowed to call this function or any other data
 * transfer function again. Also the data buffers must stay allocated and the contents
 * of unsent data must not be modified.
 *
 *   When transfer operation is completed (requested number of items transferred),
 * the callback function will be called to notify finish.
 *
 */

extern uint32_t spi_transfer(
    uint32_t qspi_id,
    const spi_block_request_t *req,
    qspi_proc_cb_t finish_proc_cb
);


/**
 * @brief SPI transfer data by PIO
 *
 * @param[in]    qspi_id    Specifies the controller id.  0 for QSPI0/ 1 for QSPI1.
 *
 * @param[in]    req        Specifies the spi_block_request_t transfer request.
 *
 * @return
 * @retval    STATUS_INVALID_REQUEST  ---  SPI controller is busy for previous tranfer request..
 * @retval    STATUS_SUCCESS  ---  This request is in progressing...
 *
 *
 * @details
 *   The function spi_transfer_pio transfers data via SPI by Programming I/O mode.
 * That is, this function uses CPU to move data to spi FIFO.
 * This function is blocking function, it returns when all data transfer complete.
 * This function is useful for short command data transfer.
 *
 *   Please do NOT　call this function if SPI is in slave mode,
 * because it can not predict when time host will send data to device.
 * CPU busy waiting for unpredictable event is not a good design.
 *
 *
 *   The parameter spi_block_request_t is the data structure of transfer data, it includes:
 *     write_buf is a pointer to the buffer with data to send.
 *     read_buf  is a pointer to the buffer which receives data.
 *     length    specifies the number of "bytes" to transfer.
 *
 *   If SPI master wants to send data to and receive data from slave at the same time,
 * the transfer length should be less than or equal 32 bytes.
 *   If SPI master only wants to send data to spi slave, the transfer length
 * there is no limit for transfer length size. But busying wait SPI transfer complete will
 * waste CPU resource. Please do NOT call this function in interrupt ISR,
 *
 *
 */

extern uint32_t spi_transfer_pio(
    uint32_t qspi_id,
    const spi_block_request_t *req
);

/**
 * @brief Get SPI controller state
 *
 * @param[in]    qspi_id    Specifies the controller id.  0 for QSPI0/ 1 for QSPI1.
 *
 *
 * @return
 * @retval    QSPI_STATE_UNINIT
 * @retval    QSPI_STATE_IDLE
 * @retval    QSPI_STATE_SETUP
 * @retval    QSPI_STATE_TRANSFER
 * @retval    QSPI_STATE_WRITE_DMA
 * @retval    QSPI_STATE_READ_DMA
 *
 * @details
 *     This function will return current controller operation state.
 *
 */

extern uint16_t get_qspi_state(uint32_t qspi_id);

/**
 * @brief abort slave received DMA command.
 *
 * @param[in]    qspi_id    Specifies the controller id.  0 for QSPI0/ 1 for QSPI1.
 *
 * @return
 * @retval    STATUS_SUCCESS
 * @retval    STATUS_INVALID_REQUEST  --- this function can not abort SPI master
 *
 *
 * @details
 *     This function will abort the qspi slave send/receive command...
 *     Because we don't know which time host will send data to SPI slave... we
 *     start dma request to wait SPI host command, but for some reason, we should terminate to wait
 *     the request.
 *
 */

extern uint32_t spi_transfer_slave_abort(uint32_t qspi_id);

/**
 * @brief clear FIFO buffer command.
 *
 * @param[in]    qspi_id    Specifies the controller id.  0 for QSPI0/ 1 for QSPI1.
 *
 * @details
 *     This function will clear the qspi FIFO buffer command...
 *
 */
extern void qspi_FIFO_clear(uint32_t qspi_id);


#ifdef __cplusplus
}
#endif

#endif /* end of ___QSPI_MASTER_H__ */

/*@}*/ /* end of peripheral_group SPI_Drver */
