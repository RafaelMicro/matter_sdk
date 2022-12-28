// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
#ifndef __ATT_H__
#define __ATT_H__

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
//                Include (Better to prevent)
//=============================================================================
#include <stdint.h>
#include "ble_att_gatt.h"

#define SIZE_DEFAULT_ATT_MTU                23
#define SIZE_PLANNED_ATT_MTU                247

typedef struct Pairs_Handle_typeUUID16
{
    uint16_t  Attr_Handle;
    uint16_t  uuid;
} Pairs_Handle_typeUUID16;


typedef struct Pairs_Handle_typeUUID128
{
    uint16_t  Attr_Handle;
    uint16_t  uuid[8];
} Pairs_Handle_typeUUID128;



typedef struct Hdl_Info_List
{
    uint16_t  Attr_Handle_found;
    uint16_t  Group_Handle_end;
} __attribute__((packed)) Hdl_Info_List;


typedef struct ATTR_OP_Error_Response
{
    uint8_t   Attr_Opcode;
    uint8_t   Req_Opcode;
    uint16_t  Attr_HandleInError;
    uint8_t   ErrorCode;
} ATTR_OP_Error_Response;


typedef struct ATTR_OP_Exchange_MTU_Request
{
    uint8_t   Attr_Opcode;
    uint16_t  MTU_ClientRx __attribute__((packed));
} ATTR_OP_Exchange_MTU_Request;


typedef struct ATTR_OP_Exchange_MTU_Response
{
    uint8_t   Attr_Opcode;
    uint16_t  MTU_ServerRx __attribute__((packed));
} ATTR_OP_Exchange_MTU_Response;


typedef struct ATTR_OP_Find_Information_Request
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl_Starting __attribute__((packed));
    uint16_t  Hdl_Ending __attribute__((packed));
} ATTR_OP_Find_Information_Request;


typedef struct ATTR_OP_Find_Information_Response
{
    uint8_t   Attr_Opcode;
    uint8_t   Type_Format;
    union
    {
        Pairs_Handle_typeUUID16     Pairs_Handle_typeUUID16[61];
        Pairs_Handle_typeUUID128    Pairs_Handle_typeUUID128[13];
        uint8_t Data[244];
    } Info;
} ATTR_OP_Find_Information_Response;


typedef struct ATTR_OP_Find_By_Type_Value_Request
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl_Starting __attribute__((packed));
    uint16_t  Hdl_Ending __attribute__((packed));
    uint16_t  Attr_Type __attribute__((packed));
    uint16_t  Attr_Value[8] __attribute__((packed));
} ATTR_OP_Find_By_Type_Value_Request;


typedef struct ATTR_OP_Find_By_Type_Value_Response
{
    uint8_t   Attr_Opcode;
    uint8_t   Hdl_Info_List[SIZE_PLANNED_ATT_MTU - 1];
} ATTR_OP_Find_By_Type_Value_Response;


typedef struct ATTR_OP_Read_By_Type_Request
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl_Starting __attribute__((packed));
    uint16_t  Hdl_Ending __attribute__((packed));
    uint16_t  typeUUID[8] __attribute__((packed));
} ATTR_OP_Read_By_Type_Request;


typedef struct ATTR_OP_Read_By_Type_Response
{
    uint8_t   Attr_Opcode;
    uint8_t   length_pair;
    uint8_t   Attr_Data_pair[SIZE_PLANNED_ATT_MTU - 2];
} ATTR_OP_Read_By_Type_Response;


typedef struct ATTR_OP_Read_Request
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl __attribute__((packed));
} ATTR_OP_Read_Request;


typedef struct ATTR_OP_Read_Response
{
    uint8_t   Attr_Opcode;
    uint8_t   Attr_Data[SIZE_PLANNED_ATT_MTU - 1];
} ATTR_OP_Read_Response;


typedef struct ATTR_OP_Read_Blob_Request
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl __attribute__((packed));
    uint16_t  value_offset __attribute__((packed));
} ATTR_OP_Read_Blob_Request;


typedef struct ATTR_OP_Read_Blob_Response
{
    uint8_t   Attr_Opcode;
    uint8_t   Attr_Data_part[SIZE_PLANNED_ATT_MTU - 1];
} ATTR_OP_Read_Blob_Response;


typedef struct ATTR_OP_Read_Multiple_Request
{
    uint8_t   Attr_Opcode;

} ATTR_OP_Read_Multiple_Request;


typedef struct ATTR_OP_Read_Multiple_Response
{
    uint8_t   Attr_Opcode;
} ATTR_OP_Read_Multiple_Response;


typedef struct ATTR_OP_Read_by_Group_Type_Request
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl_Starting __attribute__((packed));
    uint16_t  Hdl_Ending __attribute__((packed));
    uint16_t  typeUUID[8] __attribute__((packed));
} ATTR_OP_Read_by_Group_Type_Request;


typedef struct ATTR_OP_Read_by_Group_Type_Response
{
    uint8_t   Attr_Opcode;
    uint8_t   length_list;
    uint8_t  Attr_Data_list[SIZE_PLANNED_ATT_MTU - 2];
} ATTR_OP_Read_by_Group_Type_Response;


typedef struct ATTR_OP_Write_Request
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl __attribute__((packed));
    uint8_t   Attr_Data[SIZE_PLANNED_ATT_MTU - 3];
} ATTR_OP_Write_Request;


typedef struct ATTR_OP_Write_Response
{
    uint8_t   Attr_Opcode;

} ATTR_OP_Write_Response;


typedef struct ATTR_OP_Write_Command
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl __attribute__((packed));
    uint8_t   Attr_Data[SIZE_PLANNED_ATT_MTU - 3];
} ATTR_OP_Write_Command;



typedef struct ATTR_OP_Prepare_Write_Request
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl __attribute__((packed));
    uint16_t  value_offset __attribute__((packed));
    uint8_t   Attr_Data_part[SIZE_PLANNED_ATT_MTU - 5];
} ATTR_OP_Prepare_Write_Request;


typedef struct ATTR_OP_Prepare_Write_Response
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl __attribute__((packed));
    uint16_t  value_offset __attribute__((packed));
    uint8_t   Attr_Data_part[SIZE_PLANNED_ATT_MTU - 5];
} ATTR_OP_Prepare_Write_Response;


typedef struct ATTR_OP_Execute_Write_Request
{
    uint8_t   Attr_Opcode;
    uint8_t   flag;
} ATTR_OP_Execute_Write_Request;


typedef struct ATTR_OP_Execute_Write_Response
{
    uint8_t   Attr_Opcode;

} ATTR_OP_Execute_Write_Response;


typedef struct ATTR_OP_Handle_Value_Notification
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl __attribute__((packed));
    uint8_t   Attr_Data[SIZE_PLANNED_ATT_MTU - 3];
} ATTR_OP_Handle_Value_Notification;


typedef struct ATTR_OP_Handle_Value_Indication
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl __attribute__((packed));
    uint8_t   Attr_Data[SIZE_PLANNED_ATT_MTU - 3];
} ATTR_OP_Handle_Value_Indication;


typedef struct ATTR_OP_Handle_Value_Confirmation
{
    uint8_t   Attr_Opcode;

} ATTR_OP_Handle_Value_Confirmation;


typedef struct ATTR_OP_General
{
    uint8_t   Attr_Opcode;
    uint8_t   Attr_Data[512];

} ATTR_OP_General;


typedef struct ATTR_OP_General_Hdl
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl __attribute__((packed));
    uint8_t   Attr_Data[512];

} ATTR_OP_General_Hdl;


typedef struct ATTR_OP_General_Req
{
    uint8_t   Attr_Opcode;
    uint16_t  Hdl_Starting __attribute__((packed));
    uint16_t  Hdl_Ending __attribute__((packed));

} ATTR_OP_General_Req;


typedef struct ATTR_OP_VALUE_DataPair_UUID128_CHARACTERISTIC
{
    uint16_t  Hdl __attribute__((packed));
    uint8_t   Property;
    uint16_t  Hdl_Value __attribute__((packed));
    uint16_t  typeUUID[8] __attribute__((packed));
} ATTR_OP_VALUE_DataPair_UUID128_CHARACTERISTIC;


typedef struct ATTR_OP_VALUE_DataPair_UUID16_CHARACTERISTIC
{
    uint16_t  Hdl __attribute__((packed));
    uint8_t   Property;
    uint16_t  Hdl_Value __attribute__((packed));
    uint16_t  typeUUID __attribute__((packed));
} ATTR_OP_VALUE_DataPair_UUID16_CHARACTERISTIC;

//=============================================================================
//                Public Function Declaration
//=============================================================================
extern const ble_att_role_by_id_t att_db_link[];                          // defined in bleAppProfile.c
extern const ble_att_handle_param_t ATTRIBUTE_BLE_HDL_PARA_DEFAULT;     // defined in bleAppProfile.c
extern const ble_att_db_mapping_by_id_t att_db_mapping[];                    // defined in bleAppProfile.c
extern const ble_att_db_mapping_by_id_size_t att_db_mapping_size[];          // defined in bleAppProfile.c

extern int8_t (* const Prcss_BLE_OPCODE_ATT[])(void);
extern int8_t Prcss_OPCODE_ATT_Write_Command(void);
extern int8_t Prcss_OPCODE_ATT_Signed_Write_Command(void);

extern int8_t setBLE_ConnTxData(uint16_t connID, uint8_t *ScanRspData, uint8_t Length);
extern int8_t ATT_Request_ERROR_RESPONSE(uint16_t connID, uint8_t ReqOpcode_inError, uint16_t AttHandle_inError, uint8_t ErrorCode);
extern int8_t ATT_Request(uint16_t connID, uint8_t AttOpcode, uint8_t *paramGroup,  uint16_t dataLength);

#ifdef __cplusplus
};
#endif
#endif    /* __ATT_H__ */
