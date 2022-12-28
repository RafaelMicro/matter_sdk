/** @file mp_sector.h
 *
 * @brief MP sector driver header file.
 *
 */
/**
 * @defgroup mp_sector_group MP_Sector
 * @ingroup peripheral_group
 * @{
 * @brief Define MP_sector definitions, structures, and functions.
 */
#ifndef __MP_SECTOR_H__
#define __MP_SECTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include <stdint.h>


/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/
#ifdef __cplusplus
#define   __I     volatile             /*!< Defines 'read only' permissions */
#else
#define   __I     volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */


/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define MP_SECTOR_VERSION_CODE         (0x58570000)
#define MP_SECTOR_VERSION_CODE_MASK    (0xFFFF0000)
#define MP_SECTOR_VERSION_ID_V1        (MP_SECTOR_VERSION_CODE + 0x0001)
#define MP_SECTOR_VERSION_ID_V2        (MP_SECTOR_VERSION_CODE + 0x0002)
#define MP_SECTOR_VERSION_ID           (MP_SECTOR_VERSION_ID_V2)

#define MP_SECTOR_CALIBRATION_TOOL_V1  (0xFFFFFFFF)
#define MP_SECTOR_CALIBRATION_TOOL_V2  (0x5A3C0001)
#define MP_SECTOR_CALIBRATION_SW_V1    (0xFFFFFFFE)
#define MP_SECTOR_CALIBRATION_SW_V2    (0x5A3C0002)

#if (MP_SECTOR_VERSION_ID == MP_SECTOR_VERSION_ID_V1)
#define MP_SECTOR_CALIBRATION_TOOL  (MP_SECTOR_CALIBRATION_TOOL_V1)
#define MP_SECTOR_CALIBRATION_SW    (MP_SECTOR_CALIBRATION_SW_V1)
#else
#define MP_SECTOR_CALIBRATION_TOOL  (MP_SECTOR_CALIBRATION_TOOL_V2)
#define MP_SECTOR_CALIBRATION_SW    (MP_SECTOR_CALIBRATION_SW_V2)
#endif

#define MP_SECTOR_BASE_512K             (0x00070000UL)

#define MP_SECTOR_RESERVED_ADDR_512K    (MP_SECTOR_BASE_512K + 0x0000UL)
#define MP_SECTOR_ZIGBEE_ADDR_512K      (MP_SECTOR_BASE_512K + 0x4000UL)
#define MP_SECTOR_BLE_ADDR_512K         (MP_SECTOR_BASE_512K + 0xC000UL)
#define MP_SECTOR_CAL_ADDR_512K         (MP_SECTOR_BASE_512K + 0xE000UL)
#define MP_SECTOR_INFO_ADDR_512K        (MP_SECTOR_BASE_512K + 0xF000UL)

#define MP_SECTOR_RESERVED_512K         ((mp_sector_reserved_t *) MP_SECTOR_RESERVED_ADDR_512K)
#define MP_SECTOR_ZIGBEE_512K           ((mp_sector_zigbee_t *) MP_SECTOR_ZIGBEE_ADDR_512K)
#define MP_SECTOR_BLE_512K              ((mp_sector_ble_t *) MP_SECTOR_BLE_ADDR_512K)
#define MP_SECTOR_CAL_512K              ((mp_sector_cal_t *) MP_SECTOR_CAL_ADDR_512K)
#define MP_SECTOR_INFO_512K             ((mp_sector_info_t *) MP_SECTOR_INFO_ADDR_512K)

#define MP_SECTOR_BASE_1024K             (0x000F0000UL)

#define MP_SECTOR_RESERVED_ADDR_1024K   (MP_SECTOR_BASE_1024K + 0x0000UL)
#define MP_SECTOR_ZIGBEE_ADDR_1024K     (MP_SECTOR_BASE_1024K + 0x4000UL)
#define MP_SECTOR_BLE_ADDR_1024K        (MP_SECTOR_BASE_1024K + 0xC000UL)
#define MP_SECTOR_CAL_ADDR_1024K        (MP_SECTOR_BASE_1024K + 0xE000UL)
#define MP_SECTOR_INFO_ADDR_1024K       (MP_SECTOR_BASE_1024K + 0xF000UL)

#define MP_SECTOR_RESERVED_1024K        ((mp_sector_reserved_t *) MP_SECTOR_RESERVED_ADDR_1024K)
#define MP_SECTOR_ZIGBEE_1024K          ((mp_sector_zigbee_t *) MP_SECTOR_ZIGBEE_ADDR_1024K)
#define MP_SECTOR_BLE_1024K             ((mp_sector_ble_t *) MP_SECTOR_BLE_ADDR_1024K)
#define MP_SECTOR_CAL_1024K             ((mp_sector_cal_t *) MP_SECTOR_CAL_ADDR_1024K)
#define MP_SECTOR_INFO_1024K            ((mp_sector_info_t *) MP_SECTOR_INFO_ADDR_1024K)

#define MP_SECTOR_BASE_2048K             (0x001F0000UL)

#define MP_SECTOR_RESERVED_ADDR_2048K   (MP_SECTOR_BASE_2048K + 0x0000UL)
#define MP_SECTOR_ZIGBEE_ADDR_2048K     (MP_SECTOR_BASE_2048K + 0x4000UL)
#define MP_SECTOR_BLE_ADDR_2048K        (MP_SECTOR_BASE_2048K + 0xC000UL)
#define MP_SECTOR_CAL_ADDR_2048K        (MP_SECTOR_BASE_2048K + 0xE000UL)
#define MP_SECTOR_INFO_ADDR_2048K       (MP_SECTOR_BASE_2048K + 0xF000UL)

#define MP_SECTOR_RESERVED_2048K        ((mp_sector_reserved_t *) MP_SECTOR_RESERVED_ADDR_2048K)
#define MP_SECTOR_ZIGBEE_2048K          ((mp_sector_zigbee_t *) MP_SECTOR_ZIGBEE_ADDR_2048K)
#define MP_SECTOR_BLE_2048K             ((mp_sector_ble_t *) MP_SECTOR_BLE_ADDR_2048K)
#define MP_SECTOR_CAL_2048K             ((mp_sector_cal_t *) MP_SECTOR_CAL_ADDR_2048K)
#define MP_SECTOR_INFO_2048K            ((mp_sector_info_t *) MP_SECTOR_INFO_ADDR_2048K)
/*
#define MP_SECTOR_RESERVED_SIZE     (sizeof(mp_sector_reserved_t))
#define MP_SECTOR_ZIGBEE_SIZE       (sizeof(mp_sector_zigbee_t))
#define MP_SECTOR_BLE_SIZE          (sizeof(mp_sector_ble_t))
#define MP_SECTOR_CAL_SIZE          (sizeof(mp_sector_cal_t))
#define MP_SECTOR_INFO_SIZE         (sizeof(mp_sector_info_t))
*/
#define MP_SECTOR_RESERVED_SIZE     (0x4000)
#define MP_SECTOR_ZIGBEE_SIZE       (0x8000)
#define MP_SECTOR_BLE_SIZE          (0x2000)
#define MP_SECTOR_CAL_SIZE          (0x1000)
#define MP_SECTOR_INFO_SIZE         (0x1000)
#define MP_SECTOR_TOTAL_SIZE        (MP_SECTOR_RESERVED_SIZE + MP_SECTOR_ZIGBEE_SIZE + MP_SECTOR_BLE_SIZE + MP_SECTOR_CAL_SIZE + MP_SECTOR_INFO_SIZE)
/*
#if (sizeof(mp_sector_reserved_t) > MP_SECTOR_RESERVED_SIZE)
#error MP Sector Reserved Structure size exceeds definition
#endif
#if (sizeof(mp_sector_zigbee_t) > MP_SECTOR_ZIGBEE_SIZE)
#error MP Sector Zigbee Structure size exceeds definition
#endif
#if (sizeof(mp_sector_ble_t) > MP_SECTOR_BLE_SIZE)
#error MP Sector BLE Structure size exceeds definition
#endif
#if (sizeof(mp_sector_cal_t) > MP_SECTOR_CAL_SIZE)
#error MP Sector Calibration Structure size exceeds definition
#endif
#if (sizeof(mp_sector_info_t) > MP_SECTOR_INFO_SIZE)
#error MP Sector Information Structure size exceeds definition
#endif
*/


#define MP_NULL                     (0xFF)
#define MP_VALID                    (0x7E)
#define MP_INVALID                  (0x00)

#define MP_ID_DCDC                  (0x5A010001)
#define MP_ID_LLDO                  (0x5A010002)
#define MP_ID_IOLDO                 (0x5A010003)
#define MP_ID_SLDO                  (0x5A010004)
#define MP_ID_SIOLDO                (0x5A010005)
#define MP_ID_VBATADC               (0x5A010006)
#define MP_ID_AIOADC                (0x5A010007)
#define MP_ID_VCMADC                (0x5A010008)
#define MP_ID_TEMPADC               (0x5A010009)
#define MP_ID_POWERFAIL             (0x5A01000A)
#define MP_ID_CRYSTALTRIM           (0x5A01000B)
#define MP_ID_RF_BAND_SUPPORT       (0x5A01000C)
#define MP_ID_RFTRIM_2P4G           (0x5A01000D)
#define MP_ID_RFTRIM_SUBG0          (0x5A01000E)
#define MP_ID_RFTRIM_SUBG1          (0x5A01000F)
#define MP_ID_RFTRIM_SUBG2          (0x5A010010)
#define MP_ID_TX_POWER_TRIM         (0x5A010011)
#define MP_ID_RSSI_TRIM             (0x5A010012)
#define MP_ID_TEMPK                 (0x5A010013)
#define MP_ID_NULL                  (0xFFFFFFFF)

#define MP_CNT_DCDC                 (sizeof(mp_cal_regulator_t))
#define MP_CNT_LLDO                 (sizeof(mp_cal_regulator_t))
#define MP_CNT_IOLDO                (sizeof(mp_cal_regulator_t))
#define MP_CNT_SLDO                 (sizeof(mp_cal_regulator_t))
#define MP_CNT_SIOLDO               (sizeof(mp_cal_regulator_t))
#define MP_CNT_VBATADC              (sizeof(mp_cal_adc_t))
#define MP_CNT_AIOADC               (sizeof(mp_cal_adc_t))
#define MP_CNT_VCMADC               (sizeof(mp_cal_vcm_adc_t))
#define MP_CNT_TEMPADC              (sizeof(mp_cal_temp_adc_t))
#define MP_CNT_POWERFAIL            (sizeof(mp_cal_regulator_t))
#define MP_CNT_CRYSTALTRIM          (sizeof(mp_cal_xtal_trim_t))
#define MP_CNT_RF_BAND_SUPPORT      (sizeof(mp_cal_rf_band_support_t))
#define MP_CNT_RFTRIM               (sizeof(mp_cal_rf_trim_t))
#define MP_CNT_TX_POWER_TRIM        (sizeof(mp_tx_power_trim_t))
#define MP_CNT_RSSI_TRIM            (sizeof(mp_rssi_trim_t))
#define MP_CNT_TEMPK                (sizeof(mp_temp_k_t))


#define  OTP_DCDC_OFFSET            0x08
#define  OTP_LLDO_OFFSET            0x0C
#define  OTP_IOLDO_OFFSET           0x10
#define  OTP_SLDO_OFFSET            0x14
#define  OTP_SIOLDO_OFFSET          0x18
#define  OTP_VBATADC_OFFSET         0x1C
#define  OTP_AIOADC_OFFSET          0x20
#define  OTP_VCMADC_OFFSET          0x24
#define  OTP_TEMPADC_OFFSET         0x28
#define  OTP_POWERFAIL_OFFSET       0x2C
/**************************************************************************************************
 *    TYPEDEFS
 *************************************************************************************************/
typedef enum
{
    MP_SECTOR_INVALID = 0,
    MP_SECTOR_VALID_MPTOOL,
    MP_SECTOR_VALID_SWDEFAULT,
    MP_SECTOR_VALID_SWCAL,
} mp_sector_valid_t;

typedef struct __attribute__((packed))
{
    uint32_t mp_id;
    uint8_t  mp_valid;
    uint16_t mp_cnt;
}
mp_sector_head_t;

typedef struct __attribute__((packed))
{
    mp_sector_head_t head;
    uint8_t  flag;
    uint16_t voltage_1;
    uint8_t  vosel_1;
    uint16_t voltage_2;
    uint8_t  vosel_2;
    uint8_t  select;
    uint16_t target_voltage_1;
    uint8_t  target_vosel_1;
    uint16_t target_voltage_2;
    uint8_t  target_vosel_2;
    uint16_t target_voltage_3;
    uint8_t  target_vosel_3;
}
mp_cal_regulator_t;

typedef struct __attribute__((packed))
{
    mp_sector_head_t head;
    uint8_t  flag;
    uint16_t voltage_1;
    uint16_t adc_1;
    uint16_t voltage_2;
    uint16_t adc_2;
    uint16_t target_voltage_1;
    uint16_t target_adc_1;
    uint16_t target_voltage_2;
    uint16_t target_adc_2;
    uint16_t target_voltage_3;
    uint16_t target_adc_3;
}
mp_cal_adc_t;

typedef struct __attribute__((packed))
{
    mp_sector_head_t head;
    uint8_t  flag;
    uint8_t  enable;
    uint16_t adc_1;
}
mp_cal_vcm_adc_t;

typedef struct __attribute__((packed))
{
    mp_sector_head_t head;
    uint8_t  flag;
    uint16_t adc_1;
}
mp_cal_temp_adc_t;

typedef struct __attribute__((packed))
{
    mp_sector_head_t head;
    uint8_t  flag;
    uint16_t xo_trim;
}
mp_cal_xtal_trim_t;

typedef struct __attribute__((packed))
{
    mp_sector_head_t head;
    uint8_t  flag;
    uint8_t  rf_band;
}
mp_cal_rf_band_support_t;

typedef struct __attribute__((packed))
{
    mp_sector_head_t head;
    uint8_t  flag;
    uint8_t  mode;
    uint16_t cal_cfg;
    uint16_t rx_filter_cap;
    uint16_t tx_dc_offset_i;
    uint16_t tx_dc_offset_q;
    uint16_t tx_iqc_a;
    uint16_t tx_iqc_b;
    uint16_t tx_iqc_c;
}
mp_cal_rf_trim_t;

typedef struct __attribute__((packed))
{
    mp_sector_head_t head;
    uint8_t  flag;
    uint8_t  mode;
    uint8_t  tx_gain_idx_2g;
    uint8_t  tx_gain_idx_subg0;
    uint8_t  tx_gain_idx_subg1;
    uint8_t  tx_gain_idx_subg2;
}
mp_tx_power_trim_t;

typedef struct __attribute__((packed))
{
    mp_sector_head_t head;
    uint8_t  flag;
    uint8_t  mode;
    uint8_t  offset_2g;
    uint8_t  offset_subg0;
    uint8_t  offset_subg1;
    uint8_t  offset_subg2;
}
mp_rssi_trim_t;


typedef struct __attribute__((packed))
{
    mp_sector_head_t head;
    uint8_t  flag;
    uint16_t ktvalue;
}
mp_temp_k_t;
/*
typedef struct __attribute__((packed)) {
    __IO  uint8_t  RESERVED[0x4000];               //offset: 0x0000
} mp_sector_reserved_t;
*/
/*
typedef struct __attribute__((packed)) {
    __IO  uint8_t  RESERVED[0x8000];               //offset: 0x0000
} mp_sector_zigbee_t;
*/
/*
typedef struct __attribute__((packed)) {
    __IO  uint8_t  RESERVED[0x2000];               //offset: 0x0000
} mp_sector_ble_t;
*/

typedef struct __attribute__((packed))
{
    __IO  mp_cal_regulator_t DCDC;                      //offset: 0x0000
    __IO  mp_cal_regulator_t LLDO;                      //offset: 0x0018
    __IO  mp_cal_regulator_t IOLDO;                     //offset: 0x0030
    __IO  mp_cal_regulator_t SLDO;                      //offset: 0x0048
    __IO  mp_cal_regulator_t SIOLDO;                    //offset: 0x0060
    __IO  mp_cal_adc_t VBAT_ADC;                        //offset: 0x0078
    __IO  mp_cal_adc_t AIO_ADC;                         //offset: 0x0094
    __IO  mp_cal_vcm_adc_t VCM_ADC;                     //offset: 0x00B0
    __IO  mp_cal_temp_adc_t TEMP_ADC;                   //offset: 0x00BB
    __IO  mp_cal_regulator_t POWER_FAIL;                //offset: 0x00C5
    __IO  mp_cal_xtal_trim_t CRYSTAL_TRIM;              //offset: 0x00DD
    __IO  mp_cal_rf_band_support_t RF_BAND_SUPPORT;     //offset: 0x00E7
    __IO  mp_cal_rf_trim_t RF_TRIM_2P4G;                //offset: 0x00F0
    __IO  mp_cal_rf_trim_t RF_TRIM_SUBG0;               //offset: 0x0107
    __IO  mp_cal_rf_trim_t RF_TRIM_SUBG1;               //offset: 0x011E
    __IO  mp_cal_rf_trim_t RF_TRIM_SUBG2;               //offset: 0x0135
    __IO  mp_tx_power_trim_t TX_POWER_TRIM;             //offset: 0x014C
    __IO  mp_rssi_trim_t RSSI_TRIM;                     //offset: 0x0158
    __IO  mp_temp_k_t     TEMP_K;                       //offset: 0x0162
    /*
        __IO  uint8_t  RESERVED[0xEB9];                     //offset: 0x0147
    */
}
mp_sector_cal_t;


typedef struct __attribute__((packed))
{
    __IO  uint32_t  CAL_DATA_SECTOR_ADDR;          //offset: 0x0000
    __IO  uint32_t  CAL_DATA_SECTOR_SIZE;          //offset: 0x0004
    __IO  uint32_t  BLE_INFO_SECTOR_ADDR;          //offset: 0x0008
    __IO  uint32_t  BLE_INFO_SECTOR_SIZE;          //offset: 0x000C
    __IO  uint32_t  ZIGBEE_INFO_SECTOR_ADDR;       //offset: 0x0010
    __IO  uint32_t  ZIGBEE_INFO_SECTOR_SIZE;       //offset: 0x0014
    __IO  uint32_t  RESERVED[0x3F7];               //offset: 0x0018
    __IO  uint32_t  MP_SECTOR_CALIBRATION;         //offset: 0x0FF4
    __IO  uint32_t  MP_SECTOR_VERSION;             //offset: 0x0FF8
    __IO  uint32_t  MP_SECTOR_SIZE;                //offset: 0x0FFC
}
mp_sector_info_t;





typedef struct __attribute__((packed))
{
    uint8_t     flag;
    uint16_t    voltage_1;
    uint8_t     vosel_1;
    uint16_t    voltage_2;
    uint8_t     vosel_2;
}
otp_cal_regulator_t;

typedef struct __attribute__((packed))
{
    uint8_t     flag;
    uint16_t    voltage_1;
    uint16_t    adc_1;
    uint16_t    voltage_2;
    uint16_t    adc_2;
    uint16_t    voltage_3;
    uint16_t    adc_3;
}
otp_cal_adc_t;

typedef struct __attribute__((packed))
{
    uint8_t  flag;
    uint16_t adc_1;
}
otp_cal_vcm_temp_adc_t;


typedef struct __attribute__((packed))
{
    uint32_t   ver;
    uint32_t   size;
    uint32_t   cal;
    uint32_t   cal_data_sector_size;
    uint32_t   cal_data_sector_addr;
}
mp_sector_inf_t;


/**************************************************************************************************
 *    Global Prototypes
 *************************************************************************************************/
//extern const mp_sector_zigbee_t mp_sector_zigbee;
//extern const mp_sector_ble_t mp_sector_ble;
extern const mp_sector_cal_t mp_sector_cal;
extern const mp_sector_info_t mp_sector_info;


/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/

/**
* @brief  check calcation vosel value
* @param[in] targetvosel
* @param[in] id
* @return None
*/
uint8_t OtpToMpPmuMaxCheck(int32_t targetvosel, uint32_t id);
/**
 * @brief load otp value into mp_sector table struct;
 * @param[in] mp_cal
 *           \arg mp_sector_head_t head;
 *           \arg flag;
 *           \arg voltage_1;
 *           \arg vosel_1;
 *           \arg voltage_2;
 *           \arg vosel_2;
 *           \arg select;
 *           \arg target_voltage_1;
 *           \arg target_vosel_1;
 *           \arg target_voltage_2;
 *           \arg target_vosel_2;
 *           \arg target_voltage_3;
 *           \arg target_vosel_3;
 * @param[in] otp_cal
 *           \arg flag;
 *           \arg voltage_1;
 *           \arg vosel_1;
 *           \arg voltage_2;
 *           \arg vosel_2;
 * @retval None
 */
void OtpLoadPmuValue(mp_cal_regulator_t *mp_pmu_cal, otp_cal_regulator_t *otp_pmu_cal);
/**
 * @brief load otp value into mp_sector table struct;
 * @param[in] mp_adc_cal
 *           \arg mp_sector_head_t head;
 *           \arg flag;
 *           \arg voltage_1;
 *           \arg adc_1;
 *           \arg voltage_2;
 *           \arg adc_2;
 *           \arg select;
 *           \arg target_voltage_1;
 *           \arg target_adc_1;
 *           \arg target_voltage_2;
 *           \arg target_adc_2;
 *           \arg target_voltage_3;
 *           \arg target_adc_3;
 * @param[in] otp_adc_cal
 *           \arg flag;
 *           \arg voltage_1;
 *           \arg adc_1;
 *           \arg voltage_2;
 *           \arg adc_2;
 *           \arg voltage_3;
 *           \arg adc_3;
 * @retval None
 */
void OtpLoadAdcValue(mp_cal_adc_t *mp_adc_cal, otp_cal_adc_t *otp_adc_cal);

/**
 * @brief Otp val
 * @param[in] mp_cal_reg
 *           \arg mp_sector_head_t head;
 *           \arg flag;
 *           \arg voltage_1;
 *           \arg vosel_1;
 *           \arg voltage_2;
 *           \arg vosel_2;
 *           \arg select;
 *           \arg target_voltage_1;
 *           \arg target_vosel_1;
 *           \arg target_voltage_2;
 *           \arg target_vosel_2;
 *           \arg target_voltage_3;
 *           \arg target_vosel_3;
 * @param[in] mp_id
 * @retval STATUS_SUCCESS
 * @retval STATUS_INVALID_PARAM
 */
uint32_t OtpPmuRangeCheck(mp_cal_regulator_t *mp_pmu_cal, uint32_t mp_id);
/**
 * @brief Otp val
 * @param[in] mp_cal_reg
 *           \arg mp_sector_head_t head;
 *           \arg flag;
 *           \arg voltage_1;
 *           \arg adc_1;
 *           \arg voltage_2;
 *           \arg adc_2;
 *           \arg select;
 *           \arg target_voltage_1;
 *           \arg target_adc_1;
 *           \arg target_voltage_2;
 *           \arg target_adc_2;
 *           \arg target_voltage_3;
 *           \arg target_adc_3;
 * @param[in] mp_id
 * @retval STATUS_SUCCESS
 * @retval STATUS_INVALID_PARAM
 */
uint32_t OtpAdcRangeCheck(mp_cal_adc_t *mp_adc_cal_reg, uint32_t mp_id);
/**
 * @brief Otp val
 * @param[in] mp_cal_reg
 *           \arg mp_sector_head_t head;
 *           \arg flag;
 *           \arg enable;
 *           \arg adc_1;
 * @param[in] tmp_mp_cal_reg
 *           \arg mp_sector_head_t head;
 *           \arg flag;
 *           \arg adc_1;
 * @param[in] mp_id
 * @retval STATUS_SUCCESS
 * @retval STATUS_INVALID_PARAM
 */

uint32_t OtpVcmTempAdcRangeCheck(mp_cal_vcm_adc_t *mp_cal_reg, mp_cal_temp_adc_t *tmp_mp_cal_reg, uint32_t mp_id);
/**
 * @brief Otp pmu value to mp_sector calcation
 * @param[in]  mp_pmu_cal
 *           \arg mp_sector_head_t head;
 *           \arg flag;
 *           \arg voltage_1;
 *           \arg vosel_1;
 *           \arg voltage_2;
 *           \arg vosel_2;
 *           \arg select;
 *           \arg target_voltage_1;
 *           \arg target_vosel_1;
 *           \arg target_voltage_2;
 *           \arg target_vosel_2;
 *           \arg target_voltage_3;
 *           \arg target_vosel_3;
 * @retval None
 */
void OtpToMpVoselCal(mp_cal_regulator_t *mp_pmu_cal);
/**
 * @brief Otp adc value to mp_sector calcation
 *           \arg mp_sector_head_t head;
 *           \arg flag;
 *           \arg voltage_1;
 *           \arg adc_1;
 *           \arg voltage_2;
 *           \arg adc_2;
 *           \arg select;
 *           \arg target_voltage_1;
 *           \arg target_adc_1;
 *           \arg target_voltage_2;
 *           \arg target_adc_2;
 *           \arg target_voltage_3;
 *           \arg target_adc_3;
 * @retval None
 */
void OtpToMpAdcCal(mp_cal_adc_t *mp_cal);
/**
 * @brief Otp to mp_sector value calibration function
 * @param[in]  OtpPmuAry
 * @param[in]  OtpPmuIdx
 * @retval STATUS_SUCCESS
 * @retval STATUS_INVALID_PARAM
 */
uint32_t OtpPmuDataCheck(uint8_t *OtpPmuAry, uint8_t OtpPmuIdx);
/**
* @ingroup mp_sector_group
* @param[in]  OtpAdcAry
* @param[in]  OtpAdcIdx
* @retval STATUS_SUCCESS
* @retval STATUS_INVALID_PARAM
*/
uint32_t OtpAdcDataCheck(uint16_t *OtpAdcAry, uint8_t OtpAdcIdx);
/**
 * @brief Otp to mp_sector value calibration function
 * @param  Nonoe
 * @retval None
 */
uint32_t OtpToMpCalibration(void);

/**
* @ingroup mp_sector_group
* @brief Function to get the Mp sector information initinal value
* @param[in] MpSectorInf
 *            \arg ver
 *            \arg size
 *            \arg cal
 *            \arg cal_data_sector_size
 *            \arg cal_data_sector_addr
* @retval STATUS_SUCCESS
* @retval STATUS_INVALID_PARAM
*/
uint32_t GetMpSectorInfo(mp_sector_inf_t *MpSectorInf);

/**
 * @brief Function to initinal the Mp sector calibration the value
 */
void MpSectorInit(void);

/**
 * @brief Function write rf mp secotr value
 * @param[in] mp_id
 * @param[in] mp_cal_rf
 *            \arg mp_sector_head_t head;
 *            \arg flag
 *            \arg mode
 *            \arg cal_cfg
 *            \arg rx_filter_cap
 *            \arg tx_dc_offset_i
 *            \arg tx_dc_offset_q
 *            \arg tx_iqc_a
 *            \arg tx_iqc_b
 *            \arg tx_iqc_c
 * @retval STATUS_SUCCESS rf trim is successful
 * @retval STATUS_ERROR   write address is invaild
 */
uint32_t MpCalRftrimWrite(uint32_t mp_id, mp_cal_rf_trim_t *mp_cal_rf);

/**
 * @brief Function read rf mp secotr value
 * @param[in] mp_id
 * @param[in] byte_cnt read data bytes count
 * @param[out] mp_sec_data point a mp_sector data buffer
 * @retval STATUS_SUCCESS read mp sector value is succesful
 * @retval STATUS_INVALID_REQUEST read mp sector value is invaild
 */
uint32_t MpCalRftrimRead(uint32_t mp_id, uint32_t byte_cnt, uint8_t *mp_sec_data);

void MpCalAdcInit(void);
//uint32_t MpCalVbatAdc(sadc_value_t adc_val);
uint32_t MpCalVbatAdcRead(mp_cal_adc_t *mp_cal_adc);
uint32_t MpCalAioAdcRead(mp_cal_adc_t *mp_cal_adc);
uint32_t MpCalVcmAdcRead(mp_cal_vcm_adc_t *mp_cal_vcmadc);
uint32_t MpCalTempAdcRead(mp_cal_temp_adc_t *mp_cal_tempadc);
uint32_t MpCalKtRead(mp_temp_k_t *mp_cal_k);


#ifdef __cplusplus
};
#endif

#endif /* end of __MP_SECTOR_H__ */

/** @} */ /* End of Peripheral Group */
