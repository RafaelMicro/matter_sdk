/** @file i2s.h
 *
 * @brief I2s driver header file.
 *
 */

/**
 * @defgroup i2s_group I2S
 * @ingroup peripheral_group
 * @{
 * @brief Define I2S definitions, structures, and functions.
 */

#ifndef __I2S_H__
#define __I2S_H__

#ifdef __cplusplus
extern "C"
{
#endif
/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**@brief I2S-XDMA type, RDMA or WDMA.
 */
typedef enum
{
    I2S_XDMA_RDMA = 0,
    I2S_XDMA_WDMA,
    I2S_XDMA_MAX,
}   i2s_xdma_t;

/**@brief I2S Simple Rate mapping table
 */
typedef enum
{
    I2S_SR_48K, /**< I2S_WCK 48K*/
    I2S_SR_32K, /**< I2S_WCK 32K*/
    I2S_SR_16K, /**< I2S_WCK 16K*/
    I2S_SR_8K,  /**< I2S_WCK 8K*/
    I2S_SR_MAX, /**< I2S_WCK MAX*/
} i2s_sample_rate_t;

typedef enum
{
    I2S_IMCLK_12P288M,              /**< PLL_CK = 32M, I2S_WCK = 48K*/
    I2S_IMCLK_8P192M,               /**< PLL_CK = 32M, I2S_WCK = 8/16/32K*/
    I2S_IMCLK_24P576M,              /**< PLL_CK = 64M, I2S_WCK = 48K*/
    I2S_IMCLK_16P384M,              /**< PLL_CK = 64M, I2S_WCK = 64K*/
    I2S_IMCLK_MAX,                  /**< PLL_CK = 64M, I2S_WCK = 8/16/32K*/
} i2s_cfg_imck_rate_t;

/**@brief I2S channel mode, Stereo / Left channel only / Right channel only
 */
typedef enum
{
    I2S_CH_STEREO,
    I2S_CH_MONO_L,
    I2S_CH_MONO_R,
    I2S_CH_MAX,
} i2s_ch_format_t;

/**@brief I2S format table, standard I2S mode / Left Justified mode / Right Justified mode
 */
typedef enum
{
    I2S_FMT_LJ = 0,
    I2S_FMT_RJ,
    I2S_FMT_I2S,
    I2S_FMT_MAX,
} i2s_cfg_i2s_fmt_t, i2s_fmt_t;

/**@brief MCLK selection index table
 */
typedef enum
{
    I2S_MCLK_ISEL_0 = 0,
    I2S_MCLK_ISEL_1,
    I2S_MCLK_ISEL_2,
    I2S_MCLK_ISEL_3,
    I2S_MCLK_ISEL_4,
    I2S_MCLK_ISEL_5,
    I2S_MCLK_ISEL_MAX,
} i2s_mclk_isel_t;

/**@brief I2S MCLK division table
 */
typedef enum
{
    I2S_MCLK_DIV_1 = 0,
    I2S_MCLK_DIV_2,
    I2S_MCLK_DIV_4,
    I2S_MCLK_DIV_8,
    I2S_MCLK_DIV_16,
    I2S_MCLK_DIV_32,
    I2S_MCLK_DIV_MAX,
} i2s_mclk_div_t;

/**@brief I2S BCLK OSR setting table
 */
typedef enum
{
    I2S_CFG_BCK_OSR_2 = 0,
    I2S_CFG_BCK_OSR_4,
    I2S_CFG_BCK_OSR_8,
    I2S_CFG_BCK_OSR_MAX,
} i2s_cfg_bck_osr_t;

/**@brief I2S TRX mode setting table
 */
typedef enum
{
    I2S_TRX_MODE_TXRX = 0,
    I2S_TRX_MODE_TX,
    I2S_TRX_MODE_RX,
    I2S_TRX_MODE_MAX,
} i2s_cfg_i2s_mode_t, i2s_trx_mode_t;

/**@brief I2S date depth
 */
typedef enum
{
    I2S_CFG_WID_16 = 0,
    I2S_CFG_WID_24,
    I2S_CFG_WID_32,
    I2S_CFG_WID_MAX,
} i2s_cfg_txd_wid_t, i2s_cfg_rxd_wid_t, i2s_bck_width_t;

/**@brief I2S BLCK / WRCK ratio
 */
typedef enum
{
    I2S_BCK_RATIO_32 = 0,
    I2S_BCK_RATIO_48,
    I2S_BCK_RATIO_64,
    I2S_BCK_RATIO_MAX,
} i2s_cfg_bck_ratio_t;
/**
 * @brief
 */
typedef enum
{
    I2S_CB_WDMA,    /**< CB generated when the buffer is filled with samples. */
    I2S_CB_RDMA,   /**< CB generated when the requested channel is sampled. */
} i2s_cb_type_t;
/**
 * @brief
 */
typedef struct
{
    i2s_cb_type_t type;    /**< i2s type.   */
    uint16_t      blk_size;
    uint16_t      seg_size;
} i2s_cb_t;

typedef void (*i2s_isr_handler_t)(i2s_cb_t *p_cb);

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define I2S_MS_ENABLE_I2S               (0x01UL << I2S_MS_ENABLE_I2S_SHFT)
#define I2S_MS_ENABLE_MCLK              (0x01UL << I2S_MS_ENABLE_MCLK_SHFT)
#define I2S_MS_RESET                    (0x01UL << I2S_MS_RESET_SHFT)
#define I2S_RDMA_ENABLE                 (0x01UL << I2S_RDMA_ENABLE_SHFT)
#define I2S_RDMA_RESET                  (0x01UL << I2S_RDMA_RESET_SHFT)
#define I2S_WDMA_ENABLE                 (0x01UL << I2S_WDMA_ENABLE_SHFT)
#define I2S_WDMA_RESET                  (0x01UL << I2S_WDMA_RESET_SHFT)
#define I2S_RDMA_IRQ_CLR                (0x01UL << I2S_RDMA_IRQ_CLR_SHFT)
#define I2S_RDMA_ERR_IRQ_CLR            (0x01UL << I2S_RDMA_ERR_IRQ_CLR_SHFT)
#define I2S_WDMA_IRQ_CLR                (0x01UL << I2S_WDMA_IRQ_CLR_SHFT)
#define I2S_WDMA_ERR_IRQ_CLR            (0x01UL << I2S_WDMA_ERR_IRQ_CLR_SHFT)
#define I2S_RDMA_IRQ_MASK_ENABLE        (0x01UL << I2S_RDMA_IRQ_MASK_SHFT)
#define I2S_RDMA_ERR_IRQ_MASK_ENABLE    (0x01UL << I2S_RDMA_ERR_IRQ_MASK_SHFT)
#define I2S_WDMA_IRQ_MASK_ENABLE        (0x01UL << I2S_WDMA_IRQ_MASK_SHFT)
#define I2S_WDMA_ERR_IRQ_MASK_ENABLE    (0x01UL << I2S_WDMA_ERR_IRQ_MASK_SHFT)

#define I2S_GET_RDMA_SEG_SIZE               (I2S0->I2S_RDMA_SET0)
#define I2S_GET_RDMA_BLK_SIZE               (I2S0->I2S_RDMA_SET0>>16)
#define I2S_GET_WDMA_SEG_SIZE               (I2S0->I2S_WDMA_SET0)
#define I2S_GET_WDMA_BLK_SIZE               (I2S0->I2S_WDMA_SET0>>16)
/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/
#define I2S_DISABLE_I2S()               (I2S0->I2S_MS_CTL0 &= (~I2S_MS_ENABLE_I2S))
#define I2S_DISABLE_MCLK()              (I2S0->I2S_MS_CTL0 &= (~I2S_MS_ENABLE_MCLK))
#define I2S_RESET()                     (I2S0->I2S_MS_CTL1 = I2S_MS_ENABLE_I2S | I2S_MS_RESET)

#define I2S_GET_RDMA_ACCESS_POS()       (I2S0->I2S_RDMA_R0)
#define I2S_GET_WDMA_ACCESS_POS()       (I2S0->I2S_WDMA_R0)
#define I2S_GET_FW_ACCESS_POS(para_set) (para_set->i2s_fw_access_addr)

/**@brief Structure for I2S-xDMA configurarions
 */
typedef struct
{
    uint32_t    i2s_xdma_start_addr;        /**< xDMA  start address*/
    uint32_t    i2s_fw_access_addr;         /**< Firmware access address */
    uint16_t    i2s_xdma_seg_size;          /**< xDMA Segment size */
    uint16_t    i2s_xdma_blk_size;          /**< xDMA Block size*/
    uint8_t     i2s_xdma_seg_blk_ratio;     /**< xDMA Segment and Block ratio*/
} i2s_xdma_ctrl_ptr_t, i2s_rdma_ctrl_ptr_t, i2s_wdma_ctrl_ptr_t;

/**@brief Structure for I2S configurarions
 */
typedef struct
{
    i2s_rdma_ctrl_ptr_t *rdma_config;
    i2s_wdma_ctrl_ptr_t *wdma_config;
    i2s_sample_rate_t    sr;
    i2s_ch_format_t      ch;
    i2s_trx_mode_t       trx_mode;
    i2s_fmt_t            fmt;
    i2s_cfg_txd_wid_t    width;
    i2s_cfg_bck_ratio_t  bck_ratio;
    i2s_mclk_div_t       mck_div;
    i2s_cfg_bck_osr_t    bck_osr;
    i2s_cfg_imck_rate_t  imck_rate;
} i2s_para_set_t;

typedef struct
{
    uint8_t     flag;
    uint16_t    size;
    uint32_t    index;
    uint16_t    offset;
    uint32_t    address;
} i2s_buff_ptr_t;
/**************************************************************************************************
 *    GLOBAL PROTOTYPES
 *************************************************************************************************/
/**
 * @brief Register the sadc interrupt callback service routine function
 * @param[in] sadc_int_callback config the interrupt callback service routine  function
 * @return None
 */
void i2s_Register_Int_Callback(i2s_isr_handler_t i2s_int_callback);
/**
 * @brief The initial config function according to the i2s_para to set i2s registers
 *        The i2s_pars can reference the following parameter
 * @param[in] i2s_para I2S configurarions value
                \ref i2s_rdma_ctrl_ptr_t* RDMA configurarions
                \ref i2s_wdma_ctrl_ptr_t* WDMA configurarions
                \ref i2s_sample_rate_t    I2S Simple Rate configurarions
                \ref i2s_ch_format_t      channel mode configurarions ( Stereo / Left channel only / Right channel only)
                \ref i2s_trx_mode_t       I2C trx_mode 0: TxRx 1: Tx only 2: Rx only 3: TxRx
                \ref i2s_fmt_t            I2S format (LJ/RJ/I2S Mode)
                \ref i2s_cfg_txd_wid_t    I2S width  0: 16-bit;1: 24-bit; 2: 32-bit 3: 32-bit
                \ref i2s_cfg_bck_ratio_t  I2S BLCK / WRCK ratio
                \ref i2s_mclk_div_t       I2S Output MCLK divider
                \ref i2s_cfg_bck_osr_t    Ratio of MCLK and BCLK (I2S_MCLK = I2S_BCK * Ratio)
                \ref i2s_cfg_imck_rate_t  Internal MCLK rate WCLK
 * @return
 */
uint32_t I2s_Init(i2s_para_set_t *i2s_para);

/**
 * @brief Function reset i2s registers and disable IRQ, clear and disable rdma and wdam IRQ
 * @return
 */
uint32_t I2s_Uninit(void);

/**
 * @brief Function according to the i2s_pars to enable WDMA and RDMA and to set the i2s and mclk registers
 * @param[in] i2s_para I2S configurarions trx_mode value
                \ref i2s_trx_mode_t I2C trx_mode 0: TxRx 1: Tx only 2: Rx only 3: TxRx
 * @retval STATUS_SUCCESS   config the i2s enable and mclk flag is invaild
 * @retval STATUS_INVALID_PARAM config the i2s_xdma ctrl is invaild
 */
uint32_t I2s_Start(i2s_para_set_t *i2s_para);

/**
 * @brief Function clear the i2s enable and mclk registers
 * @retval STATUS_SUCCESS clear the i2s register is successful
 */
uint32_t I2s_Stop(void);

#ifdef __cplusplus
}
#endif

#endif /* End of _I2S_H_ */

/** @} */ /* End of Peripheral Group */
