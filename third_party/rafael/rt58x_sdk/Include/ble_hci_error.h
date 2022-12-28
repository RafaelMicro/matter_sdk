/** @file ble_hci_error.h
 *
 * @brief BLE HCI error header file.
 *
 */

/**
 * @defgroup ble_hci_error BLE HCI ERROR List
 * @ingroup BLE_group
 * @{
 * @brief Define BLE HCI ERROR definitions.
 * @}
 */

#ifndef __BLE_HCI_ERROR_H__
#define __BLE_HCI_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
/**
 * @defgroup hci_cmd_param_error_code HCI Error Code
 * @{
 * @ingroup ble_hci_error
*/
typedef uint8_t ble_hci_error_code_t;
#define BLE_HCI_ERR_CODE_SUCCESS                                                    0x00    /**< Success. */
#define BLE_HCI_ERR_CODE_UNKNOWN_HCI_COMMAND                                        0x01    /**< Unknown HCI command. */
#define BLE_HCI_ERR_CODE_UNKNOWN_CONNECTION_IDENTIFIER                              0x02    /**< Unknown connection identifier. */
#define BLE_HCI_ERR_CODE_HARDWARE_FAILURE                                           0x03    /**< Hardware failure. */
#define BLE_HCI_ERR_CODE_PAGE_TIMEOUT                                               0x04    /**< Page timeout. */
#define BLE_HCI_ERR_CODE_AUTHENTICATION_FAILURE                                     0x05    /**< Authentication failure. */
#define BLE_HCI_ERR_CODE_PIN_OR_KEY_MISSING                                         0x06    /**< PIN or key missing. */
#define BLE_HCI_ERR_CODE_MEMORY_CAPACITY_EXCEEDED                                   0x07    /**< Memory capacity exceeded. */
#define BLE_HCI_ERR_CODE_CONNECTION_TIMEOUT                                         0x08    /**< Connection timeout. */
#define BLE_HCI_ERR_CODE_CONNECTION_LIMIT_EXCEEDED                                  0x09    /**< Connection limit exceeded. */
#define BLE_HCI_ERR_CODE_SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED          0x0A    /**< Synchronous connection limit to a device exceeded. */
#define BLE_HCI_ERR_CODE_ACL_CONNECTION_ALREADY_EXISTS                              0x0B    /**< Connection already exists. */
#define BLE_HCI_ERR_CODE_COMMAND_DISALLOWED                                         0x0C    /**< Command disallowed. */
#define BLE_HCI_ERR_CODE_CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES               0x0D    /**< Connection rejected due to limited resources. */
#define BLE_HCI_ERR_CODE_CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS                0x0E    /**< Connection rejected due to security reasons. */
#define BLE_HCI_ERR_CODE_CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR            0x0F    /**< Connection rejected due to unacceptable BD_ADDR. */
#define BLE_HCI_ERR_CODE_CONNECTION_ACCEPT_TIMEOUT_EXCEEDED                         0x10    /**< Connection accept timeout exceeded. */
#define BLE_HCI_ERR_CODE_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE                     0x11    /**< Unsupported feature or parameter value. */
#define BLE_HCI_ERR_CODE_INVALID_HCI_COMMAND_PARAMETERS                             0x12    /**< Invalid HCI command parameters. */
#define BLE_HCI_ERR_CODE_REMOTE_USER_TERMINATED_CONNECTION                          0x13    /**< Remote user terminated connection. */
#define BLE_HCI_ERR_CODE_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES   0x14    /**< Remote device terminated connection due to low resources. */
#define BLE_HCI_ERR_CODE_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF       0x15    /**< Remote device terminated connection due to power off. */
#define BLE_HCI_ERR_CODE_CONNECTION_TERMINATED_BY_LOCAL_HOST                        0x16    /**< Connection terminated by local host. */
#define BLE_HCI_ERR_CODE_REPEATED_ATTEMPTS                                          0x17    /**< Repeated attempts. */
#define BLE_HCI_ERR_CODE_PAIRING_NOT_ALLOWED                                        0x18    /**< Pairing not allowed. */
#define BLE_HCI_ERR_CODE_UNKNOWN_LMP_PDU                                            0x19    /**< Unknown LMP PDU. */
#define BLE_HCI_ERR_CODE_UNSUPPORTED_REMOTE_FEATURE_UNSUPPORTED_LMP_FEATURE         0x1A    /**< Unsupported remote feature. */
#define BLE_HCI_ERR_CODE_SCO_OFFSET_REJECTED                                        0x1B    /**< SCO offset rejected. */
#define BLE_HCI_ERR_CODE_SCO_INTERVAL_REJECTED                                      0x1C    /**< SCO interval rejected. */
#define BLE_HCI_ERR_CODE_SCO_AIR_MODE_REJECTED                                      0x1D    /**< SCO air mode rejected. */
#define BLE_HCI_ERR_CODE_INVALID_LMP_PARAMETERS                                     0x1E    /**< Invalid LMP parameters / invalid LL parameters. */
#define BLE_HCI_ERR_CODE_UNSPECIFIED_ERROR                                          0x1F    /**< Unspecified error. */
#define BLE_HCI_ERR_CODE_UNSUPPORTED_LMP_PARAMETER_VALUE                            0x20    /**< Unsupported LMP parameter value / unsupported LL parameter value. */
#define BLE_HCI_ERR_CODE_ROLE_CHANGE_NOT_ALLOWED                                    0x21    /**< Role change not allowed. */
#define BLE_HCI_ERR_CODE_LMP_RESPONSE_TIMEOUT_LL_RESPONSE_TIMEOUT                   0x22    /**< LMP response timeout / LL response timeout. */
#define BLE_HCI_ERR_CODE_LMP_ERROR_TRANSACTION_COLLISION                            0x23    /**< LMP error transaction collision / LL procedure collision. */
#define BLE_HCI_ERR_CODE_LMP_PDU_NOT_ALLOWED                                        0x24    /**< LMP PDU not allowed. */
#define BLE_HCI_ERR_CODE_ENCRYPTION_MODE_NOT_ACCEPTABLE                             0x25    /**< Encryption mode not acceptable. */
#define BLE_HCI_ERR_CODE_LINK_KEY_CANNOT_BE_CHANGED                                 0x26    /**< Link Key cannot be changed. */
#define BLE_HCI_ERR_CODE_REQUESTED_QOS_NOT_SUPPORTED                                0x27    /**< Requested QoS not supported. */
#define BLE_HCI_ERR_CODE_INSTANT_PASSED                                             0x28    /**< Instant passed. */
#define BLE_HCI_ERR_CODE_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED                        0x29    /**< Pairing with unit key not supported. */
#define BLE_HCI_ERR_CODE_DIFFERENT_TRANSACTION_COLLISION                            0x2A    /**< Different transaction collision. */
#define BLE_HCI_ERR_CODE_QOS_UNACCEPTABLE_PARAMETER                                 0x2C    /**< QoS unacceptable parameter. */
#define BLE_HCI_ERR_CODE_QOS_REJECTED                                               0x2D    /**< QoS rejected. */
#define BLE_HCI_ERR_CODE_CHANNEL_CLASSIFICATION_NOT_SUPPORTED                       0x2E    /**< Channel classification not supported. */
#define BLE_HCI_ERR_CODE_INSUFFICIENT_SECURITY                                      0x2F    /**< Insufficient security. */
#define BLE_HCI_ERR_CODE_PARAMETER_OUT_OF_MANDATORY_RANGE                           0x30    /**< Parameter out of mandatory range. */
#define BLE_HCI_ERR_CODE_ROLE_SWITCH_PENDING                                        0x32    /**< Role switch pending. */
#define BLE_HCI_ERR_CODE_RESERVED_SLOT_VIOLATION                                    0x34    /**< Reserved slot violation. */
#define BLE_HCI_ERR_CODE_ROLE_SWITCH_FAILED                                         0x35    /**< Role switch failed. */
#define BLE_HCI_ERR_CODE_EXTENDED_INQUIRY_RESPONSE_TOO_LARGE                        0x36    /**< Extended inquiry response too large. */
#define BLE_HCI_ERR_CODE_SECURE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST                0x37    /**< Secure simple pairing not supported by host. */
#define BLE_HCI_ERR_CODE_HOST_BUSY_PAIRING                                          0x38    /**< Host busy - pairing. */
#define BLE_HCI_ERR_CODE_CONNECTION_REJECTED_DUE_TO_NO_SUITABLE_CHANNEL_FOUND       0x39    /**< Connection rejected due to no suitable channel found. */
#define BLE_HCI_ERR_CODE_CONTROLLER_BUSY                                            0x3A    /**< Controller busy. */
#define BLE_HCI_ERR_CODE_UNACCEPTABLE_CONNECTION_INTERVAL                           0x3B    /**< Unacceptable connection parameters. */
#define BLE_HCI_ERR_CODE_DIRECTED_ADVERTISING_TIMEOUT                               0x3C    /**< Advertising timeout. */
#define BLE_HCI_ERR_CODE_CONNECTION_TERMINATED_DUE_TO_MIC_FAILURE                   0x3D    /**< Connection terminated due to MIC failure. */
#define BLE_HCI_ERR_CODE_CONNECTION_FAILED_TO_BE_ESTABLISHED                        0x3E    /**< Connection failed to be established / synchronization timeout . */
#define BLE_HCI_ERR_CODE_MAC_CONNECTION_FAILED                                      0x3F    /**< MAC connection failed. */
#define BLE_HCI_ERR_CODE_COARSE_CLOCK_ADJUSTMENT_REJECTED                           0x40    /**< Coarse clock adjustment rejected but will try to adjust using clock dragging. */
#define BLE_HCI_ERR_CODE_TYPE0_SUBMAP_NOT_DEFINED                                   0x41    /**< Type0 submap not defined. */
#define BLE_HCI_ERR_CODE_UNKNOWN_ADVERTISING_IDENTIFIER                             0x42    /**< Unknown advertising identifier. */
#define BLE_HCI_ERR_CODE_LIMIT_REACHED                                              0x43    /**< Limit reached. */
#define BLE_HCI_ERR_CODE_OPERATION_CANCELLED_BY_HOST                                0x44    /**< Operation cancelled by host. */
#define BLE_HCI_ERR_CODE_PACKET_TOO_LONG                                            0x45    /**< Packet too long . */
#define BLE_ERR_CODE_CONN_PARAM_UPDATE_REJECTED                                     0xFE    /**< Connection parameter update rejected. */
#define BLE_ERR_CODE_TIMEOUT                                                        0xFF    /**< Command Timeout. */
/** @} */

#ifdef __cplusplus
};
#endif

#endif /* __BLE_HCI_ERROR_H__*/

