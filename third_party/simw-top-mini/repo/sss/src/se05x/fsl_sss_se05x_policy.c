/*
*
* Copyright 2018-2020 NXP
* SPDX-License-Identifier: Apache-2.0
*/

/** @file */

#include <fsl_sss_se05x_apis.h>
#include <nxLog_sss.h>
#include <string.h>

#if SSS_HAVE_APPLET_SE05X_IOT
#include <fsl_sss_se05x_policy.h>

/*Update header bit of policy based on the access rights sets in the policy
 Input:policy object of type sss_policy_sym_key_u
 Output:pbuffer pointing to policy header offset*/
static void sss_se05x_update_header_sym_key_policy(sss_policy_sym_key_u key_pol, uint8_t *pbuffer);

/*Update header bit of policy based on the access rights sets in the policy
 Input:policy object of type sss_policy_asym_key_u
 Output:pbuffer pointing to policy header offset*/
static void sss_se05x_update_header_asym_key_policy(sss_policy_asym_key_u key_pol, uint8_t *pbuffer);

/*Update header bit of policy based on the access rights sets in the policy
Input:policy object of type sss_policy_common_u
Output:pbuffer pointing to policy header offset*/
static void sss_se05x_update_header_common_policy(sss_policy_common_u common_pol, uint8_t *pbuffer);

/*Update header bit of policy based on the access rights sets in the policy
Input:policy object of type sss_policy_userid_u
Output:pbuffer pointing to policy header offset*/
static void sss_se05x_update_header_pin_policy(sss_policy_userid_u pin_pol, uint8_t *pbuffer);

/*Update header bit of policy based on the access rights sets in the policy
Input:policy object of type sss_policy_file_u
Output:pbuffer pointing to policy header offset*/
static void sss_se05x_update_header_file_policy(sss_policy_file_u file_pol, uint8_t *pbuffer);

/*Update header bit of policy based on the access rights sets in the policy
Input:policy object of type sss_policy_counter_u
Output:pbuffer pointing to policy header offset*/
static void sss_se05x_update_header_counter_policy(sss_policy_counter_u counter_pol, uint8_t *pbuffer);

/*Update header bit of policy based on the access rights sets in the policy
Input:policy object of type sss_policy_pcr_u
Output:pbuffer pointing to policy header offset
*/
static void sss_se05x_update_header_pcr_policy(sss_policy_pcr_u pcr_pol, uint8_t *pbuffer);

/*Update header bit of policy based on the access rights sets in the policy
Input:policy object of type sss_policy_common_pcr_value_u
Output:pbuffer pointing to policy header offset
*/
static void sss_se05x_update_header_pcr_value_policy(sss_policy_common_pcr_value_u pcr_value_pol, uint8_t *pbuffer);

static void sss_se05x_update_ext_pcr_value_policy(
    sss_policy_common_pcr_value_u pcr_value_pol, uint8_t *pbuffer, uint32_t *ext_offset);

/*
finds indices of all same auth Ids in a group of polices and returns the count
of same auth ids with in a group of Ids, it laso copies indices in to an array passed by user
Input: authId to be searched
        policies: array of all policies with diversified auth ids
Output: pindices " array contains index of all input authid
retuns :count of same auth ids with in a group of Ids
*/
static int sss_se05x_find_authId_instances(uint32_t authId, uint8_t *pindices, sss_policy_t *policies);
static void sss_se05x_copy_uint32_to_u8_array(uint32_t u32, uint8_t *pbuffer);
static void sss_se05x_copy_uint16_to_u8_array(uint16_t u16, uint8_t *pbuffer);

static void sss_se05x_update_header_sym_key_policy(sss_policy_sym_key_u key_pol, uint8_t *pbuffer)
{
    uint32_t header = 0;
    if (key_pol.can_Sign) {
        header |= POLICY_OBJ_ALLOW_SIGN;
    }
    if (key_pol.can_Verify) {
        header |= POLICY_OBJ_ALLOW_VERIFY;
    }
    if (key_pol.can_Encrypt) {
        header |= POLICY_OBJ_ALLOW_ENC;
    }
    if (key_pol.can_Decrypt) {
        header |= POLICY_OBJ_ALLOW_DEC;
    }
    if (key_pol.can_Import_Export) {
        header |= POLICY_OBJ_ALLOW_IMPORT_EXPORT;
    }
    if (key_pol.forbid_Derived_Output) {
#if SSS_HAVE_SE05X_VER_GTE_06_00
        header |= POLICY_OBJ_FORBID_DERIVED_OUTPUT;
#else
        LOG_W("forbid_Derived_Output is not applied");
#endif
    }
    if (key_pol.can_TLS_KDF) {
#if SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_TLS_KDF;
#else
        LOG_W("can_TLS_KDF is not applied");
#endif
    }
    if (key_pol.allow_kdf_ext_rnd) {
#if SSS_HAVE_SE05X_VER_GTE_06_00
        header |= POLICY_OBJ_ALLOW_KDF_EXT_RANDOM;
#else
        LOG_W("allow_kdf_ext_rnd is not applied");
#endif
    }
    if (key_pol.can_TLS_PMS_KD) {
#if SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_TLS_PMS;
#else
        LOG_W("can_TLS_PMS_KD is not applied");
#endif
    }
    if (key_pol.can_HKDF) {
#if SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_HKDF;
#else
        LOG_W("can_HKDF is not applied");
#endif
    }
    if (key_pol.can_PBKDF) {
#if SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_PBKDF;
#else
        LOG_W("can_PBKDF is not applied");
#endif
    }
    if (key_pol.can_Wrap) {
        header |= POLICY_OBJ_ALLOW_WRAP;
    }
    if (key_pol.can_Desfire_Auth) {
        header |= POLICY_OBJ_ALLOW_DESFIRE_AUTHENTICATION;
    }
    if (key_pol.can_Desfire_Dump) {
        header |= POLICY_OBJ_ALLOW_DESFIRE_DUMP_SESSION_KEYS;
    }
    if (key_pol.can_Desfire_KD) {
#if SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_DESFIRE_KDF;
#else
        LOG_W("can_Desfire_KD is not applied");
#endif
    }
    if (key_pol.forbid_external_iv) {
#if SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_FORBID_EXTERNAL_IV;
#else
        LOG_W("forbid_external_iv is not applied");
#endif
    }
    if (key_pol.can_usage_hmac_pepper) {
#if SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_USAGE_AS_HMAC_PEPPER;
#else
        LOG_W("can_usage_hmac_pepper is not applied");
#endif
    }

    /* Old policies */
    if (key_pol.can_KD) {
#if !SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_KDF;
#else
        LOG_W("can_KD is not applied");
#endif
    }
    if (key_pol.can_Write) {
#if !SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_WRITE;
#else
        LOG_W("can_Write is not applied");
#endif
    }
    if (key_pol.can_Gen) {
#if !SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_GEN;
#else
        LOG_W("can_Gen is not applied");
#endif
    }
    if (key_pol.can_KA) {
#if (SSS_HAVE_APPLET_SE051_H && SSS_HAVE_SE05X_VER_07_02)
        header |= POLICY_OBJ_ALLOW_KA;
#else
        LOG_W("can_KA is not applied");
#endif
    }
    sss_se05x_copy_uint32_to_u8_array(header, pbuffer);
}

static void sss_se05x_update_header_asym_key_policy(sss_policy_asym_key_u key_pol, uint8_t *pbuffer)
{
    uint32_t header = 0;
    if (key_pol.can_Sign) {
        header |= POLICY_OBJ_ALLOW_SIGN;
    }
    if (key_pol.can_Verify) {
        header |= POLICY_OBJ_ALLOW_VERIFY;
    }
    if (key_pol.can_Encrypt) {
        header |= POLICY_OBJ_ALLOW_ENC;
    }
    if (key_pol.can_Decrypt) {
        header |= POLICY_OBJ_ALLOW_DEC;
    }
    if (key_pol.can_Import_Export) {
        header |= POLICY_OBJ_ALLOW_IMPORT_EXPORT;
    }
    if (key_pol.forbid_Derived_Output) {
#if SSS_HAVE_SE05X_VER_GTE_06_00
        header |= POLICY_OBJ_FORBID_DERIVED_OUTPUT;
#else
        LOG_W("forbid_Derived_Output is not applied");
#endif
    }
    if (key_pol.can_Gen) {
        header |= POLICY_OBJ_ALLOW_GEN;
    }
    if (key_pol.can_KA) {
        header |= POLICY_OBJ_ALLOW_KA;
    }
    if (key_pol.can_Attest) {
        header |= POLICY_OBJ_ALLOW_ATTESTATION;
    }

    /* Old policies */
    if (key_pol.can_Read) {
#if !SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_READ;
#else
        LOG_W("can_Read is not applied");
#endif
    }
    if (key_pol.can_Write) {
#if !SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_WRITE;
#else
        LOG_W("can_Write is not applied");
#endif
    }
    if (key_pol.can_KD) {
#if !SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_KDF;
#else
        LOG_W("can_KD is not applied");
#endif
    }
    if (key_pol.can_Wrap) {
#if !SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_WRAP;
#else
        LOG_W("can_Wrap is not applied");
#endif
    }

    sss_se05x_copy_uint32_to_u8_array(header, pbuffer);
}

static void sss_se05x_update_header_common_policy(sss_policy_common_u common_pol, uint8_t *pbuffer)
{
    uint32_t header = 0;

    if (common_pol.forbid_All) {
        header |= POLICY_OBJ_FORBID_ALL;
    }
    if (common_pol.can_Read) {
#if SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_READ;
#else
        LOG_W("can_Read is not applied");
#endif
    }
    if (common_pol.can_Write) {
#if SSS_HAVE_SE05X_VER_GTE_07_02
        header |= POLICY_OBJ_ALLOW_WRITE;
#else
        LOG_W("can_Write is not applied");
#endif
    }
    if (common_pol.can_Delete) {
        header |= POLICY_OBJ_ALLOW_DELETE;
    }
    if (common_pol.req_Sm) {
        header |= POLICY_OBJ_REQUIRE_SM;
    }
    if (common_pol.req_pcr_val) {
        header |= POLICY_OBJ_REQUIRE_PCR_VALUE;
    }
    sss_se05x_copy_uint32_to_u8_array(header, pbuffer);
}

static void sss_se05x_update_header_pin_policy(sss_policy_userid_u pin_pol, uint8_t *pbuffer)
{
    uint32_t header = 0;
    if (pin_pol.can_Write) {
        header |= POLICY_OBJ_ALLOW_WRITE;
    }
    sss_se05x_copy_uint32_to_u8_array(header, pbuffer);
}

static void sss_se05x_update_header_file_policy(sss_policy_file_u file_pol, uint8_t *pbuffer)
{
    uint32_t header = 0;
    if (file_pol.can_Read) {
        header |= POLICY_OBJ_ALLOW_READ;
    }
    if (file_pol.can_Write) {
        header |= POLICY_OBJ_ALLOW_WRITE;
    }
    sss_se05x_copy_uint32_to_u8_array(header, pbuffer);
}

static void sss_se05x_update_header_counter_policy(sss_policy_counter_u counter_pol, uint8_t *pbuffer)
{
    uint32_t header = 0;
    if (counter_pol.can_Read) {
        header |= POLICY_OBJ_ALLOW_READ;
    }
    if (counter_pol.can_Write) {
        header |= POLICY_OBJ_ALLOW_WRITE;
    }
    sss_se05x_copy_uint32_to_u8_array(header, pbuffer);
}

static void sss_se05x_update_header_pcr_policy(sss_policy_pcr_u pcr_pol, uint8_t *pbuffer)
{
    uint32_t header = 0;
    if (pcr_pol.can_Read) {
        header |= POLICY_OBJ_ALLOW_READ;
    }
    if (pcr_pol.can_Write) {
        header |= POLICY_OBJ_ALLOW_WRITE;
    }
    sss_se05x_copy_uint32_to_u8_array(header, pbuffer);
}

static void sss_se05x_update_header_pcr_value_policy(sss_policy_common_pcr_value_u pcr_pol, uint8_t *pbuffer)
{
    AX_UNUSED_ARG(pcr_pol);
    uint32_t header = 0;
    header |= POLICY_OBJ_REQUIRE_PCR_VALUE;
    sss_se05x_copy_uint32_to_u8_array(header, pbuffer);
}

static void sss_se05x_update_ext_pcr_value_policy(
    sss_policy_common_pcr_value_u pcr_pol, uint8_t *pbuffer, uint32_t *ext_offset)
{
    /*copy 4 bytes PCR Object ID*/
    sss_se05x_copy_uint32_to_u8_array(pcr_pol.pcrObjId, pbuffer + *ext_offset);
    *ext_offset += sizeof(pcr_pol.pcrObjId);
    /*copy 32 bytes PCR value*/
    memcpy(pbuffer + *ext_offset, pcr_pol.pcrExpectedValue, sizeof(pcr_pol.pcrExpectedValue));
    *ext_offset += sizeof(pcr_pol.pcrExpectedValue);
}

#if SSS_HAVE_SE05X_VER_GTE_07_02

static void sss_se05x_update_header_desfire_chg_authId_value_policy(
    sss_policy_desfire_changekey_authId_value_u pcr_pol, uint8_t *pbuffer)
{
    AX_UNUSED_ARG(pcr_pol);
#if SSS_HAVE_SE05X_VER_GTE_07_02
    uint32_t header = 0;
    header |= POLICY_OBJ_ALLOW_DESFIRE_CHANGEKEY;
    sss_se05x_copy_uint32_to_u8_array(header, pbuffer);
#else
    LOG_W("desfire_changekey_authId pol is not applied");
#endif
}

static void sss_se05x_update_ext_desfire_chg_authId_value_policy(
    sss_policy_desfire_changekey_authId_value_u desfireAuthId_pol, uint8_t *pbuffer, uint32_t *ext_offset)
{
#if SSS_HAVE_SE05X_VER_GTE_07_02
    /*copy 4 bytes Auth Object ID*/
    sss_se05x_copy_uint32_to_u8_array(desfireAuthId_pol.desfire_authId, pbuffer + *ext_offset);
    *ext_offset += sizeof(desfireAuthId_pol.desfire_authId);
#else
    LOG_W("desfire_changekey_authId ext pol is not applied");
#endif
}

static void sss_se05x_update_header_key_drv_masterId_value_policy(
    sss_policy_key_drv_master_keyid_value_u masterkey_pol, uint8_t *pbuffer)
{
    AX_UNUSED_ARG(masterkey_pol);
#if SSS_HAVE_SE05X_VER_GTE_07_02
    uint32_t header = 0;
    header |= POLICY_OBJ_ALLOW_DERIVED_INPUT;
    sss_se05x_copy_uint32_to_u8_array(header, pbuffer);
#else
    LOG_W("key_drv_masterId pol is not applied");
#endif
}

static void sss_se05x_update_ext_key_drv_masterId_value_policy(
    sss_policy_key_drv_master_keyid_value_u masterkey_pol, uint8_t *pbuffer, uint32_t *ext_offset)
{
#if SSS_HAVE_SE05X_VER_GTE_07_02
    /*copy 4 bytes Master Object ID*/
    sss_se05x_copy_uint32_to_u8_array(masterkey_pol.master_keyId, pbuffer + *ext_offset);
    *ext_offset += sizeof(masterkey_pol.master_keyId);
#else
    LOG_W("key_drv_masterId ext is not applied");
#endif
}

#endif

#if SSS_HAVE_APPLET_SE051_H
static void sss_se05x_update_header_internal_sign_value_policy(
    sss_policy_internal_sign_tbs_value_u tbs_item, uint8_t *pbuffer)
{
    AX_UNUSED_ARG(tbs_item);
    uint32_t header = 0;
    header |= POLICY_OBJ_INTERNAL_SIGN;
    sss_se05x_copy_uint32_to_u8_array(header, pbuffer);
}

static void sss_se05x_update_ext_internal_sign_tbsId_value_policy(
    sss_policy_internal_sign_tbs_value_u tbs_item, uint8_t *pbuffer, uint32_t *ext_offset)
{
    /* copy 4 bytes tbsItemList_KeyId */
    sss_se05x_copy_uint32_to_u8_array(tbs_item.tbsItemList_KeyId, pbuffer + *ext_offset);
    *ext_offset += sizeof(tbs_item.tbsItemList_KeyId);
}
#endif

static void sss_se05x_copy_uint32_to_u8_array(uint32_t u32, uint8_t *pbuffer)
{
    pbuffer[0] |= (uint8_t)((u32 >> 3 * 8) & 0xFF);
    pbuffer[1] |= (uint8_t)((u32 >> 2 * 8) & 0xFF);
    pbuffer[2] |= (uint8_t)((u32 >> 1 * 8) & 0xFF);
    pbuffer[3] |= (uint8_t)((u32 >> 0 * 8) & 0xFF);
}

static void sss_se05x_copy_uint16_to_u8_array(uint16_t u16, uint8_t *pbuffer)
{
    pbuffer[0] |= (uint8_t)((u16 >> 8) & 0xFF);
    pbuffer[1] |= (uint8_t)((u16)&0xFF);
}
static int sss_se05x_find_authId_instances(uint32_t authId, uint8_t *pindices, sss_policy_t *policies)
{
    int count = 0;
    for (uint32_t i = 0; (i <= policies->nPolicies - 1) && (i <= MAX_OBJ_POLICY_TYPES); i++) {
        if (policies->policies[i] != NULL && policies->policies[i]->auth_obj_id == authId) {
            *pindices++ = i;
            count++;
        }
    }
    return count;
}

sss_status_t sss_se05x_create_object_policy_buffer(sss_policy_t *policies, uint8_t *pbuff, size_t *buf_len)
{
    uint8_t temp_buffer[MAX_OBJ_POLICY_SIZE] = {0};
    uint8_t indexArray[MAX_OBJ_POLICY_TYPES] = {0};
    uint8_t auth_id_count                    = 0;
    uint8_t policiesCopied                   = 0;
    uint32_t ext_offset                      = 0;
    uint32_t offset                          = 0;

    if ((policies == NULL) || (pbuff == NULL) || (buf_len == NULL)) {
        return kStatus_SSS_InvalidArgument;
    }

    if (policies->nPolicies > SSS_POLICY_COUNT_MAX) {
        return kStatus_SSS_InvalidArgument;
    }

    *buf_len = 0;
    /*Reinitialize policy buffer for every Secure object*/
    memset(pbuff, 0x00, MAX_POLICY_BUFFER_SIZE);
    for (uint32_t i = 0; i < policies->nPolicies && policiesCopied < policies->nPolicies; i++) {
        if (policies->policies[i] != NULL) {
            if (offset >= MAX_POLICY_BUFFER_SIZE) {
                return kStatus_SSS_InvalidArgument;
            }
            auth_id_count =
                sss_se05x_find_authId_instances(policies->policies[i]->auth_obj_id, &indexArray[0], policies);
            /*length is initialized with default length
                        will be updated when extensions are copied*/
            temp_buffer[OBJ_POLICY_LENGTH_OFFSET] = DEFAULT_OBJECT_POLICY_SIZE;
            /* Copy Auth Id*/
            sss_se05x_copy_uint32_to_u8_array(
                policies->policies[i]->auth_obj_id, &temp_buffer[OBJ_POLICY_AUTHID_OFFSET]);
            for (int j = 0; j < auth_id_count; j++) {
                /* Update AR Header as per object type*/
                switch (policies->policies[indexArray[j]]->type) {
                case KPolicy_Sym_Key:
                    sss_se05x_update_header_sym_key_policy(
                        (policies->policies[indexArray[j]])->policy.symmkey, &temp_buffer[OBJ_POLICY_HEADER_OFFSET]);
                    break;
                case KPolicy_Asym_Key:
                    sss_se05x_update_header_asym_key_policy(
                        (policies->policies[indexArray[j]])->policy.asymmkey, &temp_buffer[OBJ_POLICY_HEADER_OFFSET]);
                    break;
                case KPolicy_Common:
                    sss_se05x_update_header_common_policy(
                        (policies->policies[indexArray[j]])->policy.common, &temp_buffer[OBJ_POLICY_HEADER_OFFSET]);
                    break;
                case KPolicy_Common_PCR_Value:
                    sss_se05x_update_header_pcr_value_policy(
                        (policies->policies[indexArray[j]])->policy.common_pcr_value,
                        &temp_buffer[OBJ_POLICY_HEADER_OFFSET]);
                    sss_se05x_update_ext_pcr_value_policy((policies->policies[indexArray[j]])->policy.common_pcr_value,
                        &temp_buffer[OBJ_POLICY_EXT_OFFSET],
                        &ext_offset);
                    temp_buffer[OBJ_POLICY_LENGTH_OFFSET] += OBJ_POLICY_PCR_DATA_SIZE;
                    break;
                case KPolicy_File:
                    sss_se05x_update_header_file_policy(
                        (policies->policies[indexArray[j]])->policy.file, &temp_buffer[OBJ_POLICY_HEADER_OFFSET]);
                    break;
                case KPolicy_Counter:
                    sss_se05x_update_header_counter_policy(
                        (policies->policies[indexArray[j]])->policy.counter, &temp_buffer[OBJ_POLICY_HEADER_OFFSET]);
                    break;
                case KPolicy_PCR:
                    sss_se05x_update_header_pcr_policy(
                        (policies->policies[indexArray[j]])->policy.pcr, &temp_buffer[OBJ_POLICY_HEADER_OFFSET]);
                    break;
                case KPolicy_UserID:
                    sss_se05x_update_header_pin_policy(
                        (policies->policies[indexArray[j]])->policy.pin, &temp_buffer[OBJ_POLICY_HEADER_OFFSET]);
                    break;
                case KPolicy_Desfire_Changekey_Auth_Id:
#if SSS_HAVE_SE05X_VER_GTE_07_02
                    sss_se05x_update_header_desfire_chg_authId_value_policy(
                        (policies->policies[indexArray[j]])->policy.desfire_auth_id,
                        &temp_buffer[OBJ_POLICY_HEADER_OFFSET]);
                    sss_se05x_update_ext_desfire_chg_authId_value_policy(
                        (policies->policies[indexArray[j]])->policy.desfire_auth_id,
                        &temp_buffer[OBJ_POLICY_EXT_OFFSET],
                        &ext_offset);
                    temp_buffer[OBJ_POLICY_LENGTH_OFFSET] += OBJ_POLICY_OBJ_ID_SIZE;
#else
                    LOG_W("KPolicy_Desfire_Changekey_Auth_Id is not applied");
#endif
                    break;
                case KPolicy_Derive_Master_Key_Id:
#if SSS_HAVE_SE05X_VER_GTE_07_02
                    sss_se05x_update_header_key_drv_masterId_value_policy(
                        (policies->policies[indexArray[j]])->policy.master_key_id,
                        &temp_buffer[OBJ_POLICY_HEADER_OFFSET]);
                    sss_se05x_update_ext_key_drv_masterId_value_policy(
                        (policies->policies[indexArray[j]])->policy.master_key_id,
                        &temp_buffer[OBJ_POLICY_EXT_OFFSET],
                        &ext_offset);
                    temp_buffer[OBJ_POLICY_LENGTH_OFFSET] += OBJ_POLICY_OBJ_ID_SIZE;
#else
                    LOG_W("KPolicy_Derive_Master_Key_Id is not applied");
#endif
                    break;
                case KPolicy_Internal_Sign:
#if SSS_HAVE_APPLET_SE051_H
                    sss_se05x_update_header_internal_sign_value_policy(
                        (policies->policies[indexArray[j]])->policy.tbsItemList,
                        &temp_buffer[OBJ_POLICY_HEADER_OFFSET]);
                    sss_se05x_update_ext_internal_sign_tbsId_value_policy(
                        (policies->policies[indexArray[j]])->policy.tbsItemList,
                        &temp_buffer[OBJ_POLICY_EXT_OFFSET],
                        &ext_offset);
                    temp_buffer[OBJ_POLICY_LENGTH_OFFSET] += OBJ_POLICY_OBJ_ID_SIZE;
#else
                    LOG_W("KPolicy_Internal_Sign is not applied");
#endif
                default:
                    break;
                }
                policies->policies[indexArray[j]] = NULL;
            }
            memcpy(pbuff + offset, temp_buffer, (temp_buffer[0] + 1));
            *buf_len += (temp_buffer[0] + 1);
            policiesCopied = policiesCopied + auth_id_count;
            offset += (temp_buffer[0] + 1);
            /* reinitialize temp buffer for a new policy*/
            memset(&temp_buffer[0], 0x00, sizeof(temp_buffer));
            ext_offset = 0;
        }
    }

    return kStatus_SSS_Success;
}

sss_status_t sss_se05x_create_session_policy_buffer(
    sss_policy_session_u *session_policy, uint8_t *session_pol_buff, size_t *buf_len)
{
    uint16_t session_header = 0;
    /*Reinitialize policy buffer for every Secure object*/
    memset(session_pol_buff, 0x00, MAX_POLICY_BUFFER_SIZE);

    if ((session_policy == NULL) || (session_pol_buff == NULL) || (buf_len == NULL)) {
        return kStatus_SSS_InvalidArgument;
    }

    *buf_len = DEFAULT_SESSION_POLICY_SIZE;

    /*set default length*/
    session_pol_buff[SESSION_POLICY_LENGTH_OFFSET] = DEFAULT_SESSION_POLICY_SIZE;
    if (session_policy->has_MaxOperationsInSession) {
        session_header |= POLICY_SESSION_MAX_APDU;
        sss_se05x_copy_uint16_to_u8_array(session_header, &session_pol_buff[SESSION_POLICY_AR_HEADER_OFFSET]);
        sss_se05x_copy_uint16_to_u8_array(session_policy->maxOperationsInSession, &session_pol_buff[*buf_len]);
        *buf_len += sizeof(session_policy->maxOperationsInSession);
    }
    if (session_policy->has_MaxDurationOfSession_sec) {
        session_header |= POLICY_SESSION_MAX_TIME;
        sss_se05x_copy_uint16_to_u8_array(session_header, &session_pol_buff[SESSION_POLICY_AR_HEADER_OFFSET]);
        sss_se05x_copy_uint16_to_u8_array(session_policy->maxDurationOfSession_sec, &session_pol_buff[*buf_len]);
        *buf_len += sizeof(session_policy->maxDurationOfSession_sec);
    }
    if (session_policy->allowRefresh) {
        session_header |= POLICY_SESSION_ALLOW_REFRESH;
        sss_se05x_copy_uint16_to_u8_array(session_header, &session_pol_buff[SESSION_POLICY_AR_HEADER_OFFSET]);
    }
    session_pol_buff[0] = (uint8_t)(*buf_len - 1); //Exclude Length of Policy field.
    return kStatus_SSS_Success;
}
#endif /* SSS_HAVE_APPLET_SE05X_IOT */
