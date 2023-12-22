// *** <<< Use Configuration Wizard in Context Menu >>> ***
/**************************************************************************//**
 * @file     rf_mcu_chip.h
 * @version
 * $Revision:
 * $Date:
 * @brief
 * @note
 * Copyright (C) 2019 Rafael Microelectronics Inc. All rights reserved.
 *
 ******************************************************************************/
#ifndef _RF_MCU_CHIP_H_
#define _RF_MCU_CHIP_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
*   CONSTANT AND DEFINE
*******************************************************************************/
/* Chip model */
#define RF_MCU_CHIP_569                     (0x10)
#define RF_MCU_CHIP_569T3                   (0x11)
#define RF_MCU_CHIP_569MP                   (0x20)
#define RF_MCU_CHIP_569M0                   (0x30)
#define RF_MCU_CHIP_UNDEFINE                (0xFF)
#define RF_MCU_CHIP_MODEL                   (RF_MCU_CHIP_569MP)

/* Chip version */
#define RF_MCU_CHIP_VER_A                   (0x00)
#define RF_MCU_CHIP_VER_B                   (0x01)
#define RF_MCU_CHIP_VER_C                   (0x02)
#define RF_MCU_CHIP_VER_UNDEFINE            (0xFF)
#if (RF_MCU_CHIP_MODEL == RF_MCU_CHIP_569M0)
#define RF_MCU_CHIP_VER                     (RF_MCU_CHIP_VER_B)
#else
#define RF_MCU_CHIP_VER                     (RF_MCU_CHIP_VER_B)
#endif

/* Platform type */
#define RF_MCU_TYPE_FPGA_RFT3               (0x02)
#define RF_MCU_TYPE_FPGA_RFM0               (0x03)
#define RF_MCU_TYPE_ASIC                    (0x10)
#define RF_MCU_TYPE_UNDEFINE                (0xFF)
#if (RF_MCU_CHIP_MODEL == RF_MCU_CHIP_569M0)
#define RF_MCU_CHIP_TYPE                    (RF_MCU_TYPE_ASIC)
#else
#define RF_MCU_CHIP_TYPE                    (RF_MCU_TYPE_ASIC)
#endif

/* Program type */
#define BASE_RAM_TYPE                       (0x00)
#define BASE_ROM_TYPE                       (0x01)
#if ((RF_MCU_CHIP_MODEL == RF_MCU_CHIP_569M0) && (RF_MCU_CHIP_TYPE == RF_MCU_TYPE_ASIC))
#define RF_MCU_CHIP_BASE                    (BASE_ROM_TYPE)
#else
#define RF_MCU_CHIP_BASE                    (BASE_RAM_TYPE)
#endif

/* FW target */
#define RF_MCU_FW_TARGET_MAC                (0x00)
#define RF_MCU_FW_TARGET_BLE                (0x01)
#define RF_MCU_FW_TARGET_MULTI_PROTOCOL     (0x02)
#define RF_MCU_FW_TARGET_UNDEFINE           (0xFF)
#if ((RF_MCU_CHIP_MODEL == RF_MCU_CHIP_569M0) && (RF_MCU_CHIP_TYPE == RF_MCU_TYPE_ASIC) && (RF_MCU_CHIP_BASE == BASE_ROM_TYPE))
#define RF_MCU_FW_TARGET                    (RF_MCU_FW_TARGET_MAC)
#else
#define RF_MCU_FW_TARGET                    (RF_MCU_FW_TARGET_UNDEFINE)
#endif

/* Chip ID */
#define RF_MCU_CHIP_ID_569M0                (0x6A)
#define RF_MCU_CHIP_ID_UNDEFINE             (0xFF)
#if (RF_MCU_CHIP_MODEL == RF_MCU_CHIP_569M0)
#define RF_MCU_CHIP_ID                      (RF_MCU_CHIP_ID_569M0)
#else
#define RF_MCU_CHIP_ID                      (RF_MCU_CHIP_ID_UNDEFINE)
#endif

/* Interface type */
#define RF_MCU_CTRL_BY_AHB                  (0x00)
#define RF_MCU_CTRL_BY_SPI                  (0x01)
#if (RF_MCU_CHIP_MODEL == RF_MCU_CHIP_569M0)
#define CFG_RF_MCU_CTRL_TYPE                (RF_MCU_CTRL_BY_SPI)
#else
#define CFG_RF_MCU_CTRL_TYPE                (RF_MCU_CTRL_BY_AHB)
#endif

/* Patch type */
#if (RF_MCU_CHIP_MODEL == RF_MCU_CHIP_569M0)
#if (RF_MCU_FW_TARGET == RF_MCU_FW_TARGET_MAC)
#define RF_MCU_PATCH_SUPPORTED              (0x01)
#else
#define RF_MCU_PATCH_SUPPORTED              (0x00)
#endif
#else
#define RF_MCU_PATCH_SUPPORTED              (0x00)
#endif

/* Const Load Enable */
#define RF_MCU_CONST_LOAD_SUPPORTED         (0x00)
#if (RF_MCU_CONST_LOAD_SUPPORTED == 1)
#undef RF_MCU_PATCH_SUPPORTED
#define RF_MCU_PATCH_SUPPORTED              (0x00)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _RF_MCU_CHIP_H_ */


