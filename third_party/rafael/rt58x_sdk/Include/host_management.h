// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
#ifndef __HOST_MANAGEMENT_H__
#define __HOST_MANAGEMENT_H__

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
//                Include (Better to prevent)
//=============================================================================
#include <stdint.h>
#include "ble_hci_event.h"
#include "ble_event.h"

#define HS_DBP_S0                               0x00                //used on state_DB_parse;
#define HS_DBP_S1                               (HS_DBP_S0 + 1)
#define HS_DBP_S2                               (HS_DBP_S0 + 2)
#define HS_DBP_S3                               (HS_DBP_S0 + 3)
#define HS_DBP_S4                               (HS_DBP_S0 + 4)
#define HS_DBP_S252                             (HS_DBP_S0 + 252)
#define HS_DBP_S253                             (HS_DBP_S0 + 253)
#define HS_DBP_S254                             (HS_DBP_S0 + 254)
#define HS_DBP_S255                             (HS_DBP_S0 + 255)

#define HS_ATE_S0                               0x00                //used on state_authe;
#define HS_ATE_S1                               (HS_ATE_S0 + 1)
#define HS_ATE_S2                               (HS_ATE_S0 + 2)
#define HS_ATE_S3                               (HS_ATE_S0 + 3)
#define HS_ATE_S4                               (HS_ATE_S0 + 4)
#define HS_ATE_S253                             (HS_ATE_S0 + 253)
#define HS_ATE_S255                             (HS_ATE_S0 + 255)

#define HS_ATO_S0                               0x00                //used on state_autho;
#define HS_ATO_S1                               (HS_ATO_S0 + 1)
#define HS_ATO_S2                               (HS_ATO_S0 + 2)
#define HS_ATO_S252                             (HS_ATO_S0 + 252)
#define HS_ATO_S255                             (HS_ATO_S0 + 255)

#define STATE_ATT_MTU_OPR_MSK_REQ_RDY           0x01
#define STATE_ATT_MTU_OPR_MSK_RSP_RDY           0x02
#define STATE_ATT_MTU_OPR_MSK_X_ING             0x04

#define ATT_RSP_FORMAT_16UUID                   0x01  /**< 16-bits UUID. */
#define ATT_RSP_FORMAT_128UUID                  0x02  /**< 128-bits UUID. */

#define BLE_CONNID_RESERVED                     0x0000

typedef struct __attribute__((packed))
{
    uint16_t  Hdl_Starting;
    uint16_t  Hdl_Ending;
    uint16_t  typeUUID;
    uint16_t  idx_Starting_DB;
    uint16_t  idx_Ending_DB;
    uint16_t  idx_Starting_DB_CharcGrp[3];
    uint16_t  idx_Ending_DB_CharcGrp[3];
}
Pair_ATT_Hdl_Parsing_Param;

typedef struct __attribute__((packed))
{
    uint8_t host_id;
}
MHS_ATT_DB_Parse_Carry_On_Para;

typedef struct __attribute__((packed))
{
    uint8_t host_id;
    uint8_t Command_State;
    uint8_t report_status;
}
MHS_ATT_Authe_Carry_On_Para;

typedef struct __attribute__((packed))
{
    uint8_t host_id;
}
MHS_ATT_Autho_Carry_On_Para;

typedef struct __attribute__((packed))
{
    uint16_t att_Hdl;
    uint16_t length;
    uint8_t  error_code;
    uint8_t  att_data[];
}
MHS_ATT_Queue_Data_Pkt_Para;

typedef struct __attribute__((packed))
{
    uint16_t conn_handle;
    uint16_t length;
    uint8_t  data_idx0;
    uint8_t  data_idx1;
    uint8_t  acl_data[48 - 6];
}
ble_hci_acl_data_evt_t;

/* Message block */
typedef struct MBLK
{
    void *next;          /* point to next mblk*/
    uint16_t primitive;  /* primitive type */
    uint16_t length;     /* total data length */
    union
    {
        uint8_t Data[52];
        ble_hci_evt_param_disconn_complete_t                          hci_evt_disconn_complete;
        ble_hci_evt_param_encrypt_change_t                            hci_evt_encrypt_change;
        ble_hci_evt_param_cmd_complete_t                              hci_evt_cmd_complete;
        ble_hci_evt_param_cmd_status_t                                hci_evt_cmd_status;
        ble_hci_evt_param_key_refresh_complete_t                      hci_evt_key_refresh_complete;
        ble_hci_le_meta_evt_t                                         hci_le_meta_evt;
        ble_hci_acl_data_evt_t                                        hci_acl_data_evt;
        ble_evt_param_t                                               evt_param;
        //        struct LLHC_LE_Ctrl_Pkt_Para                        LLHC_LE_Ctrl_Pkt_Para;
        //        struct HCLL_LE_Ctrl_Pkt_Para                        HCLL_LE_Ctrl_Pkt_Para;
        //        struct HCLL_LE_Ctrl_Pkt_Conn_Update_Para            HCLL_LE_Ctrl_Pkt_Conn_Update_Para;
        //        struct HCLL_LE_Ctrl_Pkt_Set_Host_Ch_Para            HCLL_LE_Ctrl_Pkt_Set_Host_Ch_Para;
        //        struct MLL_LE_Conn_Para_SMP                         MLL_LE_Conn_Para_SMP;
        //        struct MLL_LL_Enc_Req_Para                          MLL_LL_Enc_Req_Para;
        //        struct MLL_LE_CCM_Manual_Para                       MLL_LE_CCM_Manual_Para;
        //        struct LLEC_LE_CCM_Manual_Para                      LLEC_LE_CCM_Manual_Para;
        //        struct ble_hci_evt_param_cmd_complete_s                        MHC_Cmd_Complete_Para;
        //        struct ble_hci_evt_param_cmd_status_s                          MHC_Cmd_Status_Para;
        //        struct ble_hci_evt_param_disconn_complete_s                    MHC_Disconn_Complete_Para;
        //        struct ble_hci_evt_param_num_of_complete_packets_s              MHC_Num_Of_Completed_Pckts_Para;
        //        struct MHC_Read_Remote_Ver_Info_Complete_Para       MHC_Read_Remote_Ver_Info_Complete_Para;
        //        struct ble_hci_evt_param_encrypt_change_s                      MHC_Encrypt_Change_Para;
        //        struct ble_hci_evt_param_key_refresh_complete_s        MHC_Encrypt_Key_Refresh_Complete_Para;
        //        struct MHC_Data_Buffer_Overflow_Para                MHC_Data_Buffer_Overflow_Para;
        //        struct MHC_Hardware_Error_Para                      MHC_Hardware_Error_Para;
        //        struct ble_hci_meta_evt_param_conn_complete_s       MHC_Le_Conn_Complete_Para;
        //        struct ble_hci_meta_evt_param_adv_report_s          MHC_Le_Adv_Report_Para;
        //        struct ble_hci_meta_evt_param_conn_update_s         MHC_Le_Conn_Update_Complete_Para;
        //        struct MHC_Le_Read_Remote_Used_Feat_Complete_Para   MHC_Le_Read_Remote_Used_Feat_Complete_Para;
        //        struct ble_hci_meta_evt_param_long_term_key_req_s                MHC_Le_Long_Term_Key_Req_Para;
        //        struct MHC_Le_Remote_Conn_param_Req_Event_Para      MHC_Le_Remote_Conn_param_Req_Event_Para;
        //        struct MHC_Authen_Payload_Timeout_Expired_Para      MHC_Authen_Payload_Timeout_Expired_Para;
        //        struct MHC_Event_HCI_Raw_Para                       MHC_Event_HCI_Raw_Para;
        //        struct ble_hci_meta_evt_param_data_length_change_s               MHC_Le_Data_Length_Change_Para;
        //        struct ble_hci_meta_evt_param_phy_update_complete_s              MHC_Le_PHY_Update_Complete_Para;
        //        struct HCLL_Le_Read_Phy_Para                        HCLL_Le_Read_Phy_Para;
        //        struct HCLL_Le_Set_Default_Phy_Para                 HCLL_Le_Set_Default_Phy_Para;
        //        struct HCLL_Le_Set_Phy_Para                         HCLL_Le_Set_Phy_Para;
        //        struct HCLL_LE_Set_Data_Length_Para                 HCLL_LE_Set_Data_Length_Para;
        //        struct MLL_LL_CHN_Map_Upd_Para                      MLL_LL_CHN_Map_Upd_Para;
        //        struct PFHS_Att_Chk_Client_Tab_Para                 PFHS_Att_Chk_Client_Tab_Para;
        //        struct HSPF_Att_Cfm_Client_Tab_Para                 HSPF_Att_Cfm_Client_Tab_Para;
        MHS_ATT_Queue_Data_Pkt_Para                  MHS_ATT_Queue_Data_Pkt_Para;
        MHS_ATT_DB_Parse_Carry_On_Para               MHS_ATT_DB_Parse_Carry_On_Para;
        MHS_ATT_Authe_Carry_On_Para                  MHS_ATT_Authe_Carry_On_Para;
        MHS_ATT_Autho_Carry_On_Para                  MHS_ATT_Autho_Carry_On_Para;
        Pair_ATT_Hdl_Parsing_Param                   Pair_ATT_Hdl_Parsing_Param;
        //        struct MHC_Le_Stk_Gen_Para                          MHC_Le_Stk_Gen_Para;
        //        struct MHC_Le_Auth_status_Para                      MHC_Le_Auth_status_Para;
    } para;             /* parameters */
} MBLK;
#define MAX_MBLK_NO         150
#define SIZE_MBLK           (sizeof(MBLK))
#define SIZE_MBLK_ACL_DATA_UNIT (52/*sizeof(MBLK.para.Data)*/ - 2/*connection handle*/ - 2/*length*/ - 2/*idx*/)
#define MAX_MBLK_RSV_L2     68
#define MAX_MBLK_RSV_L1     20

typedef struct LE_Host_Para
{
    uint16_t  LL_ConnID;                                    //addr: 00,
    uint8_t   Role;                                         //addr: 02, 0: master, 1:slave, ref.HCI_ROLE_MASTER
    uint8_t   PeerAddrType;                                 //addr: 03, 0: public, 1:random, ref.HCI_ADDR_TYPE_PUBLIC
    uint8_t   PeerAddr[6];                                  //addr: 04
    uint16_t  ConnInterval;                                 //addr: 0A
    uint16_t  ConnLatency;                                  //addr: 0C
    uint16_t  SvisionTimeout;                               //addr: 0E
    uint16_t  att_MTU;                                      //addr: 10
    uint16_t  att_MTU_planed;                               //addr: 12
    uint8_t   state_authe;                                  //addr: 14, Authentication
    uint8_t   state_autho;                                  //addr: 15, Authorization
    uint8_t   state_DB_parse;                               //addr: 16
    uint8_t   state_att_MTU_opr;                            //addr: 17
    uint16_t  numHdl_fc_cl;                                 //addr: 18, flow control, client
    uint16_t  numHdl_fc_sr;                                 //addr: 1A, flow control, server
    MBLK    *mblk_Param_L0;
    MBLK    *mblk_Param_L1;
    MBLK    *mblk_Q_4_Wr;
    uint8_t   state_pairing;
    uint8_t   private_addr24[3];

    uint8_t   Smp_Phase;
    uint8_t   IsEncryption;
    uint8_t   SecurityMode;

    uint8_t   Remote_IOCapab;
    uint8_t   Remote_OOBDataFlag;
    uint8_t   Remote_AuthReq;
    uint8_t   Remote_MaxEncKeySize;
    uint8_t   Remote_InitiatorKeyDistrib;
    uint8_t   Remote_ResponserKeyDistrib;
    uint8_t   RandomValue[16];
    uint8_t   Confirm_STK[16];

    uint8_t   BOND_Role;
    uint8_t   SMP_STKGenMethod;
    uint8_t   MaxEncKeySize;
    uint8_t   KeyType;
    uint8_t   PeerRAND[8];
    uint8_t   PeerEDIV[2];
    uint8_t   PeerLTK_Passkey[16];
    uint8_t   PeerIRK_M_S_RAND[16];
    uint8_t   IDAddrType;
    uint8_t   IDAddr[6];

    uint8_t   OwnAddrType;
    uint8_t   OwnAddr[6];
    uint8_t   OwnRAND[8];
    uint8_t   OwnEDIV[2];
    uint8_t   OwnLTK[16];
    uint8_t   OwnIRK[16];
} LE_Host_Para;
#define SIZE_LE_HOST_PARA        (sizeof(LE_Host_Para))

typedef struct LE_Host_Exist
{
    LE_Host_Para    *Host_central;
    LE_Host_Para    *Host_Peripheral;
} LE_Host_Exist;

typedef struct ble_irk_rpa_addr_s
{
    uint8_t rpa_addr[6];
    uint8_t irk[16];
} ble_irk_rpa_addr_t;



extern LE_Host_Para *LE_Host;
extern LE_Host_Exist LE_Host_Active;
extern uint8_t smp_Para_Bond_tmp[128];
extern uint8_t acl_data_tx[251];

extern uint8_t bhc_query_host_id_by_conn_id(uint16_t conn_id);
extern void HS_Msg_ATT_DB_PARSE_CARRY_ON(uint8_t host_id);
extern void bhc_host_release_db_parsing_resource(uint8_t host_id);
extern void HS_Msg_ATT_DB_PARSE_CARRY_ON(uint8_t host_id);
extern void bhc_host_param_init(uint8_t host_id);
extern uint8_t chkATT_DB(uint8_t host_id, uint16_t conn_id);
extern void MHS_ATT_DB_Parse_Carry_On(void *pMBlk);
extern void bhc_att_queue_write_resource(uint8_t host_id);
extern void Chk_BOND_StorageOrNot(uint8_t host_id);
extern uint8_t bhc_host_privacy_private_addr_scan_check(ble_gap_addr_t *addr_param);
#ifdef __cplusplus
};
#endif

#endif    /* __HOST_MANAGEMENT_H__ */
