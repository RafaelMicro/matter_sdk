/**************************************************************************//**
 * @file     dma.h
 * @version
 * @brief    DMA driver header file
 *
 * @copyright
 ******************************************************************************/
#ifndef ___DMA_H__
#define ___DMA_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @defgroup DMA_Driver DMA
 *  @ingroup  peripheral_group
 *  @{
 *  @brief  DMA information
 */




#define  DMA_MODE32           2                 /*!< DMA move 32-bits width data    */
#define  DMA_MODE8            0                 /*!< DMA move 8-bits width data     */


#define  DMA_CNTL_SRC_FIXED         (1U<<31)     /*!< DMA source address is fixed     */
#define  DMA_CNTL_SRC_INC           (0U<<31)     /*!< DMA source address is increment  */

#define  DMA_CNTL_SRC_WIDTH_8BIT    (0<<28)      /*!< DMA source AHB HSIZE  8-bits      */
#define  DMA_CNTL_SRC_WIDTH_16BIT   (1<<28)      /*!< DMA source AHB HSIZE  16-bits     */
#define  DMA_CNTL_SRC_WIDTH_32BIT   (2<<28)      /*!< DMA source AHB HSIZE  32-bits, recommend used this setting */


#define  DMA_CNTL_SRC_BURST8        (5<<25)      /*!< DMA source AHB HBURST  8-beat, recommend used this setting */
#define  DMA_CNTL_SRC_BURST4        (3<<25)      /*!< DMA source AHB HBURST  3-beat   */
#define  DMA_CNTL_SRC_BURST1        (0<<25)      /*!< DMA source AHB HBURST  1-beat,  */

#define  DMA_CNTL_SRC_PORT          (2<<21)      /*!< \hideinitializer */

#define  DMA_CNTL_DEST_FIXED        (1<<15)      /*!< DMA destination address is fixed     */
#define  DMA_CNTL_DEST_INC          (0<<15)      /*!< DMA destination address is increment     */

#define  DMA_CNTL_DEST_WIDTH_8BIT   (0<<12)      /*!< DMA destination AHB HSIZE  8-bits      */
#define  DMA_CNTL_DEST_WIDTH_16BIT  (1<<12)      /*!< DMA destination AHB HSIZE  16-bits      */
#define  DMA_CNTL_DEST_WIDTH_32BIT  (2<<12)      /*!< DMA destination AHB HSIZE  32-bits, recommend used this setting */


#define  DMA_CNTL_DEST_BURST8       (5<<9)       /*!< DMA destination AHB HBURST  8-beat, recommend used this setting */
#define  DMA_CNTL_DEST_BURST4       (3<<9)       /*!< DMA destination AHB HBURST  3-beat */
#define  DMA_CNTL_DEST_BURST1       (0<<9)       /*!< DMA destination AHB HBURST  1-beat */

#define  DMA_CNTL_DEST_PORT         (1<<5)       /*!< \hideinitializer */

#define  DMA_CNTL_REPEAT_MODE       (1<<3)       /*!< \hideinitializer */
#define  DMA_CNTL_SINGLE_MODE       (0<<3)       /*!< \hideinitializer */

#define  DMA_CNTL_MODE_MEMTOMEM     (0<<1)       /*!< DMA is using for memory to memory */


#define  DMA_CNTL_LINKLIST_ENABLE   (1)          /*!< DMA uses LINKLIST move mode. */
#define  DMA_CNTL_LINKLIST_DISABLE  (0)          /*!< DMA uses normal data move mode.  */


#define  DMA_CNTL_LINKLIST_END      (0xFFFFFFFF)    /*!< The last DMA entry index for LINKLIST mode. */


#define  DMA_INT_XFER_DONE          (1<<0)          /*!< DMA transfer complete */
#define  DMA_INT_SRC_ERR            (1<<1)          /*!< \hideinitializer */
#define  DMA_INT_DEST_ERR           (1<<2)          /*!< \hideinitializer */
#define  DMA_INT_LL_ELEMENT_Done    (1<<3)          /*!< DMA transfer for single linklist element complete */

#define  DMA_INT_ALL                (0xF)           /*!< DMA transfer using interrupt mode. when all DMA interrupt event happen, DMA notify callback will be called. */


/**@brief DMA config structure for DMA setting
 *
 */
typedef struct
{
    uint32_t   dma_control_setting;                 /*!< DMA hardware request setting */
    uint32_t   dma_src_addr;                        /*!< DMA soruce address in memory */
    uint32_t   dma_dest_addr;                       /*!< DMA destination address in memory */
    uint32_t   dma_xfer_length;                     /*!< DMA transfer length */
    uint32_t   dma_repeat_setting;                  /*!< DMA repeat mode. */
    uint32_t   dma_int_enable;                      /*!< when DMA complete, interrupt will generate or not (interrupt mode or poll mode)  */
} dma_config_t;


/**
 *  @brief DMA linklist mode for DMA setting
 *  This linklist mode is very special dma mode, most application will not use this special setting.
 *  Only if you need to move some "spare data" to become "continous" data, this mode will use.
 *
 */

typedef struct  __attribute__((packed))
{
    uint8_t    *src_ptr;      /*!< linklist source pointer */
    uint8_t    *dest_ptr;     /*!< linklist destination pointer */
    uint32_t   size;          /*!< move bytes number */
}
dma_linklist_request_t;

/**
 * @brief DMA finish routine callback for user application.
 * @param chennel_id:   DMA channel_id
 * @param statue:       DMA transfer status
 *
 */
typedef void (*dma_proc_cb_t)(uint32_t channel_id, uint32_t status);


/**@brief DMA LinkListEntry.
 *   Please do NOT change the declare order  of dma_linklist_entry_t.. it is hardware used setting
 *   Please refer DMA document for the structure setting
 */
typedef struct
{
    uint32_t   src_addr;
    uint32_t   dest_addr;
    uint32_t   transfer_length;
    uint32_t   control_set;
    uint32_t   next_ptr_addr;
    uint32_t   status;
    uint32_t   repeat_set;
    uint32_t   resv;
} dma_linklist_entry_t;


/**
 * @brief DMA interrupt ISR function register. Use to notify channel DMA finished.
 *
 * @param dma_channel_id   Specifies the dma channel number. if dma_channel_id is invalid id, this request will be ignored.
 * @param app_dma_callback
 *
 * @return  NONE
 *
 *
 *
 */
void dma_register_isr(
    uint32_t  dma_channel_id,
    dma_proc_cb_t  app_dma_callback
);

/**
 * @brief DMA config. Use to config normal mode DMA setting.
 *
 * @param dma_channel_id   Specifies the dma channel number.
 * @param *dma_config_t    Specifies the dma setting for normal mode.
 *
 * @return
 * @retval   STATUS_INVALID_PARAM  The dma_channel_id is invalid.
 * @retval   STATUS_SUCCESS        This request setting success.
 *
 */
uint32_t dma_config(
    uint32_t  dma_channel_id,
    const dma_config_t *mode
);

/**
 * @brief DMA enable function.
 *
 * @param dma_channel_id  Specifies the dma channel number.
 *
 * @return
 * @retval   STATUS_INVALID_PARAM  The dma_channel_id is invalid.
 * @retval   STATUS_SUCCESS        This request is in progressing...
 *
 *
 */
uint32_t dma_enable(uint32_t  dma_channel_id);


/**
 * @brief Get DMA transfer status.
 *
 * @param dma_channel_id  Specifies the dma channel number.
 * @param status    DMA tranfer status.
 *
 * @return
 * @retval   STATUS_INVALID_PARAM  The dma_channel_id is invalid.
 * @retval   STATUS_SUCCESS
 *
 *
 */
uint32_t dma_get_status(uint32_t  dma_channel_id, uint32_t *status);

/**
 * @brief Get DMA repeat transfer status.
 *
 * @param dma_channel_id  Specifies the dma channel number.
 * @param repeat_status   DMA tranfer repeat status.
 *
 *
 * @return
 * @retval   STATUS_INVALID_PARAM  The dma_channel_id is invalid.
 * @retval   STATUS_SUCCESS
 *
 */
uint32_t dma_get_repeatstatus(uint32_t  dma_channel_id, uint32_t *repeat_status);

/**
 * @brief DMA finish function. Use this function to clear DMA enable bit and clear flags.
 *  If you use interrupt mode for DMA done, you can ignore this function
 *  Because this function will be call in DMA interrupt.
 *  ONLY call this function when you use polling mode for DMA done.
 *
 * @param dma_channel_id  Specifies the dma channel number.
 *
 * @return
 * @retval   STATUS_INVALID_PARAM  The dma_channel_id is invalid.
 * @retval   STATUS_SUCCESS
 *
 */
uint32_t dma_finish(uint32_t  dma_channel_id);


/**
 * @brief Set Linked List dma move. Use this function to set
 *        dma move for "scatter element"
 *
 * @param dma_channel_id  Specifies the dma channel number.
 * @param dma_lli         The Linked List Item that hardware dma request.
 * @param dma_int_enable  The interrupt enable control register setting.
 *
 */

uint32_t dma_set_link_list(
    uint32_t dma_channel_id,
    dma_linklist_entry_t *dma_lli,
    uint32_t dma_int_enable);

/**
 * @brief Use create_dma_link_list function to create hardware descriptor for dma scatter move
 *
 * @param  dma_linklist_request    pointer to dma_linklist_request_t that software describe data move "instructions"
 * @param  entry_number            number of linklist requests in the dma_linklist_request array
 * @param  dma_link_list           pointer to dma_linklist_entry_t array that will save hardware descriptor setting.
 * @param  control_mode            dma transfer mode
 *
 */
uint32_t create_dma_link_list(
    dma_linklist_request_t  *dma_linklist_request,
    uint32_t                entry_number,
    dma_linklist_entry_t    *dma_link_list,
    uint32_t                control_mode
);

/**
* @brief Set dma channel to init state. Please call this function
*        in system init stage.
*
*
*/
uint32_t dma_init(void);


/*@}*/ /* end of group DMA_Driver */



#ifdef __cplusplus
}
#endif

#endif
