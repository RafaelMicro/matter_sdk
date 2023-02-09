/**************************************************************************//**
 * @file     flashctl_reg.h
 * @version  V1.00
 * @brief    flash controller register definition header file
 *
 * @copyright
 *****************************************************************************/
#ifndef ___FLASHCTL_REG_H__
#define ___FLASHCTL_REG_H__

#if defined (__CC_ARM)
#pragma anon_unions
#endif



typedef struct
{
    __IO uint32_t COMMAND;        /*0x00*/
    __IO uint32_t FLASH_ADDR;     /*0x04*/
    __IO uint32_t START;          /*0x08*/
    __IO uint32_t STATUS;         /*0x0C*/
    __IO uint32_t FLASH_DATA;     /*0x10*/
    __IO uint32_t MEM_ADDR;       /*0x14*/
    __IO uint32_t CONTROL_SET;    /*0x18*/
    __IO uint32_t CRC;            /*0x1C*/
    __IO uint32_t DPD;            /*0x20*/
    __IO uint32_t RDPD;           /*0x24*/
    __IO uint32_t SUSPEND;        /*0x28*/
    __IO uint32_t RESUME;         /*0x2C*/
    __IO uint32_t FLASH_INSTR;    /*0x30*/
    __IO uint32_t PAGE_READ_WORD; /*0x34*/
    __IO uint32_t FLASH_INFO;     /*0x38*/
    __IO uint32_t RESV;           /*0x3C*/
    __IO uint32_t FLASH_INT;      /*0x40*/
    __IO uint32_t PATTERN;        /*0x44*/
} FLASHCTL_T;



#define  CMD_READBYTE       0x00

#define  CMD_READVERIFY     0x04
#define  CMD_READPAGE       0x08
#define  CMD_READUID        0x09

#define  CMD_READ_STATUS1      0x0D
#define  CMD_READ_STATUS2      0x0E
#define  CMD_READ_STATUS3      0x0F

#define  CMD_ERASEPAGE         0x20
#define  CMD_ERASESECTOR       0x21
#define  CMD_ERASE_BL32K       0x22
#define  CMD_ERASE_BL64K       0x24

#define  CMD_WRITEBYTE         0x10
#define  CMD_WRITEPAGE         0x18

#define  CMD_WRITE_STATUS      0x1C
#define  CMD_WRITE_STATUS1     0x1D
#define  CMD_WRITE_STATUS2     0x1E
#define  CMD_WRITE_STATUS3     0x1F


#define  CMD_READ_SEC_PAGE     ((1<<6) | CMD_READPAGE)
#define  CMD_WRITE_SEC_PAGE    ((1<<6) | CMD_WRITEPAGE)
#define  CMD_ERASE_SEC_PAGE    ((1<<6) | CMD_ERASESECTOR)

#define  STARTBIT          (1)
#define  BUSYBIT           (1<<8)

#define  CMD_FLASH_RESET_ENABLE     0x66
#define  CMD_FLASH_RESET            0x99
#define  CMD_MANUAL_MODE            0x30

#if defined (__CC_ARM)
#pragma no_anon_unions
#endif

#endif
