/**************************************************************************//**
 * @file     gcc_startup_cm3_mcu.S
 * @version  
 * @brief    
 *
 * @copyright 
 *      Purpose: Cortex-M3 device startup code for gcc
*****************************************************************************/

    .syntax unified
    .arch armv7-m

/*
// <h> Stack Configuration
//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

    .section .stack
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0x100
#endif
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop


/*
// <h> Heap Configuration
//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

    .section .heap
    .align 3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0x100
#endif
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .if    Heap_Size
    .space    Heap_Size
    .endif
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit


/* Vector Table */

    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                  /* Top of Stack                  */
    .long   Reset_Handler               /* Reset Handler                 */
    .long   NMI_Handler                 /* NMI Handler                   */
    .long   HardFault_Handler           /* Hard Fault Handler            */
    .long   MemManage_Handler           /* MPU Fault Handler             */
    .long   BusFault_Handler            /* Bus Fault Handler             */
    .long   UsageFault_Handler          /* Usage Fault Handler           */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
#ifdef USE_FREERTOS    
    .long   vPortSVCHandler            /* SVCall Handler                */    
#else
    .long   SVC_Handler                 /* SVCall Handler                */
#endif
    .long   DebugMon_Handler            /* Debug Monitor Handler         */
    .long   0                           /* Reserved                      */
    
#ifdef USE_FREERTOS  
    .long   xPortPendSVHandler          /* PendSV Handler                */    
    .long   xPortSysTickHandler         /* SysTick Handler               */
#else    
    .long   PendSV_Handler              /* PendSV Handler                */    
    .long   SysTick_Handler             /* SysTick Handler               */
#endif    

    /* External Interrupts */
    .long   gpio_handler                /* 16: GPIO    Handler         */
    .long   Timer0_Handler              /* 17: Timer 0 Handler         */
    .long   Timer1_Handler              /* 18: Timer 1 Handler         */
    .long   Wdt_Handler                 /* 19: WatchDog Handler        */
    .long   uart0_handler               /* 20: UART 0  Handler         */
    .long   i2cm_handler                /* 21: I2C Master Handler      */
    .long   flashctl_handler            /* 22: FlashCtl Handler        */
    .long   uart1_handler               /* 23: UART 1  Handler         */
    .long   rtc_handler                 /* 24: RTC     Handler         */
    .long   dma0_c0_handler             /* 25: DMA0[0] Handler         */
    .long   dma0_c1_handler             /* 26: DMA0[1] Handler         */
    .long   dma0_c2_handler             /* 27: DMA0[2] Handler         */
    .long   dma0_c3_handler             /* 28: DMA0[3] Handler         */
    .long   qspi0_handler               /* 29: QSPI0   Handler         */
    .long   I2s_Handler                 /* 30: I2S0    Handler         */
    .long   Timer3_Handler              /* 31: Timer 3 Handler         */
    .long   Timer4_Handler              /* 32: Timer 4 Handler         */
    .long   dma1_c2_handler             /* 33: DMA1[2] Handler         */
    .long   bod_handler                 /* 34: BOD     Handler         */
    .long   uart2_handler               /* 35: UART 2  Handler         */
    .long   commsubsystem_handler       /* 36: COMM SUBSYSTEM COMM Handler*/
    .long   Pwm0_Handler                /* 37: PWM0    Handler         */
    .long   Pwm1_Handler                /* 38: PWM1    Handler         */
    .long   Pwm2_Handler                /* 39: PWM2    Handler         */
    .long   Pwm3_Handler                /* 40: PWM3    Handler         */
    .long   Pwm4_Handler                /* 41: PWM4    Handler         */
    .long   Sadc_Handler                /* 42: SADC    Handler         */
    .long   crypto_handler              /* 43: CRYPTO  Handler         */
    .long   Timer2_Handler              /* 44: TIMER 2 Handler         */
    .long   qspi1_handler               /* 45: QSPI1   Handler         */
    .long   Sw_Handler                  /* 46: Software Handler        */
    .long   trng_handler                /* 47: TRNG    Handler         */
    .long   Comp_Handler                /* 48: COMP    Handler         */

    .size    __isr_vector, . - __isr_vector

/* Reset Handler */
    .text
    .thumb
    .thumb_func
    .align 2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs    r3, r2
    ble    .LC1
.LC0:
    subs    r3, #4
    ldr    r0, [r1, r3]
    str    r0, [r2, r3]
    bgt    .LC0
.LC1:

#ifdef __STARTUP_CLEAR_BSS

/*     This part of work usually is done in C library startup code. Otherwise,
 *     define this macro to enable it in this startup.
 *
 *     Loop to zero out BSS section, which uses following symbols
 *     in linker script:
 *      __bss_start__: start of BSS section. Must align to 4
 *      __bss_end__: end of BSS section. Must align to 4
 */
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__

    movs    r0, 0
.LC2:
    cmp     r1, r2
    itt    lt
    strlt   r0, [r1], #4
    blt    .LC2
#endif /* __STARTUP_CLEAR_BSS */

#ifndef __NO_SYSTEM_INIT
    /* bl    SystemInit */
    ldr     r0,=SystemInit
    blx     r0
#endif

    bl    _start
    
    .pool
    .size Reset_Handler, . - Reset_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .size    \handler_name, . - \handler_name
    .endm

/* System Exception Handlers */

    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    MemManage_Handler
    def_default_handler    BusFault_Handler
    def_default_handler    UsageFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    DebugMon_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler

/* IRQ Handlers */

    def_default_handler    gpio_handler
    def_default_handler    Timer0_Handler
    def_default_handler    Timer1_Handler
    def_default_handler    Wdt_Handler
    def_default_handler    uart0_handler
    def_default_handler    i2cm_handler
    def_default_handler    flashctl_handler
    def_default_handler    uart1_handler
    def_default_handler    rtc_handler
    def_default_handler    dma0_c0_handler
    def_default_handler    dma0_c1_handler
    def_default_handler    dma0_c2_handler
    def_default_handler    dma0_c3_handler
    def_default_handler    qspi0_handler
    def_default_handler    I2s_Handler
    def_default_handler    Timer3_Handler
    def_default_handler    Timer4_Handler
    def_default_handler    dma1_c2_handler
    def_default_handler    bod_handler
    def_default_handler    uart2_handler
    def_default_handler    commsubsystem_handler
    def_default_handler    Pwm0_Handler
    def_default_handler    Pwm1_Handler
    def_default_handler    Pwm2_Handler
    def_default_handler    Pwm3_Handler
    def_default_handler    Pwm4_Handler
    def_default_handler    Sadc_Handler
    def_default_handler    crypto_handler
    def_default_handler    Timer2_Handler
    def_default_handler    qspi1_handler
    def_default_handler    Sw_Handler
    def_default_handler    trng_handler
    def_default_handler    Comp_Handler
    
    /*
    def_default_handler    Default_Handler
    .weak    DEF_IRQHandler
    .set    DEF_IRQHandler, Default_Handler
    */
    .end
