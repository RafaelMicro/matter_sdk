// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
#ifndef __L2CAP_H__
#define __L2CAP_H__

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
//                Include (Better to prevent)
//=============================================================================
#include <stdint.h>
#include "att.h"
#include "smp.h"

//=============================================================================
//                Private Definitions of const value
//=============================================================================
#define L2CAP_CID_NULL_IDENTIFIER                               0x00
#define L2CAP_CID_L2CAP_SIGNALING_CHANNEL                       0x01    //ACL
#define L2CAP_CID_CONNECTIONLESS_CHANNEL                        0x02    //ACL
#define L2CAP_CID_AMP_MANAGER_PROTOCOL                          0x03    //ACL
#define L2CAP_CID_ATTRIBUTE_PROTOCOL                            0x04    //LE
#define L2CAP_CID_LE_L2CAP_SIGNALING_CHANNEL                    0x05    //LE
#define L2CAP_CID_SECURITY_MANAGER_PROTOCOL                     0x06    //LE
#define L2CAP_CID_BREDR_SECURITY_MANAGER                        0x07

#define CODE_SIGNL_CMD_RESERVED                                 0x00
#define CODE_SIGNL_CMD_COMMAND_REJECT                           0x01
#define CODE_SIGNL_CMD_CONNECTION_REQUEST                       0x02
#define CODE_SIGNL_CMD_CONNECTION_RESPONSE                      0x03
#define CODE_SIGNL_CMD_CONFIGURE_REQUEST                        0x04
#define CODE_SIGNL_CMD_CONFIGURE_RESPONSE                       0x05
#define CODE_SIGNL_CMD_DISCONNECTION_REQUEST                    0x06
#define CODE_SIGNL_CMD_DISCONNECTION_RESPONSE                   0x07
#define CODE_SIGNL_CMD_ECHO_REQUEST                             0x08
#define CODE_SIGNL_CMD_ECHO_RESPONSE                            0x09
#define CODE_SIGNL_CMD_INFORMATION_REQUEST                      0x0A
#define CODE_SIGNL_CMD_INFORMATION_RESPONSE                     0x0B
#define CODE_SIGNL_CMD_CREATE_CHANNEL_REQUEST                   0x0C
#define CODE_SIGNL_CMD_CREATE_CHANNEL_RESPONSE                  0x0D
#define CODE_SIGNL_CMD_MOVE_CHANNEL_REQUEST                     0x0E
#define CODE_SIGNL_CMD_MOVE_CHANNEL_RESPONSE                    0x0F
#define CODE_SIGNL_CMD_MOVE_CHANNEL_CONFIRMATION                0x10
#define CODE_SIGNL_CMD_MOVE_CHANNEL_CONFIRMATION_RESPONSE       0x11
#define CODE_SIGNL_CMD_CONNECTION_PARAMETER_UPDATE_REQUEST      0x12
#define CODE_SIGNL_CMD_CONNECTION_PARAMETER_UPDATE_RESPONSE     0x13
#define CODE_SIGNL_CMD_LE_CREDIT_BASED_CONNECTION_REQUEST       0x14
#define CODE_SIGNL_CMD_LE_CREDIT_BASED_CONNECTION_RESPONSE      0x15
#define CODE_SIGNL_CMD_FLOW_CONTROL_CREDIT_IND                  0x16
#define CODE_SIGNL_CMD_CREDIT_BASED_CONNECTION_REQUEST          0x17
#define CODE_SIGNL_CMD_CREDIT_BASED_CONNECTION_RESPONSE         0x18
#define CODE_SIGNL_CMD_CREDIT_BASED_RECONFIGURE_REQUEST         0x19
#define CODE_SIGNL_CMD_CREDIT_BASED_RECONFIGURE_RESPONSE        0x1A

#define SIZE_BASIC_L2CAP_HEADER                                 4
//=============================================================================
//                Public Function Declaration
//=============================================================================
typedef struct __attribute__((packed))
{
    uint16_t    length;
    uint16_t    cid;
    uint8_t     code;
}
ble_l2cap_pkt_hdr_t;

typedef struct __attribute__((packed))
{
    uint16_t    length;
    uint16_t    cid;
    uint8_t     opcode;
}
ble_le_l2cap_pkt_hdr_t;

typedef struct ble_l2cap_buffer_t
{
    uint16_t    conn_handle;
    uint8_t     data[251];         //251: maximun LL MTU after Bluetooth Spec. ver.4.2
} ble_l2cap_buffer_t;

typedef struct L2CAPR_SIGNL_OP_Command_Reject_Response
{
    uint8_t   code;
    uint8_t   identifier;
    uint16_t  length __attribute__((packed));
    uint16_t  reason __attribute__((packed));
} L2CAPR_SIGNL_OP_Command_Reject_Response;

typedef struct L2CAPR_SIGNL_OP_Connection_Parameter_Update_Request
{
    uint8_t   Code;
    uint8_t   identifier;
    uint16_t  Length __attribute__((packed));
    uint16_t  Intervalmin __attribute__((packed));
    uint16_t  Intervalmax __attribute__((packed));
    uint16_t  SlaveLatency __attribute__((packed));
    uint16_t  TimeoutMultiplier __attribute__((packed));
} L2CAPR_SIGNL_OP_Connection_Parameter_Update_Request;

typedef struct L2CAPR_SIGNL_OP_Connection_Parameter_Update_Response
{
    uint8_t   Code;
    uint8_t   Identifier;
    uint16_t  Length __attribute__((packed));
    uint16_t  Result __attribute__((packed));
} L2CAPR_SIGNL_OP_Connection_Parameter_Update_Response;

typedef struct ACL_Data
{
    uint16_t  Length_PDU;
    uint16_t  ChannelID;
    union
    {
        ATTR_OP_Error_Response                  ATTR_OP_Error_Response;
        ATTR_OP_Exchange_MTU_Request            ATTR_OP_Exchange_MTU_Request;
        ATTR_OP_Exchange_MTU_Response           ATTR_OP_Exchange_MTU_Response;
        ATTR_OP_Find_Information_Request        ATTR_OP_Find_Information_Request;
        ATTR_OP_Find_Information_Response       ATTR_OP_Find_Information_Response;
        ATTR_OP_Find_By_Type_Value_Request      ATTR_OP_Find_By_Type_Value_Request;
        ATTR_OP_Find_By_Type_Value_Response     ATTR_OP_Find_By_Type_Value_Response;
        ATTR_OP_Read_By_Type_Request            ATTR_OP_Read_By_Type_Request;
        ATTR_OP_Read_By_Type_Response           ATTR_OP_Read_By_Type_Response;
        ATTR_OP_Read_Request                    ATTR_OP_Read_Request;
        ATTR_OP_Read_Response                   ATTR_OP_Read_Response;
        ATTR_OP_Read_Blob_Request               ATTR_OP_Read_Blob_Request;
        ATTR_OP_Read_Blob_Response              ATTR_OP_Read_Blob_Response;
        ATTR_OP_Read_Multiple_Request           ATTR_OP_Read_Multiple_Request;
        ATTR_OP_Read_Multiple_Response          ATTR_OP_Read_Multiple_Response;
        ATTR_OP_Read_by_Group_Type_Request      ATTR_OP_Read_by_Group_Type_Request;
        ATTR_OP_Read_by_Group_Type_Response     ATTR_OP_Read_by_Group_Type_Response;
        ATTR_OP_Write_Request                   ATTR_OP_Write_Request;
        ATTR_OP_Write_Response                  ATTR_OP_Write_Response;
        ATTR_OP_Write_Command                   ATTR_OP_Write_Command;
        ATTR_OP_Prepare_Write_Request           ATTR_OP_Prepare_Write_Request;
        ATTR_OP_Prepare_Write_Response          ATTR_OP_Prepare_Write_Response;
        ATTR_OP_Execute_Write_Request           ATTR_OP_Execute_Write_Request;
        ATTR_OP_Execute_Write_Response          ATTR_OP_Execute_Write_Response;
        ATTR_OP_Handle_Value_Notification       ATTR_OP_Handle_Value_Notification;
        ATTR_OP_Handle_Value_Indication         ATTR_OP_Handle_Value_Indication;
        ATTR_OP_Handle_Value_Confirmation       ATTR_OP_Handle_Value_Confirmation;
        ATTR_OP_General                         ATTR_OP_General;
        ATTR_OP_General_Hdl                     ATTR_OP_General_Hdl;
        ATTR_OP_General_Req                     ATTR_OP_General_Req;

        smp_op_generic_format_t                 smp_op_generic;
        smp_op_pairing_request_t                smp_op_pairing_request;
        smp_op_pairing_response_t               smp_op_pairing_response;
        smp_op_pairing_confirm_t                smp_op_pairing_confirm;
        smp_op_pairing_random_t                 smp_op_pairing_random;
        smp_op_pairing_failed_t                 smp_op_pairing_failed;
        smp_op_encryption_information_t         smp_op_encryption_information;
        smp_op_master_identification_t          smp_op_master_identification;
        smp_op_identity_information_t           smp_op_identity_information;
        smp_op_identity_addr_information_t      smp_op_identity_addr_information;
        smp_op_signing_information_t            smp_op_signing_information;
        smp_op_security_request_t               smp_op_security_request;
        smp_op_pairing_public_key_t             smp_op_pairing_public_key;
        smp_op_pairing_dhkey_check_t            smp_op_pairing_dhkey_check;
        L2CAPR_SIGNL_OP_Command_Reject_Response                L2CAPR_SIGNL_OP_Command_Reject_Response;
        L2CAPR_SIGNL_OP_Connection_Parameter_Update_Request    L2CAPR_SIGNL_OP_Connection_Parameter_Update_Request;
        L2CAPR_SIGNL_OP_Connection_Parameter_Update_Response   L2CAPR_SIGNL_OP_Connection_Parameter_Update_Response;
    } PDU;
} ACL_Data;

extern ble_l2cap_buffer_t g_l2cap_buffer;
extern uint8_t g_l2cap_dataTx[251];

#ifdef __cplusplus
};
#endif

#endif    /* __L2CAP_H__ */
