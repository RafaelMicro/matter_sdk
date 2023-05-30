/** @file mp_sector.c
 *
 * @brief MP sector driver file.
 *
 */

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "mp_sector.h"
#include "cm3_mcu.h"
#include "project_config.h"
#include <stdio.h>
#include <string.h>

/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/

#define OTP_MINUS_CHECK(x) (x < 0 ? -1 : 1)
/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/

/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/

/**************************************************************************************************
 *    GLOBAL VARIABLES
 *************************************************************************************************/
static mp_sector_cal_t otp_cal;
const mp_sector_cal_t otp_cal_t = {
    { { MP_ID_DCDC, MP_VALID, MP_CNT_DCDC }, 2, 0, 0, 0, 0, 1, 1200, 10, 1100, 10, 1300, 10 },                 /* DCDC */
    { { MP_ID_LLDO, MP_VALID, MP_CNT_LLDO }, 2, 0, 0, 0, 0, 1, 1200, 10, 1100, 10, 1300, 10 },                 /* LLDO */
    { { MP_ID_IOLDO, MP_VALID, MP_CNT_IOLDO }, 2, 0, 0, 0, 0, 1, 1800, 7, 1700, 7, 1900, 7 },                  /* IOLDO */
    { { MP_ID_SLDO, MP_VALID, MP_CNT_SLDO }, 2, 0, 0, 0, 0, 1, 820, 0, 800, 0, 900, 0 },                       /* SLDO */
    { { MP_ID_SIOLDO, MP_VALID, MP_CNT_SIOLDO }, 2, 0, 0, 0, 0, 1, 1500, 0, 1400, 0, 1600, 0 },                /* SIOLDO */
    { { MP_ID_VBATADC, MP_VALID, MP_CNT_VBATADC }, 2, 0, 0, 0, 0, 2000, 1766, 2500, 1931, 3300, 2194 },        /* VBAT_ADC */
    { { MP_ID_AIOADC, MP_VALID, MP_CNT_AIOADC }, 2, 0, 0, 0, 0, 2000, 2464, 2500, 2810, 3300, 3363 },          /* AIO_ADC */
    { { MP_ID_VCMADC, MP_VALID, MP_CNT_VCMADC }, 2, 0, 0 },                                                    /* VCM_ADC */
    { { MP_ID_TEMPADC, MP_VALID, MP_CNT_TEMPADC }, 2, 0 },                                                     /* TEMP_ADC */
    { { MP_ID_POWERFAIL, MP_VALID, MP_CNT_POWERFAIL }, 2, 0, 0, 0, 0, 1, 2000, 6, 2100, 8, 2200, 10 },         /* POWER_FAIL */
    { { MP_ID_CRYSTALTRIM, MP_VALID, MP_CNT_CRYSTALTRIM }, 2, 384 },                                           /* CRYSTAL_TRIM */
    { { MP_ID_RF_BAND_SUPPORT, MP_VALID, MP_CNT_RF_BAND_SUPPORT }, 2, 1 },                                     /* RF_BAND_SUPPORT */
    { { MP_ID_RFTRIM_2P4G, MP_VALID, MP_CNT_RFTRIM }, 2, 0, 65535, 65535, 65535, 65535, 65535, 65535, 65535 }, /* RF_TRIM_2P4G */
    { { MP_ID_RFTRIM_SUBG0, MP_VALID, MP_CNT_RFTRIM }, 2, 0, 65535, 65535, 65535, 65535, 65535, 65535, 65535 }, /* RF_TRIM_SUBG0 */
    { { MP_ID_RFTRIM_SUBG1, MP_VALID, MP_CNT_RFTRIM }, 2, 0, 65535, 65535, 65535, 65535, 65535, 65535, 65535 }, /* RF_TRIM SUGG1 */
    { { MP_ID_RFTRIM_SUBG2, MP_VALID, MP_CNT_RFTRIM }, 2, 0, 65535, 65535, 65535, 65535, 65535, 65535, 65535 }, /* RF_TRIM SUBG2 */
    { { MP_ID_TX_POWER_TRIM, MP_VALID, MP_CNT_TX_POWER_TRIM }, 0, 0, 0, 0, 0, 0 },                              /* TX_POWER_TRIM */
    { { MP_ID_RSSI_TRIM, MP_VALID, MP_CNT_RSSI_TRIM }, 0, 0, 0, 0, 0, 0 },                                      /* RSSI_TRIM */
    { { MP_ID_TEMPK, MP_VALID, MP_CNT_TEMPK }, 2, 0 },                                                          /* TEMPERATURE K*/
    { { MP_ID_TX_POWER_OQPSK_TRIM, MP_VALID, MP_CNT_TX_POWER_OQPSK_TRIM }, 0, 0, 0, 0, 0 }, /* TX_POWER_OQPSK_TRIM */
};

uint8_t mp_cal_vbatadc_flag  = 0;
uint16_t mp_cal_vbatadc_v1   = 0;
uint16_t mp_cal_vbatadc_adc1 = 0;
uint16_t mp_cal_vbatadc_v2   = 0;
uint16_t mp_cal_vbatadc_adc2 = 0;

uint8_t mp_cal_aioadc_flag  = 0;
uint16_t mp_cal_aioadc_v1   = 0;
uint16_t mp_cal_aioadc_adc1 = 0;
uint16_t mp_cal_aioadc_v2   = 0;
uint16_t mp_cal_aioadc_adc2 = 0;

uint8_t mp_cal_vcmadc_flag   = 0;
uint8_t mp_cal_vcmadc_enable = 0;
uint16_t mp_cal_vcmadc_adc1  = 0;

uint8_t mp_cal_tempadc_flag  = 0;
uint16_t mp_cal_tempadc_adc1 = 0;

uint8_t mp_cal_tempk_flag   = 0;
uint16_t mp_cal_tempk_value = 0;

mp_sector_valid_t mp_sector_valid = MP_SECTOR_INVALID;

/**************************************************************************************************
 *    LOCAL FUNCTIONS
 *************************************************************************************************/

/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/

/**
 * @ingroup mp_sector_group
 * @brief
 * @return None
 */
void OtpLoadPmuValue(mp_cal_regulator_t * mp_pmu_cal, otp_cal_regulator_t * otp_pmu_cal)
{
    mp_pmu_cal->voltage_1 = otp_pmu_cal->voltage_1;
    mp_pmu_cal->voltage_2 = otp_pmu_cal->voltage_2;
    mp_pmu_cal->vosel_1   = otp_pmu_cal->vosel_1;
    mp_pmu_cal->vosel_2   = otp_pmu_cal->vosel_2;
}
/**
 * @ingroup mp_sector_group
 * @brief
 * @return None
 */
void OtpToMpVoselCal(mp_cal_regulator_t * mp_pmu_cal)
{
    // Target_vosel=vosel_2+(target_voltage-voltage_2)*(vosel_1-vosel_2)/(voltage_1-voltage_2)  ; reference OTP_TO_Mp_sector
    // programe guid ppt
    int32_t cal_temp[3];
    int32_t cal_voltage_temp;
    int8_t cal_vosel_temp;
    int8_t minus_sign_temp;
    int8_t cal_vosel_target[3];
    int8_t i = 0;

    cal_vosel_temp   = (int8_t) (mp_pmu_cal->vosel_1 - mp_pmu_cal->vosel_2);
    cal_voltage_temp = (int32_t) (mp_pmu_cal->voltage_1 - mp_pmu_cal->voltage_2);

    cal_temp[0] = (int32_t) (mp_pmu_cal->target_voltage_1 - mp_pmu_cal->voltage_2);
    cal_temp[1] = (int32_t) (mp_pmu_cal->target_voltage_2 - mp_pmu_cal->voltage_2);
    cal_temp[2] = (int32_t) (mp_pmu_cal->target_voltage_3 - mp_pmu_cal->voltage_2);

    for (i = 0; i < 3; i++)
    {
        minus_sign_temp     = OTP_MINUS_CHECK(cal_temp[i]);
        cal_temp[i]         = (cal_temp[i] * cal_vosel_temp * 100 * (minus_sign_temp)) / cal_voltage_temp + 50;
        cal_temp[i]         = (cal_temp[i] / 100) * (minus_sign_temp);
        cal_temp[i]         = (int32_t) mp_pmu_cal->vosel_2 + cal_temp[i];
        cal_vosel_target[i] = OtpToMpPmuMaxCheck(cal_temp[i], mp_pmu_cal->head.mp_id);
    }

    mp_pmu_cal->target_vosel_1 = cal_vosel_target[0];
    mp_pmu_cal->target_vosel_2 = cal_vosel_target[1];
    mp_pmu_cal->target_vosel_3 = cal_vosel_target[2];
}
/**
 * @ingroup mp_sector_group
 * @brief  Otp to mp pmu value check
 * @return None
 */
uint8_t OtpToMpPmuMaxCheck(int32_t targetvosel, uint32_t id)
{
    uint8_t voselMax = 15;
    uint8_t voselresult;

    if (id == MP_ID_SIOLDO) // SIOLDO voselMax Range Max 7 other 15
    {
        voselMax = 7;
    }

    if (id == MP_ID_SLDO)
    {
        if (targetvosel < 0)
        {
            targetvosel += 16;
        }

        voselresult = targetvosel;
    }
    else
    {
        if (targetvosel < 0)
        {
            voselresult = 0;
        }
        else
        {
            if (targetvosel > voselMax)
            {
                voselresult = voselMax;
            }
            else
            {
                voselresult = (uint8_t) targetvosel;
            }
        }
    }

    return voselresult;
}
/**
 * @ingroup mp_sector_group
 * @brief
 * @return None
 */
void OtpLoadAdcValue(mp_cal_adc_t * mp_adc_cal, otp_cal_adc_t * otp_adc_cal)
{
    mp_adc_cal->voltage_1 = otp_adc_cal->voltage_1;
    mp_adc_cal->voltage_2 = otp_adc_cal->voltage_3;

    mp_adc_cal->adc_1 = otp_adc_cal->adc_1;
    mp_adc_cal->adc_2 = otp_adc_cal->adc_3;
}
/**
 * @ingroup mp_sector_group
 * @brief
 * @return None
 */
void OtpToMpAdcCal(mp_cal_adc_t * cal_adc)
{
    // Target_ADC=ADC_1+(target_voltage-voltage_1)*(ADC_3-ADC1)/(voltage_3-voltage_1) ; reference OTP to Mp_sector programe guid ppt
    cal_adc->target_adc_1 = cal_adc->adc_1 +
        (cal_adc->target_voltage_1 - cal_adc->voltage_1) * (cal_adc->adc_2 - cal_adc->adc_1) /
            (cal_adc->voltage_2 - cal_adc->voltage_1);

    cal_adc->target_adc_2 = cal_adc->adc_1 +
        (cal_adc->target_voltage_2 - cal_adc->voltage_1) * (cal_adc->adc_2 - cal_adc->adc_1) /
            (cal_adc->voltage_2 - cal_adc->voltage_1);

    cal_adc->target_adc_3 = cal_adc->adc_1 +
        (cal_adc->target_voltage_3 - cal_adc->voltage_1) * (cal_adc->adc_2 - cal_adc->adc_1) /
            (cal_adc->voltage_2 - cal_adc->voltage_1);
}
/**
 * @ingroup mp_sector_group
 * @brief
 * @return None
 */
uint32_t OtpPmuRangeCheck(mp_cal_regulator_t * mp_pmu_cal, uint32_t mp_id)
{
    uint8_t voselMax    = 15;
    uint16_t voltageMax = 2000;

    if (mp_id == MP_ID_SIOLDO) // SIOLDO voselMax Range Max 7 other 15
    {
        voselMax = 7;
    }

    if (mp_id == MP_ID_POWERFAIL) // Power Fail Range Max 3600 other 2000
    {
        voltageMax = 3600;
    }

    if (mp_pmu_cal->vosel_1 > voselMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_pmu_cal->vosel_2 > voselMax)
    {
        return STATUS_INVALID_PARAM;
    }

    if (mp_pmu_cal->target_vosel_1 > voselMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_pmu_cal->target_vosel_2 > voselMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_pmu_cal->target_vosel_3 > voselMax)
    {
        return STATUS_INVALID_PARAM;
    }

    if (mp_pmu_cal->voltage_1 > voltageMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_pmu_cal->voltage_2 > voltageMax)
    {
        return STATUS_INVALID_PARAM;
    }

    if (mp_pmu_cal->target_voltage_1 > voltageMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_pmu_cal->target_voltage_2 > voltageMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_pmu_cal->target_voltage_3 > voltageMax)
    {
        return STATUS_INVALID_PARAM;
    }

    mp_pmu_cal->flag   = 1; // update flag to 1
    mp_pmu_cal->select = 1;

    return STATUS_SUCCESS;
}
/**
 * @ingroup mp_sector_group
 * @brief
 * @return None
 */
uint32_t OtpAdcRangeCheck(mp_cal_adc_t * mp_adc_cal_reg, uint32_t mp_id)
{

    uint16_t voltageMax = 3600;
    uint16_t adcMax     = 4095;

    if (mp_adc_cal_reg->adc_1 > adcMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_adc_cal_reg->adc_2 > adcMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_adc_cal_reg->target_adc_1 > adcMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_adc_cal_reg->target_adc_2 > adcMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_adc_cal_reg->target_adc_3 > adcMax)
    {
        return STATUS_INVALID_PARAM;
    }

    if (mp_adc_cal_reg->voltage_1 > voltageMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_adc_cal_reg->voltage_2 > voltageMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_adc_cal_reg->target_voltage_1 > voltageMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_adc_cal_reg->target_voltage_2 > voltageMax)
    {
        return STATUS_INVALID_PARAM;
    }
    if (mp_adc_cal_reg->target_voltage_3 > voltageMax)
    {
        return STATUS_INVALID_PARAM;
    }

    mp_adc_cal_reg->flag = 1; // update flag to 1

    return STATUS_SUCCESS;
}
/**
 * @ingroup mp_sector_group
 * @brief
 * @return None
 */
uint32_t OtpVcmTempAdcRangeCheck(mp_cal_vcm_adc_t * mp_cal_reg, mp_cal_temp_adc_t * tmp_mp_cal_reg, uint32_t mp_id)
{
    uint8_t Tccompenstion = 1;
    uint16_t adcMax       = 4095;

    if (mp_id == MP_ID_VCMADC)
    {
        if (mp_cal_reg->enable > Tccompenstion)
        {
            return STATUS_INVALID_PARAM;
        }
        if (mp_cal_reg->adc_1 > adcMax)
        {
            return STATUS_INVALID_PARAM;
        }

        mp_cal_reg->flag = 1; // update flag to 1
    }
    else
    {
        if (tmp_mp_cal_reg->adc_1 > adcMax)
        {
            return STATUS_INVALID_PARAM;
        }
        tmp_mp_cal_reg->flag = 1; // update flag to 1
    }

    return STATUS_SUCCESS;
}
/**
 * @ingroup mp_sector_group
 * @brief
 * @return None
 */
uint32_t GetMpSectorInfo(mp_sector_inf_t * MpSectorInf)
{
    uint32_t flash_size_id = (flash_get_deviceinfo() >> FLASH_SIZE_ID_SHIFT);

    if (flash_size_id == FLASH_512K)
    {
        MpSectorInf->ver                  = MP_SECTOR_INFO_512K->MP_SECTOR_VERSION;
        MpSectorInf->size                 = MP_SECTOR_INFO_512K->MP_SECTOR_SIZE;
        MpSectorInf->cal                  = MP_SECTOR_INFO_512K->MP_SECTOR_CALIBRATION;
        MpSectorInf->cal_data_sector_size = MP_SECTOR_INFO_512K->CAL_DATA_SECTOR_SIZE;
        MpSectorInf->cal_data_sector_addr = MP_SECTOR_INFO_512K->CAL_DATA_SECTOR_ADDR;
    }
    else if (flash_size_id == FLASH_1024K)
    {
        MpSectorInf->ver                  = MP_SECTOR_INFO_1024K->MP_SECTOR_VERSION;
        MpSectorInf->size                 = MP_SECTOR_INFO_1024K->MP_SECTOR_SIZE;
        MpSectorInf->cal                  = MP_SECTOR_INFO_1024K->MP_SECTOR_CALIBRATION;
        MpSectorInf->cal_data_sector_size = MP_SECTOR_INFO_1024K->CAL_DATA_SECTOR_SIZE;
        MpSectorInf->cal_data_sector_addr = MP_SECTOR_INFO_1024K->CAL_DATA_SECTOR_ADDR;
    }
    else if (flash_size_id == FLASH_2048K)
    {
        MpSectorInf->ver                  = MP_SECTOR_INFO_2048K->MP_SECTOR_VERSION;
        MpSectorInf->size                 = MP_SECTOR_INFO_2048K->MP_SECTOR_SIZE;
        MpSectorInf->cal                  = MP_SECTOR_INFO_2048K->MP_SECTOR_CALIBRATION;
        MpSectorInf->cal_data_sector_size = MP_SECTOR_INFO_2048K->CAL_DATA_SECTOR_SIZE;
        MpSectorInf->cal_data_sector_addr = MP_SECTOR_INFO_2048K->CAL_DATA_SECTOR_ADDR;
    }
    else
    {
        return STATUS_INVALID_PARAM;
    }

    return STATUS_SUCCESS;
}
/**
 * @ingroup mp_sector_group
 * @brief Function according to the mp_cal_reg parameters to initial the DCDC registers
 * @param[in] mp_cal_reg
 *            \arg mp_sector_head_t head
 *            \arg flag;
 *            \arg voltage_1
 *            \arg vosel_1
 *            \arg voltage_2
 *            \arg vosel_2
 *            \arg select
 *            \arg target_voltage_1
 *            \arg target_vosel_1
 *            \arg target_voltage_2
 *            \arg target_vosel_2
 *            \arg target_voltage_3
 *            \arg target_vosel_3
 * @return None
 */
void MpCalDcdcInit(mp_cal_regulator_t * mp_cal_reg)
{
    uint8_t flag;
    uint8_t select;
    uint8_t target_vosel = 0;

    flag   = mp_cal_reg->flag;
    select = mp_cal_reg->select;

    switch (select)
    {
    case 1:
        target_vosel = mp_cal_reg->target_vosel_1;
        break;

    case 2:
        target_vosel = mp_cal_reg->target_vosel_2;
        break;

    case 3:
        target_vosel = mp_cal_reg->target_vosel_3;
        break;

    default:
        flag = 0;
        break;
    }

    if ((flag == 1) || (flag == 2))
    {
        PMU->PMU_VOUT_SEL0.bit.DCDC_VOSEL_NM = target_vosel;
        PMU->PMU_VOUT_SEL0.bit.DCDC_VOSEL_SP = target_vosel;
        PMU->PMU_VOUT_SEL0.bit.DCDC_VOSEL_DS = target_vosel;
    }
}
/**
 * @ingroup mp_sector_group
 * @brief Function according to the mp_cal_reg parameters to initial the LDO registers
 * @param[in] mp_cal_reg
 *            \arg mp_sector_head_t head
 *            \arg flag;
 *            \arg voltage_1
 *            \arg vosel_1
 *            \arg voltage_2
 *            \arg vosel_2
 *            \arg select
 *            \arg target_voltage_1
 *            \arg target_vosel_1
 *            \arg target_voltage_2
 *            \arg target_vosel_2
 *            \arg target_voltage_3
 *            \arg target_vosel_3
 * @return None
 */
void MpCalLldoInit(mp_cal_regulator_t * mp_cal_reg)
{
    uint8_t flag;
    uint8_t select;
    uint8_t target_vosel = 0;

    flag   = mp_cal_reg->flag;
    select = mp_cal_reg->select;

    switch (select)
    {
    case 1:
        target_vosel = mp_cal_reg->target_vosel_1;
        break;

    case 2:
        target_vosel = mp_cal_reg->target_vosel_2;
        break;

    case 3:
        target_vosel = mp_cal_reg->target_vosel_3;
        break;

    default:
        flag = 0;
        break;
    }

    if ((flag == 1) || (flag == 2))
    {
        PMU->PMU_VOUT_SEL0.bit.LLDO_VOSEL_NM = target_vosel;
        PMU->PMU_VOUT_SEL0.bit.LLDO_VOSEL_SP = target_vosel;
        PMU->PMU_VOUT_SEL0.bit.LLDO_VOSEL_DS = target_vosel;
    }
}
/**
 * @ingroup mp_sector_group
 * @brief Function according to the mp_cal_reg parameters to initial the IOLDO registers
 * @param[in] mp_cal_reg
 *            \arg mp_sector_head_t head
 *            \arg flag;
 *            \arg voltage_1
 *            \arg vosel_1
 *            \arg voltage_2
 *            \arg vosel_2
 *            \arg select
 *            \arg target_voltage_1
 *            \arg target_vosel_1
 *            \arg target_voltage_2
 *            \arg target_vosel_2
 *            \arg target_voltage_3
 *            \arg target_vosel_3
 * @return None
 */
void MpCalIoldoInit(mp_cal_regulator_t * mp_cal_reg)
{
    uint8_t flag;
    uint8_t select;
    uint8_t target_vosel = 0;

    flag   = mp_cal_reg->flag;
    select = mp_cal_reg->select;

    switch (select)
    {
    case 1:
        target_vosel = mp_cal_reg->target_vosel_1;
        break;

    case 2:
        target_vosel = mp_cal_reg->target_vosel_2;
        break;

    case 3:
        target_vosel = mp_cal_reg->target_vosel_3;
        break;

    default:
        flag = 0;
        break;
    }

    if ((flag == 1) || (flag == 2))
    {
        PMU->PMU_VOUT_SEL2.bit.IOLDO_VOSEL_NM = target_vosel;
        PMU->PMU_VOUT_SEL2.bit.IOLDO_VOSEL_SP = target_vosel;
        PMU->PMU_VOUT_SEL2.bit.IOLDO_VOSEL_DS = target_vosel;
    }
}
/**
 * @ingroup mp_sector_group
 * @brief Function according to the mp_cal_reg parameters to initial the SLDO registers
 * @param[in] mp_cal_reg
 *            \arg mp_sector_head_t head
 *            \arg flag;
 *            \arg voltage_1
 *            \arg vosel_1
 *            \arg voltage_2
 *            \arg vosel_2
 *            \arg select
 *            \arg target_voltage_1
 *            \arg target_vosel_1
 *            \arg target_voltage_2
 *            \arg target_vosel_2
 *            \arg target_voltage_3
 *            \arg target_vosel_3
 * @return None
 */
void MpCalSldoInit(mp_cal_regulator_t * mp_cal_reg)
{
    uint8_t flag;
    uint8_t select;
    uint8_t target_vosel = 0;

    flag   = mp_cal_reg->flag;
    select = mp_cal_reg->select;

    switch (select)
    {
    case 1:
        target_vosel = mp_cal_reg->target_vosel_1;
        break;

    case 2:
        target_vosel = mp_cal_reg->target_vosel_2;
        break;

    case 3:
        target_vosel = mp_cal_reg->target_vosel_3;
        break;

    default:
        flag = 0;
        break;
    }

    if ((flag == 1) || (flag == 2))
    {
        PMU->PMU_VOUT_SEL1.bit.SLDO_VOSEL_NM = target_vosel;
        PMU->PMU_VOUT_SEL1.bit.SLDO_VOSEL_SP = target_vosel;
        PMU->PMU_VOUT_SEL1.bit.SLDO_VOSEL_DS = target_vosel;
    }
}
/**
 * @ingroup mp_sector_group
 * @brief Function according to the mp_cal_reg parameters to initial the SIOLDO registers
 * @param[in] mp_cal_reg
 *            \arg mp_sector_head_t head
 *            \arg flag;
 *            \arg voltage_1
 *            \arg vosel_1
 *            \arg voltage_2
 *            \arg vosel_2
 *            \arg select
 *            \arg target_voltage_1
 *            \arg target_vosel_1
 *            \arg target_voltage_2
 *            \arg target_vosel_2
 *            \arg target_voltage_3
 *            \arg target_vosel_3
 * @return None
 */
void MpCalSioldoInit(mp_cal_regulator_t * mp_cal_reg)
{
    uint8_t flag;
    uint8_t select;
    uint8_t target_vosel = 0;

    flag   = mp_cal_reg->flag;
    select = mp_cal_reg->select;

    switch (select)
    {
    case 1:
        target_vosel = mp_cal_reg->target_vosel_1;
        break;

    case 2:
        target_vosel = mp_cal_reg->target_vosel_2;
        break;

    case 3:
        target_vosel = mp_cal_reg->target_vosel_3;
        break;

    default:
        flag = 0;
        break;
    }

    if ((flag == 1) || (flag == 2))
    {
        PMU->PMU_VOUT_SEL2.bit.IOLDO_RET_VOSEL_NM = target_vosel;
        PMU->PMU_VOUT_SEL2.bit.IOLDO_RET_VOSEL_SP = target_vosel;
        PMU->PMU_VOUT_SEL2.bit.IOLDO_RET_VOSEL_DS = target_vosel;
    }
}

/*
uint32_t MpCalVbatAdc(sadc_value_t adc_val)
{
    int32_t cal_vol = 0;

    if ((mp_cal_vbatadc_flag == 1) || (mp_cal_vbatadc_flag == 2))
    {
        cal_vol = (adc_val - mp_cal_vbatadc_adc1);
        cal_vol *= (mp_cal_vbatadc_v2 - mp_cal_vbatadc_v1);
        cal_vol /= (mp_cal_vbatadc_adc2 - mp_cal_vbatadc_adc1);
        cal_vol += mp_cal_vbatadc_v1;
    }

    return cal_vol;
}
*/
uint32_t MpCalVbatAdcRead(mp_cal_adc_t * mp_cal_adc)
{
    uint32_t read_status = STATUS_INVALID_PARAM;

    if ((mp_cal_vbatadc_flag == 1) || (mp_cal_vbatadc_flag == 2))
    {
        mp_cal_adc->voltage_1 = mp_cal_vbatadc_v1;
        mp_cal_adc->adc_1     = mp_cal_vbatadc_adc1;
        mp_cal_adc->voltage_2 = mp_cal_vbatadc_v2;
        mp_cal_adc->adc_2     = mp_cal_vbatadc_adc2;

        read_status = STATUS_SUCCESS;
    }

    return read_status;
}

uint32_t MpCalAioAdcRead(mp_cal_adc_t * mp_cal_adc)
{
    uint32_t read_status = STATUS_INVALID_PARAM;

    if ((mp_cal_aioadc_flag == 1) || (mp_cal_aioadc_flag == 2))
    {
        mp_cal_adc->voltage_1 = mp_cal_aioadc_v1;
        mp_cal_adc->adc_1     = mp_cal_aioadc_adc1;
        mp_cal_adc->voltage_2 = mp_cal_aioadc_v2;
        mp_cal_adc->adc_2     = mp_cal_aioadc_adc2;

        read_status = STATUS_SUCCESS;
    }

    return read_status;
}

uint32_t MpCalVcmAdcRead(mp_cal_vcm_adc_t * mp_cal_vcmadc)
{
    uint32_t read_status = STATUS_INVALID_PARAM;

    if ((mp_cal_vcmadc_flag == 1) || (mp_cal_vcmadc_flag == 2))
    {
        mp_cal_vcmadc->enable = mp_cal_vcmadc_enable;
        mp_cal_vcmadc->adc_1  = mp_cal_vcmadc_adc1;

        read_status = STATUS_SUCCESS;
    }

    return read_status;
}

uint32_t MpCalTempAdcRead(mp_cal_temp_adc_t * mp_cal_tempadc)
{
    uint32_t read_status = STATUS_INVALID_PARAM;

    if ((mp_cal_tempadc_flag == 1) || (mp_cal_tempadc_flag == 2))
    {
        mp_cal_tempadc->adc_1 = mp_cal_tempadc_adc1;

        read_status = STATUS_SUCCESS;
    }

    return read_status;
}

uint32_t MpCalKtRead(mp_temp_k_t * mp_cal_k)
{
    uint32_t read_status = STATUS_INVALID_PARAM;

    if ((mp_cal_tempk_flag == 1) || (mp_cal_tempk_flag == 2))
    {
        mp_cal_k->ktvalue = mp_cal_tempk_value;

        read_status = STATUS_SUCCESS;
    }

    return read_status;
}

void MpCalVbatAdcInit(mp_cal_adc_t * mp_cal_adc)
{
    mp_cal_vbatadc_flag = mp_cal_adc->flag;
    mp_cal_vbatadc_v1   = mp_cal_adc->voltage_1;
    mp_cal_vbatadc_adc1 = mp_cal_adc->adc_1;
    mp_cal_vbatadc_v2   = mp_cal_adc->voltage_2;
    mp_cal_vbatadc_adc2 = mp_cal_adc->adc_2;
}

void MpCalAioAdcInit(mp_cal_adc_t * mp_cal_adc)
{
    mp_cal_aioadc_flag = mp_cal_adc->flag;
    mp_cal_aioadc_v1   = mp_cal_adc->voltage_1;
    mp_cal_aioadc_adc1 = mp_cal_adc->adc_1;
    mp_cal_aioadc_v2   = mp_cal_adc->voltage_2;
    mp_cal_aioadc_adc2 = mp_cal_adc->adc_2;
}

void MpCalVcmAdcInit(mp_cal_vcm_adc_t * mp_cal_vcmadc)
{
    mp_cal_vcmadc_flag   = mp_cal_vcmadc->flag;
    mp_cal_vcmadc_enable = mp_cal_vcmadc->enable;
    mp_cal_vcmadc_adc1   = mp_cal_vcmadc->adc_1;
}

void MpCalTempAdcInit(mp_cal_temp_adc_t * mp_cal_tempadc)
{
    mp_cal_tempadc_flag = mp_cal_tempadc->flag;
    mp_cal_tempadc_adc1 = mp_cal_tempadc->adc_1;
}

void MpCalKtReadInit(mp_temp_k_t * mp_cal_k)
{
    mp_cal_tempk_flag  = mp_cal_k->flag;
    mp_cal_tempk_value = mp_cal_k->ktvalue;
}
/**
 * @ingroup mp_sector_group
 * @brief Function according to the mp_cal_reg parameters to initial the PMU->PMU_COMP0.bit.AUX_COMP_VSEL registers
 * @param[in] mp_cal_reg
 *            \arg mp_sector_head_t head
 *            \arg flag;
 *            \arg voltage_1
 *            \arg vosel_1
 *            \arg voltage_2
 *            \arg vosel_2
 *            \arg select
 *            \arg target_voltage_1
 *            \arg target_vosel_1
 *            \arg target_voltage_2
 *            \arg target_vosel_2
 *            \arg target_voltage_3
 *            \arg target_vosel_3
 * @return None
 */
void MpCalPowerfailInit(mp_cal_regulator_t * mp_cal_reg)
{
    uint8_t flag;
    uint8_t select;
    uint8_t target_vosel = 0;

    flag   = mp_cal_reg->flag;
    select = mp_cal_reg->select;

    switch (select)
    {
    case 1:
        target_vosel = mp_cal_reg->target_vosel_1;
        break;

    case 2:
        target_vosel = mp_cal_reg->target_vosel_2;
        break;

    case 3:
        target_vosel = mp_cal_reg->target_vosel_3;
        break;

    default:
        flag = 0;
        break;
    }

    if ((flag == 1) || (flag == 2))
    {
        PMU->PMU_COMP0.bit.AUX_COMP_VSEL = target_vosel;
    }
}
/**
 * @ingroup mp_sector_group
 * @brief Function according to the mp_cal_reg parameters to initial the PMU->PMU_COMP0.bit.AUX_COMP_VSEL registers
 * @param[in] mp_cal_xtaltrim
 *            \arg mp_sector_head_t head;
 *            \arg flag
 *            \arg xo_trim;
 * @return None
 */
void MpCalCrystaltrimInit(mp_cal_xtal_trim_t * mp_cal_xtaltrim)
{
    uint8_t flag;
    uint16_t target_xo_trim;

    flag           = mp_cal_xtaltrim->flag;
    target_xo_trim = mp_cal_xtaltrim->xo_trim;

    if ((flag == 1) || (flag == 2))
    {
        PMU->PMU_PM_SEL.bit.XOCAP_UPDATE_MODE = 1;
        PMU->PMU_XTAL.bit.CFG_XTAL_CAP_SEL    = target_xo_trim;
    }
}

mp_sector_head_t * GetSpecValidMpId(uint32_t spec_mp_id);
mp_sector_head_t * GetNullMpId(void);

uint32_t MpCalRftrimWrite(uint32_t mp_id, mp_cal_rf_trim_t * mp_cal_rf)
{
    uint32_t i;
    uint32_t write_status = STATUS_SUCCESS;
    uint32_t write_addr;
    uint32_t write_cnt;
    uint8_t write_byte;
    uint8_t * pWriteByte;

    mp_sector_head_t * valid_rf_trim;
    valid_rf_trim = GetSpecValidMpId(mp_id);

    pWriteByte = (uint8_t *) mp_cal_rf;

    // write_cnt = mp_cal_rf->head.mp_cnt;
    write_cnt  = sizeof(mp_cal_rf_trim_t);
    write_addr = (uint32_t) GetNullMpId();

    if (write_addr == 0)
    {
        return STATUS_ERROR;
    }

    for (i = 0; i < write_cnt; i++)
    {
        write_byte = *pWriteByte;
        pWriteByte++;

        flash_write_byte((write_addr + i), write_byte);
        while (flash_check_busy())
            ;
    }

    if (valid_rf_trim != NULL)
    {
        write_addr = (uint32_t) &valid_rf_trim->mp_valid;
        flash_write_byte(write_addr, MP_INVALID);
        while (flash_check_busy())
            ;
    }

    if (CACHE->CCR.bit.CACHE_EN)
    {
        CACHE->CCR.bit.CACHE_EN = 0;
        CACHE->CCR.bit.CACHE_EN = 1;
    }

    return write_status;
}

uint32_t MpCalRftrimRead(uint32_t mp_id, uint32_t byte_cnt, uint8_t * mp_sec_data)
{
    uint32_t i;
    uint32_t read_status = STATUS_SUCCESS;
    uint8_t * pValidByte;
    uint8_t * pReadByte;

    mp_sector_head_t * valid_mp_sec;
    valid_mp_sec = GetSpecValidMpId(mp_id);

    do
    {
        if (valid_mp_sec == NULL)
        {
            read_status = STATUS_INVALID_REQUEST;
            break;
        }

        pValidByte = (uint8_t *) valid_mp_sec;
        pReadByte  = (uint8_t *) mp_sec_data;

        for (i = 0; i < byte_cnt; i++)
        {
            *pReadByte = *pValidByte;
            pValidByte++;
            pReadByte++;
        }

    } while (0);

    return read_status;
}
/*
mp_sector_head_t * GetFirstValidMpId(mp_sector_head_t *pMpHead)
{
//    uint32_t get_status = 0;
    uint32_t mp_next = 0;

    mp_sector_head_t *pSearchHead;

    pSearchHead = pMpHead;

    do
    {
        if (pSearchHead->mp_id == MP_ID_NULL)
        {
            pMpHead = NULL;
//            get_status = 0;
            break;
        }

        if (pSearchHead->mp_valid == MP_VALID)
        {
            pMpHead = pSearchHead;
//            get_status = 1;
            break;
        }

        mp_next = (uint32_t)pSearchHead;
        mp_next += pSearchHead->mp_cnt;
        pSearchHead = (mp_sector_head_t *)mp_next;
    } while (1);

//    return get_status;
    return pMpHead;
}

uint32_t GetNextValidMpId(mp_sector_head_t *pMpHead)
{
}
*/
/**
 * @ingroup mp_sector_group
 * @brief Function to get the Mp sector id
 * @param[in] pMpHead
 *            \arg mp_id    id
 *            \arg mp_valid vaild flag
 *            \arg mp_cnt   counter value
 * @return pNextHead next vaild the point
 */
mp_sector_head_t * GetNextMpId(mp_sector_head_t * pMpHead)
{
    uint32_t mp_next             = 0;
    mp_sector_head_t * pNextHead = NULL;

    if ((pMpHead->mp_id != MP_ID_NULL) && (pMpHead->mp_cnt != 0))
    {
        mp_next = (uint32_t) pMpHead;
        mp_next += pMpHead->mp_cnt;
        pNextHead = (mp_sector_head_t *) mp_next;
    }

    return pNextHead;
}
/**
 * @ingroup mp_sector_group
 * @brief Function to get first vaild the Mp sector id
 * @param[in] pMpHead
 *            \arg mp_id    id
 *            \arg mp_valid vaild flag
 *            \arg mp_cnt   counter value
 * @return pNextHead vaild the point
 */
mp_sector_head_t * GetFirstValidMpId(mp_sector_head_t * pMpHead)
{
    mp_sector_head_t * pValidHead = NULL;

    while ((pMpHead != NULL) && (pMpHead->mp_id != MP_ID_NULL))
    {
        if (pMpHead->mp_valid == MP_VALID)
        {
            pValidHead = pMpHead;
            break;
        }

        pMpHead = GetNextMpId(pMpHead);
    }

    return pValidHead;
}
/**
 * @ingroup mp_sector_group
 * @brief Function to get next vaild the Mp sector id
 * @param[in] pMpHead
 *            \arg mp_id    id
 *            \arg mp_valid vaild flag
 *            \arg mp_cnt   counter value
 * @return pValidHead vaild the point
 */
mp_sector_head_t * GetNextValidMpId(mp_sector_head_t * pMpHead)
{
    mp_sector_head_t * pValidHead = NULL;

    if (pMpHead != NULL)
    {
        pValidHead = GetFirstValidMpId(GetNextMpId(pMpHead));
    }

    return pValidHead;
}

/**
 * @ingroup mp_sector_group
 * @brief Function to get a vaild point
 * @param[in] spec_mp_id
 *
 * @return pMpHead vaild the point
 */
mp_sector_head_t * GetSpecValidMpId(uint32_t spec_mp_id)
{
    mp_sector_head_t * pMpHead;
    mp_sector_inf_t MpInf;
    memset(&MpInf, '\0', sizeof(mp_sector_inf_t));

    GetMpSectorInfo(&MpInf);

    pMpHead = (mp_sector_head_t *) ((mp_sector_cal_t *) MpInf.cal_data_sector_addr);

    while (pMpHead != NULL)
    {
        if (pMpHead->mp_id == spec_mp_id)
        {
            break;
        }

        pMpHead = GetNextValidMpId(pMpHead);
    }

    return pMpHead;
}
/**
 * @ingroup mp_sector_group
 * @brief Function to get a vaild point
 * @return pNullMpHead vaild the point
 */
mp_sector_head_t * GetNullMpId(void)
{
    uint32_t mp_next = 0;
    mp_sector_head_t * pMpHead;
    mp_sector_head_t * pNullMpHead = NULL;
    mp_sector_inf_t MpInf;
    memset(&MpInf, '\0', sizeof(mp_sector_inf_t));

    GetMpSectorInfo(&MpInf);

    pMpHead = (mp_sector_head_t *) ((mp_sector_cal_t *) MpInf.cal_data_sector_addr);

    while (pMpHead != NULL)
    {
        if (pMpHead->mp_id == MP_ID_NULL)
        {
            pNullMpHead = pMpHead;
            break;
        }

        if (pMpHead->mp_cnt == 0)
        {
            break;
        }

        mp_next = (uint32_t) pMpHead;
        mp_next += pMpHead->mp_cnt;
        pMpHead = (mp_sector_head_t *) mp_next;
    }

    return pNullMpHead;
}

/**
 * @ingroup mp_sector_group
 * @brief Function to calcation initinal calibration the value
 * @param[in] cal_sector
 *            \arg mp_cal_regulator_t DCDC;
 *            \arg mp_cal_regulator_t LLDO;
 *            \arg mp_cal_regulator_t IOLDO;
 *            \arg mp_cal_regulator_t SLDO;
 *            \arg mp_cal_regulator_t SIOLDO;
 *            \arg mp_cal_adc_t VBAT_ADC;
 *            \arg mp_cal_adc_t AIO_ADC;
 *            \arg mp_cal_vcm_adc_t VCM_ADC;
 *            \arg mp_cal_temp_adc_t TEMP_ADC;
 *            \arg mp_cal_regulator_t POWER_FAIL;
 *            \arg mp_cal_xtal_trim_t CRYSTAL_TRIM;
 *            \arg mp_cal_rf_band_support_t RF_BAND_SUPPORT;
 *            \arg mp_cal_rf_trim_t RF_TRIM_2P4G;
 *            \arg mp_cal_rf_trim_t RF_TRIM_SUBG0;
 *            \arg mp_cal_rf_trim_t RF_TRIM_SUBG1;
 *            \arg mp_cal_rf_trim_t RF_TRIM_SUBG2;
 * @return None
 */
void MpCalibrationInit(mp_sector_cal_t * cal_sector)
{
    mp_sector_head_t * pMpHead;

    pMpHead = GetFirstValidMpId((mp_sector_head_t *) cal_sector);

    while (pMpHead != NULL)
    {
        switch (pMpHead->mp_id)
        {
        case MP_ID_DCDC:
            MpCalDcdcInit((mp_cal_regulator_t *) &cal_sector->DCDC);
            break;

        case MP_ID_LLDO:
            MpCalLldoInit((mp_cal_regulator_t *) &cal_sector->LLDO);
            break;

        case MP_ID_IOLDO:
            MpCalIoldoInit((mp_cal_regulator_t *) &cal_sector->IOLDO);
            break;

        case MP_ID_SLDO:
            MpCalSldoInit((mp_cal_regulator_t *) &cal_sector->SLDO);
            break;

        case MP_ID_SIOLDO:
            MpCalSioldoInit((mp_cal_regulator_t *) &cal_sector->SIOLDO);
            break;
        /*
                    case MP_ID_VBATADC:
                        MpCalVbatAdcInit((mp_cal_adc_t *)&cal_sector->VBAT_ADC);
                        break;

                    case MP_ID_AIOADC:
                        MpCalAioAdcInit((mp_cal_adc_t *)&cal_sector->AIO_ADC);
                        break;

                    case MP_ID_VCMADC:
                        MpCalVcmAdcInit((mp_cal_vcm_adc_t *)&cal_sector->VCM_ADC);
                        break;

                    case MP_ID_TEMPADC:
                        MpCalTempAdcInit((mp_cal_temp_adc_t *)&cal_sector->TEMP_ADC);
                        break;
        */
        case MP_ID_POWERFAIL:
            MpCalPowerfailInit((mp_cal_regulator_t *) &cal_sector->POWER_FAIL);
            break;

        case MP_ID_CRYSTALTRIM:
            MpCalCrystaltrimInit((mp_cal_xtal_trim_t *) &cal_sector->CRYSTAL_TRIM);
            break;

        default:
            break;
        }

        pMpHead = GetNextValidMpId(pMpHead);
    }
}

void MpSectorInit(void)
{
    uint32_t mp_sector_check = 0;
    mp_sector_valid          = MP_SECTOR_INVALID;

    mp_sector_inf_t MpInf;
    memset(&MpInf, '\0', sizeof(mp_sector_inf_t));

    GetMpSectorInfo(&MpInf);

    do
    {

        if (((MpInf.ver & MP_SECTOR_VERSION_CODE_MASK) == MP_SECTOR_VERSION_CODE) && (MpInf.size == MP_SECTOR_TOTAL_SIZE) &&
            (MpInf.cal_data_sector_size <= MP_SECTOR_CAL_SIZE))
        {
            mp_sector_check = 1;
        }

        if (mp_sector_check == 1)
        {
            if (((MpInf.ver == MP_SECTOR_VERSION_ID_V1) && (MpInf.cal == MP_SECTOR_CALIBRATION_TOOL_V1)) ||
                (MpInf.cal == MP_SECTOR_CALIBRATION_TOOL_V2))
            {
                mp_sector_valid = MP_SECTOR_VALID_MPTOOL;

                break;
            }
        }

        if (OtpToMpCalibration() == STATUS_SUCCESS)
        {
            mp_sector_valid = MP_SECTOR_VALID_SWCAL;

            break;
        }

        if (mp_sector_check == 1)
        {
            if (((MpInf.ver == MP_SECTOR_VERSION_ID_V1) && (MpInf.cal == MP_SECTOR_CALIBRATION_SW_V1)) ||
                (MpInf.cal == MP_SECTOR_CALIBRATION_SW_V2))
            {
                mp_sector_valid = MP_SECTOR_VALID_SWDEFAULT;

                break;
            }
        }

    } while (0);

    switch (mp_sector_valid)
    {
    case MP_SECTOR_VALID_MPTOOL:
    case MP_SECTOR_VALID_SWDEFAULT:

        MpCalibrationInit((mp_sector_cal_t *) MpInf.cal_data_sector_addr);

        break;

    case MP_SECTOR_VALID_SWCAL:
        MpCalibrationInit((mp_sector_cal_t *) &otp_cal);
        break;

    default:
        break;
    }
}

void MpCalibrationAdcInit(mp_sector_cal_t * cal_sector)
{
    mp_sector_head_t * pMpHead;

    pMpHead = GetFirstValidMpId((mp_sector_head_t *) cal_sector);

    while (pMpHead != NULL)
    {
        switch (pMpHead->mp_id)
        {
        case MP_ID_VBATADC:
            MpCalVbatAdcInit((mp_cal_adc_t *) &cal_sector->VBAT_ADC);
            break;

        case MP_ID_AIOADC:
            MpCalAioAdcInit((mp_cal_adc_t *) &cal_sector->AIO_ADC);
            break;

        case MP_ID_VCMADC:
            MpCalVcmAdcInit((mp_cal_vcm_adc_t *) &cal_sector->VCM_ADC);
            break;

        case MP_ID_TEMPADC:
            MpCalTempAdcInit((mp_cal_temp_adc_t *) &cal_sector->TEMP_ADC);
            break;

        case MP_ID_TEMPK:
            MpCalKtReadInit((mp_temp_k_t *) &cal_sector->TEMP_K);
            break;

        default:
            break;
        }

        pMpHead = GetNextValidMpId(pMpHead);
    }
}

void MpCalAdcInit(void)
{
    uint32_t mp_sector_check = 0;

    mp_sector_inf_t MpInf;
    memset(&MpInf, '\0', sizeof(mp_sector_inf_t));

    GetMpSectorInfo(&MpInf);

    do
    {
        if (mp_sector_valid != MP_SECTOR_INVALID)
        {
            break;
        }

        if (((MpInf.ver & MP_SECTOR_VERSION_CODE_MASK) == MP_SECTOR_VERSION_CODE) && (MpInf.size == MP_SECTOR_TOTAL_SIZE) &&
            (MpInf.cal_data_sector_size <= MP_SECTOR_CAL_SIZE))
        {
            mp_sector_check = 1;
        }

        if (mp_sector_check == 1)
        {
            if (((MpInf.ver == MP_SECTOR_VERSION_ID_V1) && (MpInf.cal == MP_SECTOR_CALIBRATION_TOOL_V1)) ||
                (MpInf.cal == MP_SECTOR_CALIBRATION_TOOL_V2))
            {
                mp_sector_valid = MP_SECTOR_VALID_MPTOOL;

                break;
            }
        }

        if (OtpToMpCalibration() == STATUS_SUCCESS)
        {
            mp_sector_valid = MP_SECTOR_VALID_SWCAL;

            break;
        }

        if (mp_sector_check == 1)
        {
            if (((MpInf.ver == MP_SECTOR_VERSION_ID_V1) && (MpInf.cal == MP_SECTOR_CALIBRATION_SW_V1)) ||
                (MpInf.cal == MP_SECTOR_CALIBRATION_SW_V2))
            {
                mp_sector_valid = MP_SECTOR_VALID_SWDEFAULT;

                break;
            }
        }

    } while (0);

    switch (mp_sector_valid)
    {
    case MP_SECTOR_VALID_MPTOOL:
    case MP_SECTOR_VALID_SWDEFAULT:

        MpCalibrationAdcInit((mp_sector_cal_t *) MpInf.cal_data_sector_addr);

        break;

    case MP_SECTOR_VALID_SWCAL:
        MpCalibrationAdcInit((mp_sector_cal_t *) &otp_cal);
        break;

    default:
        break;
    }
}

/**
 * @ingroup mp_sector_group
 * @brief  read otp value and calibration to mp_sector
 * @return None
 */
uint32_t OtpPmuDataCheck(uint8_t * OtpPmuAry, uint8_t OtpPmuIdx)
{
    uint8_t i = 0;

    for (i = 0; i < OtpPmuIdx; i++)
    {
        if (OtpPmuAry[i] == 0xFF)
        {
            return STATUS_INVALID_PARAM;
        }
    }

    return STATUS_SUCCESS;
}
/**
 * @ingroup mp_sector_group
 * @brief  read otp value and calibration to mp_sector
 * @return None
 */
uint32_t OtpAdcDataCheck(uint16_t * OtpAdcAry, uint8_t OtpAdcIdx)
{

    uint8_t i = 0;

    for (i = 0; i < OtpAdcIdx; i++)
    {
        if (OtpAdcAry[i] == 0xFFFF)
        {
            return STATUS_INVALID_PARAM;
        }
    }

    return STATUS_SUCCESS;
}
/**
 * @ingroup mp_sector_group
 * @brief  read otp value and calibration to mp_sector
 * @return None
 */

uint32_t OtpToMpCalibration()
{
    static uint32_t otp_rd_buf_addr[64];

    otp_cal_regulator_t OtpPmuTemp;
    otp_cal_adc_t OtpAdcTemp;
    otp_cal_vcm_temp_adc_t OtpVcmTempAdcTmp;

    mp_cal_regulator_t OtpCalTmp;
    mp_cal_adc_t OtpCalAdcTmp;
    mp_cal_vcm_adc_t OtpCalVcmAdc;
    mp_cal_temp_adc_t OtpCalTempAdc;

    otp_cal = otp_cal_t;

    if (flash_read_otp_sec_page((uint32_t) otp_rd_buf_addr) != STATUS_SUCCESS)
    {
        return STATUS_INVALID_PARAM;
    }

    // MP_ID_DCDC
    OtpPmuTemp = *(otp_cal_regulator_t *) (otp_rd_buf_addr + OTP_DCDC_OFFSET);

    if (OtpPmuTemp.flag == 1) // 1:update 0:don't update
    {
        // if(OtpPmuDataCheck((otp_rd_buf+OTP_DCDC_OFFSET+1),6)){return STATUS_INVALID_PARAM;}
        OtpCalTmp = otp_cal.DCDC;

        OtpLoadPmuValue((mp_cal_regulator_t *) &OtpCalTmp, (otp_cal_regulator_t *) &OtpPmuTemp);

        OtpToMpVoselCal((mp_cal_regulator_t *) &OtpCalTmp);

        if (OtpPmuRangeCheck((mp_cal_regulator_t *) &OtpCalTmp, MP_ID_DCDC) == STATUS_SUCCESS)
        {
            otp_cal.DCDC = OtpCalTmp;
        }
        else
        {
            return STATUS_INVALID_PARAM;
        }
    }
    else if (OtpPmuTemp.flag == 0xFF) // flag 0:bypass 1:tool k 2:sw k
    {
        return STATUS_INVALID_PARAM;
    }

    // MP_ID_LLDO
    OtpPmuTemp = *(otp_cal_regulator_t *) (otp_rd_buf_addr + OTP_LLDO_OFFSET);

    if (OtpPmuTemp.flag == 1) // 1:update 0:don't update
    {
        // if(OtpPmuDataCheck((otp_rd_buf+OTP_LLDO_OFFSET+1),6)){ return STATUS_INVALID_PARAM;}

        OtpCalTmp = otp_cal.LLDO;

        OtpLoadPmuValue((mp_cal_regulator_t *) &OtpCalTmp, (otp_cal_regulator_t *) &OtpPmuTemp);

        OtpToMpVoselCal((mp_cal_regulator_t *) &OtpCalTmp);

        if (OtpPmuRangeCheck((mp_cal_regulator_t *) &OtpCalTmp, MP_ID_LLDO) == STATUS_SUCCESS)
        {
            otp_cal.LLDO = OtpCalTmp;
        }
        else
        {
            return STATUS_INVALID_PARAM;
        }
    }
    else if (OtpPmuTemp.flag == 0xFF) // flag 0:bypass 1:tool k 2:sw k
    {
        return STATUS_INVALID_PARAM;
    }

    // MP_ID_IOLDO
    OtpPmuTemp = *(otp_cal_regulator_t *) (otp_rd_buf_addr + OTP_IOLDO_OFFSET);

    if (OtpPmuTemp.flag == 1)
    {
        // if(OtpPmuDataCheck((otp_rd_buf+OTP_IOLDO_OFFSET+1),6)){return STATUS_INVALID_PARAM;}

        OtpCalTmp = otp_cal.IOLDO;

        OtpLoadPmuValue((mp_cal_regulator_t *) &OtpCalTmp, (otp_cal_regulator_t *) &OtpPmuTemp);

        OtpToMpVoselCal((mp_cal_regulator_t *) &OtpCalTmp);

        if (OtpPmuRangeCheck((mp_cal_regulator_t *) &OtpCalTmp, MP_ID_IOLDO) == STATUS_SUCCESS)
        {
            otp_cal.IOLDO = OtpCalTmp;
        }
        else
        {
            return STATUS_INVALID_PARAM;
        }
    }
    else if (OtpPmuTemp.flag == 0xFF) // flag 0:bypass 1:tool k 2:sw k
    {
        return STATUS_INVALID_PARAM;
    }
    // MP_ID_SLDO
    OtpPmuTemp = *(otp_cal_regulator_t *) (otp_rd_buf_addr + OTP_SLDO_OFFSET);

    if (OtpPmuTemp.flag == 1)
    {
        // if(OtpPmuDataCheck((otp_rd_buf+OTP_SLDO_OFFSET+1),6)){return STATUS_INVALID_PARAM;}

        OtpCalTmp = otp_cal.SLDO;

        OtpLoadPmuValue((mp_cal_regulator_t *) &OtpCalTmp, (otp_cal_regulator_t *) &OtpPmuTemp);

        OtpToMpVoselCal((mp_cal_regulator_t *) &OtpCalTmp);

        if (OtpPmuRangeCheck((mp_cal_regulator_t *) &OtpCalTmp, MP_ID_SLDO) == STATUS_SUCCESS)
        {
            otp_cal.SLDO = OtpCalTmp;
        }
        else
        {
            return STATUS_INVALID_PARAM;
        }
    }
    else if (OtpPmuTemp.flag == 0xFF) // flag 0:bypass 1:tool k 2:sw k
    {
        return STATUS_INVALID_PARAM;
    }
    // MP_ID_SIOLDO
    OtpPmuTemp = *(otp_cal_regulator_t *) (otp_rd_buf_addr + OTP_SIOLDO_OFFSET);

    if (OtpPmuTemp.flag == 1)
    {
        // if(OtpPmuDataCheck((otp_rd_buf+OTP_SIOLDO_OFFSET+1),6)){return STATUS_INVALID_PARAM;}

        OtpCalTmp = otp_cal.SIOLDO;

        OtpLoadPmuValue((mp_cal_regulator_t *) &OtpCalTmp, (otp_cal_regulator_t *) &OtpPmuTemp);

        OtpToMpVoselCal((mp_cal_regulator_t *) &OtpCalTmp);
        ;

        if (OtpPmuRangeCheck((mp_cal_regulator_t *) &OtpCalTmp, MP_ID_SIOLDO) == STATUS_SUCCESS)
        {
            otp_cal.SIOLDO = OtpCalTmp;
        }
        else
        {
            return STATUS_INVALID_PARAM;
        }
    }
    else if (OtpPmuTemp.flag == 0xFF) // flag 0:bypass 1:tool k 2:sw k
    {
        return STATUS_INVALID_PARAM;
    }
    // MP_ID_VBATADC
    OtpAdcTemp = *(otp_cal_adc_t *) (otp_rd_buf_addr + OTP_VBATADC_OFFSET);

    if (OtpAdcTemp.flag == 1)
    {
        // if( OtpAdcDataCheck((uint16_t*)(otp_rd_buf+OTP_VBATADC_OFFSET+1),6)) {return STATUS_INVALID_PARAM;}

        OtpCalAdcTmp = otp_cal.VBAT_ADC;

        OtpLoadAdcValue((mp_cal_adc_t *) &OtpCalAdcTmp, (otp_cal_adc_t *) &OtpAdcTemp);

        OtpToMpAdcCal((mp_cal_adc_t *) &OtpCalAdcTmp);

        if (OtpAdcRangeCheck((mp_cal_adc_t *) &OtpCalAdcTmp, MP_ID_VBATADC) == STATUS_SUCCESS)
        {
            otp_cal.VBAT_ADC = OtpCalAdcTmp;
        }
        else
        {
            return STATUS_INVALID_PARAM;
        }
    }
    else if (OtpAdcTemp.flag == 0xFF) // flag 0:bypass 1:tool k 2:sw k
    {
        return STATUS_INVALID_PARAM;
    }

    // MP_ID_AIOADC
    OtpAdcTemp = *(otp_cal_adc_t *) (otp_rd_buf_addr + OTP_AIOADC_OFFSET);

    if (OtpAdcTemp.flag == 1)
    {
        // if(OtpAdcDataCheck((uint16_t*)(otp_rd_buf+OTP_AIOADC_OFFSET+1),6)) {return STATUS_INVALID_PARAM;}

        OtpCalAdcTmp = otp_cal.AIO_ADC;

        OtpLoadAdcValue((mp_cal_adc_t *) &OtpCalAdcTmp, (otp_cal_adc_t *) &OtpAdcTemp);

        OtpToMpAdcCal((mp_cal_adc_t *) &OtpCalAdcTmp);

        if (OtpAdcRangeCheck((mp_cal_adc_t *) &OtpCalAdcTmp, MP_ID_AIOADC) == STATUS_SUCCESS)
        {
            otp_cal.AIO_ADC = OtpCalAdcTmp;
        }
        else
        {
            return STATUS_INVALID_PARAM;
        }
    }
    else if (OtpAdcTemp.flag == 0xFF) // flag 0:bypass 1:tool k 2:sw k
    {
        return STATUS_INVALID_PARAM;
    }

    // MP_ID_VCMADC
    OtpVcmTempAdcTmp = *(otp_cal_vcm_temp_adc_t *) (otp_rd_buf_addr + OTP_VCMADC_OFFSET);

    if (OtpVcmTempAdcTmp.flag == 1)
    {
        // if(OtpAdcDataCheck((uint16_t*)(otp_rd_buf+OTP_VCMADC_OFFSET+1),1)) {return STATUS_INVALID_PARAM;}

        OtpCalVcmAdc = otp_cal.VCM_ADC;

        OtpCalVcmAdc.adc_1 = OtpVcmTempAdcTmp.adc_1;

        if (OtpVcmTempAdcTmp.adc_1 > 0)
        {
            OtpCalVcmAdc.enable = 1;
        }

        if (OtpVcmTempAdcRangeCheck((mp_cal_vcm_adc_t *) &OtpCalVcmAdc, (mp_cal_temp_adc_t *) &OtpVcmTempAdcTmp, MP_ID_VCMADC) ==
            STATUS_SUCCESS)
        {

            otp_cal.VCM_ADC = OtpCalVcmAdc;
        }
        else
        {
            return STATUS_INVALID_PARAM;
        }
    }
    else if (OtpVcmTempAdcTmp.flag == 0xFF) // flag 0:bypass 1:tool k 2:sw k
    {
        return STATUS_INVALID_PARAM;
    }

    // MP_ID_TEMPADC
    OtpVcmTempAdcTmp = *(otp_cal_vcm_temp_adc_t *) (otp_rd_buf_addr + OTP_TEMPADC_OFFSET);

    if (OtpVcmTempAdcTmp.flag == 1)
    {
        // if(OtpAdcDataCheck((uint16_t*)(otp_rd_buf+OTP_TEMPADC_OFFSET+1),1)) {return STATUS_INVALID_PARAM;}

        OtpCalTempAdc = otp_cal.TEMP_ADC;

        OtpCalTempAdc.adc_1 = OtpVcmTempAdcTmp.adc_1;

        if (OtpVcmTempAdcRangeCheck((mp_cal_vcm_adc_t *) &OtpCalVcmAdc, (mp_cal_temp_adc_t *) &OtpCalTempAdc, MP_ID_TEMPADC) ==
            STATUS_SUCCESS)
        {
            otp_cal.TEMP_ADC = OtpCalTempAdc;
        }
        else
        {
            return STATUS_INVALID_PARAM;
        }
    }
    else if (OtpVcmTempAdcTmp.flag == 0xFF) // flag 0:bypass 1:tool k 2:sw k
    {
        return STATUS_INVALID_PARAM;
    }

    // MP_ID_POWERFAIL
    OtpPmuTemp = *(otp_cal_regulator_t *) (otp_rd_buf_addr + OTP_POWERFAIL_OFFSET);

    if (OtpPmuTemp.flag == 1)
    {
        // if(OtpPmuDataCheck((otp_rd_buf+OTP_SIOLDO_OFFSET+1),6)){return STATUS_INVALID_PARAM;}

        OtpCalTmp = otp_cal.POWER_FAIL;

        OtpLoadPmuValue((mp_cal_regulator_t *) &OtpCalTmp, (otp_cal_regulator_t *) &OtpPmuTemp);

        OtpToMpVoselCal((mp_cal_regulator_t *) &OtpCalTmp);

        if (OtpPmuRangeCheck((mp_cal_regulator_t *) &OtpCalTmp, MP_ID_POWERFAIL) == STATUS_SUCCESS)
        {
            otp_cal.POWER_FAIL = OtpCalTmp;
        }
        else
        {
            return STATUS_INVALID_PARAM;
        }
    }
    else if (OtpPmuTemp.flag == 0xFF) // flag 0:bypass 1:tool k 2:sw k
    {
        return STATUS_INVALID_PARAM;
    }

    return STATUS_SUCCESS;
}

/** @} */ /* end of examples group */
