/*
** ###################################################################
**     Processors:          RW610EVA0IK
**                          RW610EVA0IMP
**                          RW610HNA0IK
**                          RW610HNA0IMP
**                          RW610UKA0IZ
**
**     Compiler:            GNU C Compiler
**     Reference manual:    MIMXRW685 User manual Rev. 0.95 11 November 2019
**     Version:             rev. 1.0, 2021-03-16
**     Build:               b211216
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2023 NXP
**     All rights reserved.
**
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x0A00;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0A00;

m_flash_start                       = 0x08000000;
m_flash_size                        = 0x04000000;
m_flash_end                         = m_flash_start + m_flash_size - 1;
m_sector_size                       = 0x1000;

wifi_fw_size                        = 0xA0000;
ble_fw_size                         = 0x50000;
z154_fw_size                        = 0x50000;
fw_storage_start                    = 0x08400000;
fw_storage_size                     = (wifi_fw_size + ble_fw_size + z154_fw_size) * 2;
fw_storage_end                      = fw_storage_start + fw_storage_size -1;

NV_STORAGE_MAX_SECTORS              = DEFINED(gNVMSectorCountLink_d) ? gNVMSectorCountLink_d : 16;
NV_STORAGE_SECTOR_SIZE              = m_sector_size;
NV_STORAGE_END_ADDRESS              = m_flash_end;
m_nvm_storage_size                  = NV_STORAGE_MAX_SECTORS * NV_STORAGE_SECTOR_SIZE;
NV_STORAGE_START_ADDRESS            = NV_STORAGE_END_ADDRESS - m_nvm_storage_size + 1;
NV_STORAGE_START_ADDRESS_OFFSET     = NV_STORAGE_START_ADDRESS - m_flash_start;

m_text_start                        = 0x08001280;
m_text_end                          = fw_storage_start - 1;
m_text_size                         = m_text_end - m_text_start + 1;

/* Specify the memory areas */
MEMORY
{
  m_flash_config        (RX)  : ORIGIN = 0x08000400, LENGTH = 0x00000C00
  m_interrupts          (RX)  : ORIGIN = 0x08001000, LENGTH = 0x00000280
  m_text                (RX)  : ORIGIN = m_text_start, LENGTH = m_text_size
  m_nvm_region          (RX)  : ORIGIN = NV_STORAGE_START_ADDRESS, LENGTH = m_nvm_storage_size
  m_data                (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00130000
  m_sqram_cpu13_mbox    (RW)  : ORIGIN = 0x41380000, LENGTH = 0x00000488
  m_sqram_cpu13_txq     (RW)  : ORIGIN = 0x41380488, LENGTH = 0x00001000
  m_sqram_cpu31_txq     (RW)  : ORIGIN = 0x41380488, LENGTH = 0x00001000
  m_sqram_cpu23_mbox    (RW)  : ORIGIN = 0x443C0000, LENGTH = 0x00000488
  m_sqram_cpu23_txq     (RW)  : ORIGIN = 0x443C0488, LENGTH = 0x00001080
  m_sqram_cpu32_txq     (RW)  : ORIGIN = 0x443C1508, LENGTH = 0x00001080
}

/* Define output sections */
SECTIONS
{
  .flash_config :
  {
    . = ALIGN(4);
    __FLASH_BASE = .;
    KEEP(* (.flash_conf))     /* flash config section */
    . = ALIGN(4);
  } > m_flash_config

  /* The startup code goes first into internal ram */
  .interrupts :
  {
    . = ALIGN(4);
    __VECTOR_TABLE = .;
    __Vectors = .;
    KEEP(*(.isr_vector))     /* Startup code */
    FILL(0x00)
    . = 0x280;
  } > m_interrupts

  /* The program code and other data goes into internal ram */
  .text :
  {
    . = ALIGN(4);
    EXCLUDE_FILE(
        /* Exclude flash and frequently executed functions from XIP */
        *fsl_adapter_flexspi_nor_flash.c.obj
        *mflash_drv.c.obj
        *fsl_flexspi.c.obj
        *port.c.obj
        *portasm.c.obj
        *queue.c.obj
        *timers.c.obj
        *tasks.c.obj
        *inet_chksum.c.obj
        *ip6.c.obj
        *pbuf.c.obj
        *sys_arch.c.obj
        *tcp_out.c.obj
        *tcp_in.c.obj
        *fsl_adapter_rfimu.c.obj
        *fsl_cache.c.obj
        *fsl_os_abstraction_free_rtos.c.obj
    )
    *(.text* .rodata*)
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_text

    .NVM_TABLE :
  {
    . = ALIGN(4);
    PROVIDE(__start_NVM_TABLE = .);
    KEEP(*(.NVM_TABLE));
    PROVIDE(__stop_NVM_TABLE = .);
    . = ALIGN(4);
  } > m_text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text

 .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  __VECTOR_RAM = ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = 0x0;

  .data : AT(__DATA_ROM)
  {
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    . = ALIGN(4);
    *(CodeQuickAccess)       /* CodeQuickAccess sections */
    *(DataQuickAccess)       /* DataQuickAccess sections */
    /* Explicit placement of flash and frequently executed functions in RAM */
    *fsl_adapter_flexspi_nor_flash.c.obj(.text* .rodata*)
    *mflash_drv.c.obj(.text* .rodata*)
    *fsl_flexspi.c.obj(.text* .rodata*)
    *port.c.obj(.text* .rodata*)
    *portasm.c.obj(.text* .rodata*)
    *queue.c.obj(.text* .rodata*)
    *timers.c.obj(.text* .rodata*)
    *tasks.c.obj(.text* .rodata*)
    *inet_chksum.c.obj(.text* .rodata*)
    *ip6.c.obj(.text* .rodata*)
    *pbuf.c.obj(.text* .rodata*)
    *sys_arch.c.obj(.text* .rodata*)
    *tcp_out.c.obj(.text* .rodata*)
    *tcp_in.c.obj(.text* .rodata*)
    *fsl_adapter_rfimu.c.obj(.text* .rodata*)
    *fsl_cache.c.obj(.text* .rodata*)
    *fsl_os_abstraction_free_rtos.c.obj(.text* .rodata*)
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")
  _image_size = __DATA_END - __VECTOR_TABLE;

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
    __end__ = .;
  } > m_data

  .stack :
  {
    . = ALIGN(8);
    __StackLimit = .;
    . += STACK_SIZE;
    __StackTop = .;
  } > m_data

  PROVIDE(__stack = __StackTop);

  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
    *(.heap)
  } > m_data

  .smu_cpu23_mbox (NOLOAD) :
  {
    . = ALIGN(4);
    SQRAM_CPU23_MBOX = .;
    *(.smu_cpu23_mbox)
    KEEP (*(.smu_cpu23_mbox))
    . = ALIGN(4);
  } > m_sqram_cpu23_mbox

  .smu_cpu32_txq (NOLOAD) :
  {
    . = ALIGN(4);
    *(.smu_cpu32_txq)
    KEEP (*(.smu_cpu32_txq))
    . = ALIGN(4);
  } > m_sqram_cpu32_txq

  .smu_cpu13_mbox (NOLOAD) :
  {
    . = ALIGN(4);
    SQRAM_CPU13_MBOX = .;
    *(.smu_cpu13_mbox)
    KEEP (*(.smu_cpu13_mbox))
    . = ALIGN(4);
  } > m_sqram_cpu13_mbox

  .smu_cpu31_txq (NOLOAD) :
  {
    . = ALIGN(4);
    *(.smu_cpu31_txq)
    KEEP (*(.smu_cpu31_txq))
    . = ALIGN(4);
  } > m_sqram_cpu31_txq

  .ARM.attributes 0 : { *(.ARM.attributes) }
}

