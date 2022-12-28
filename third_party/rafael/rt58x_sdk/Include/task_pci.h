/**
 * @defgroup task_pci TASK PCI
 * @ingroup RF_FW_Control_group
 * @{
 * @brief Define task PCI definitions, structures, and functions.
 * @}
 */

#ifndef __TASK_PCI_H__
#define __TASK_PCI_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "sys_arch.h"
#include "project_config.h"
#include "rf_mcu.h"
#include "rf_common_init.h"

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/

#define TX_SW_QUEUE_FULL             (0xF1)     /**< TX status response by SW, freeRTOS queue full. */
#define TX_MALLOC_FAIL               (0xF2)     /**< TX status response by SW, memory allocation fail. */

#define NUM_QUEUE_PCI_DATA           (10)       /**< Maximum size of PCI data queue. */
#define NUM_QUEUE_PCI_CMD            (10)       /**< Maximum size of PCI command queue. */

#define PCI_ARRAY_MAX_LENGTH         (268)      /**< Maximum data length get from HW. */

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/**
 * @brief The different message types of PCI task.
 * @ingroup task_pci
 */

typedef enum
{
    PCI_MSG_BASE = 0x0020,       /**< Message base that send to PCI task */
    PCI_MSG_TX_PCI_CMD,          /**< Message for upper layer to notify PCI TX PCI command */
    PCI_MSG_TX_PCI_DATA,         /**< Message for upper layer to notify PCI TX PCI data */
    PCI_MSG_MAX = 0x0030,
} pci_msg_tag_def_t;

/**
 * @brief The parameter definition of PCI task transmit PCI command.
 * @ingroup task_pci
 */

typedef struct __attribute__((packed))
{
    uint8_t     ruci_header;  /**< RUCI header. */
    uint8_t     sub_header;   /**< RUCI sub-header. */
    uint8_t     length;       /**< Parameter length. */
    uint8_t     parameter[];  /**< Parameters. */
}
pci_15p4_cmd_hdr_t;

/**
 * @brief The parameter definition of PCI task transmit PCI data.
 * @ingroup task_pci
 */

typedef struct __attribute__((packed))
{
    uint8_t    ruci_header;  /**< RUCI header. */
    uint8_t    sub_header;   /**< RUCI sub-header. */
    uint16_t   length;       /**< Parameter length. */
    uint8_t    mac_control;  /**< MAC control field. */
    uint8_t    mac_dsn;      /**< MAC data sequence field. */
    uint8_t    parameter[];  /**< Parameters. */
}
pci_15p4_data_hdr_t;

/**
 * @brief All messages that PCI task can handle.
 * @ingroup task_pci
 */

typedef struct
{
    union
    {
        uint8_t             pci_array[PCI_ARRAY_MAX_LENGTH];    /**< PCI array for read data from communication subsystem. */
        pci_15p4_cmd_hdr_t  pci_tx_hdr;                         /**< PCI command format send to controller. */
        pci_15p4_data_hdr_t pci_tx_data_hdr;                    /**< PCI data format send to controller. */
    } msg_type;
} pci_message_struct_t;

/**
 * @brief The message sub-parameter of PCI task.
 * @ingroup task_pci
 */

typedef struct
{
    union
    {
        pci_message_struct_t  *pci_msg_ptr;    /**< pci sub-parameter pointer.  */
        uint32_t              pci_msg_var;     /**< pci sub-parameter variable. */
    } param_type;
} pci_message_param_type_struct_t;

/**
 * @brief The message parameter of PCI task.
 * @ingroup task_pci
 */

typedef struct
{
    uint32_t pci_msg_tag;                      /**< pci message tag. */
    pci_message_param_type_struct_t pci_msg;   /**< pci message parameter. */
} pci_task_common_queue_t;

/**************************************************************************
 * EXTERN DEFINITIONS
 **************************************************************************/
/**@brief The handle of PCI common queue.
 * @ingroup task_pci
 */
extern sys_queue_t g_pci_common_handle;


/**@brief The handle of PCI TX data queue.
 * @ingroup task_pci
 */
extern sys_queue_t g_pci_tx_data_handle;


/**@brief The handle of PCI TX command queue.
 * @ingroup task_pci
 */
extern sys_queue_t g_pci_tx_cmd_handle;


/**************************************************************************************************
 *    PUBLIC FUNCTIONS
 *************************************************************************************************/

/**@brief PCI task initialization.
 *
 * @ingroup task_pci
 *
 * @return none
 */
void task_pci_init(void);

/**@brief Upper layer fail to send data to PCI data queue.
 *
 * @ingroup task_pci
 *
 * @return none
 */
void send_data_to_pci_fail(uint8_t reason);


#ifdef __cplusplus
};
#endif

#endif /* __TASK_PCI_H__*/
