/**************************************************************************//**
 * @file    i2cm_reg.h
 * @version
 * @brief    I2C MASTER Register defined
 *
 * @copyright
 ******************************************************************************/

#ifndef ___I2CM_REG_H__
#define ___I2CM_REG_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif




typedef struct
{
    __IO  uint32_t  STATUS;
    __I   uint32_t  RD_DATA;
    __IO  uint32_t  COMMAND;
    __IO  uint32_t  INT_ENABLE;
    __IO  uint32_t  INT_STATUS;
    __IO  uint32_t  CONTROL;
    __IO  uint32_t  PRESCALE_H;
} I2CM_T;



#define I2CM_STATUS_CMDFIFO_EMPTY   0x0001
#define I2CM_STATUS_DATA_READY      0x0002

#define I2CM_STATUS_I2C_ERR         0x0004
#define I2CM_STATUS_I2C_ERR_CLEAR   0x0004

#define I2CM_STATUS_LOST_ARB        0x0008

#define I2CM_STATUS_RDFIFO_UNDFL    0x0010
#define I2CM_STATUS_CMDFIFO_OVERFL  0x0020

#define I2CM_STATUS_FIFO_FULL       0x0040
#define I2CM_STATUS_TRANSFER        0x0080
#define I2CM_STATUS_RDFIFO_OVFRFL   0x0100
#define I2CM_STATUS_CMDFIFO_UNDFL   0x0200

#define I2CM_STATUS_FIFO_CLR        0x0400

// I2C Interrupt

// control register bit definitions:
#define I2CM_CTRL_ENABLE            0x80
#define I2CM_CTRL_DISABLE           0x00

#define I2CM_CTRL_CLK_SRC_APB       0x40
#define I2CM_CTRL_CLK_SRC_EXT       0x00

#define I2CM_CTRL_CLK_DIV_MASK      0x3F

// prescale register bit definitions
#define I2CM_CLK_PRESCALE_SHIFT       6


// interrupt mask register bit definitions
#define I2CM_INT_CMDFIFO_EMPTY      0x01
#define I2CM_INT_DATA_READY         0x02
#define I2CM_INT_ERROR              0x04
#define I2CM_INT_LOST_ARB           0x08

#define I2CM_INT_COMPLETE           0x10

// definitions for I2C Engine commands
#define I2CM_CMD_NULL        0x00   // Command string NULL terminator (only when state machine is expecting a command, not data)
#define I2CM_CMD_WDAT0       0x10   // write a 0 as 1 bit of data - this can be used as a Master ACK
#define I2CM_CMD_WDAT1       0x11   // write a 1 as 1 bit of data
#define I2CM_CMD_WDAT8       0x12   // next byte in command queue is 8 bit data to write (can be word address)
#define I2CM_CMD_RDAT8       0x13   // read 8 bits of data
#define I2CM_CMD_STOP        0x14   // transmit a stop  bit
#define I2CM_CMD_STRT        0x15   // transmit a start bit
#define I2CM_CMD_VACK        0x16   // verify that device is sending an ACK
#define I2CM_CMD_VNACK       0x17   // verify that device is sending a NACK


#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif

#endif
