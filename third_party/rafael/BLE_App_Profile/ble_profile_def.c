/************************************************************************
 *
 * File Name  : ble_profile_def.c
 * Description: This file contains the definitions of BLE profiles.
 *
 ************************************************************************/
#include "stdio.h"
#include "ble_profile.h"

/**************************************************************************
 * Profile Definitions
 **************************************************************************/

/** Service Combination #00
 * @note included @ref ATT_GAPS_SERVICE @ref ATT_GATTS_SERVICE @ref ATT_DIS_SERVICE
 */
const ble_att_param_t *const att_service_comb00[] =
    {
        &ATT_NULL_INVALID, // mandatory, don't remove it.
        ATT_GAPS_SERVICE,
        ATT_GATTS_SERVICE,
        ATT_DIS_SERVICE,
        ATT_MATTER_SERVICE,
        ATT_TRSPS_SERVICE,
        ATT_FOTAS_SERVICE,
};
/**************************************************************************
 * BLE Connection Link Definitions
 **************************************************************************/

/** BLE Connection Links Definition
 * @attention Do NOT modify the name of this definition.
 * @note If there does not support server or client please set to "((const ble_att_param_t **)0)" which means NULL.
 */
const ble_att_role_by_id_t att_db_link[] =
    {
        // Link 0
        {
            ((const ble_att_param_t **)0), // Client Profile
            att_service_comb00,            // Server Profile
        },
};

/** BLE Connection Link Parameter Definition
 * @attention Every active role in active links shall be defined related link parameters.
 */
ble_att_handle_param_t att_hdl_para_links00[SIZE_ARRAY_ROW(att_service_comb00)]; // Link 0 Server

/** BLE Connection Link Parameter Table Definition
 * @attention Do NOT modify the name of this definition.
 * @note If there does not support server or client please set to "((ble_att_handle_param_t *)0)" which means NULL.
 */
const ble_att_db_mapping_by_id_t att_db_mapping[] =
    {
        // Link 0
        {
            ((ble_att_handle_param_t *)0), // Client Link Parameter
            att_hdl_para_links00,          // Server Link Parameter
        },
};

/** BLE Connection Link Mapping Size Definition
 * @attention Do NOT modify the name of this definition.
 * @note If there does not support server or client please set to 0.
 */
const ble_att_db_mapping_by_id_size_t att_db_mapping_size[] =
    {
        // Link 0
        {
            0,                                  // Client Link Mapping Size
            SIZE_ARRAY_ROW(att_service_comb00), // Server Link Mapping Size
        },
};

/** Maximum Number of Host Connection Link Definition
 * @attention Do NOT modify this definition.
 * @note Defined for host layer.
 */
const uint8_t max_num_conn_host = (SIZE_ARRAY_ROW(att_db_mapping_size));

/** Host Connection Link Information Definition
 * @attention Do NOT modify this definition.
 * @note Defined for host layer.
 */
uint8_t *param_rsv_host[SIZE_ARRAY_ROW(att_db_mapping_size)][(REF_SIZE_LE_HOST_PARA >> 2)];
