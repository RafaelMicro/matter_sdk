;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/
;// ----------------------------------------------
;// Purpose: Cortex-M3 device startup code for Arm 
;//           toolchains
;// ----------------------------------------------

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000800

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000800

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                IF :DEF:USE_FREERTOS
                IMPORT xPortPendSVHandler
                IMPORT xPortSysTickHandler
                IMPORT vPortSVCHandler
                ENDIF

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                IF :DEF:USE_FREERTOS
                DCD     vPortSVCHandler           ; SVCall Handler
                ELSE
                DCD     SVC_Handler               ; SVCall Handler
                ENDIF
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                IF :DEF:USE_FREERTOS
                DCD     xPortPendSVHandler        ; PendSV Handler
                DCD     xPortSysTickHandler       ; SysTick Handler
                ELSE
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler
                ENDIF

                ;External Interrupts
                DCD     gpio_handler              ; GPIO Handler
                DCD     Timer0_Handler            ; Timer 0 Handler
                DCD     Timer1_Handler            ; Timer 1 Handler
                DCD     Wdt_Handler               ; WatchDog handler
                DCD     uart0_handler             ; UART 0 Handler
                DCD     i2cm_handler              ; I2C Master Handler
                DCD     flashctl_handler          ; FlashCtl Handler
                DCD     uart1_handler             ; UART 1 Handler
                DCD     rtc_handler               ; Rtc Handler
                DCD     dma0_c0_handler           ; DMA0[0] Handler
                DCD     dma0_c1_handler           ; DMA0[1] Hanlder
                DCD     dma0_c2_handler           ; DMA0[2] Handler
                DCD     dma0_c3_handler           ; DMA0[3] Handler
                DCD     qspi0_handler             ; QSPI0 Handler
                DCD     I2s_Handler               ; I2S Handler
                DCD     Timer3_Handler            ; Timer 3 Handler
                DCD     Timer4_Handler            ; Timer 4 Handler
                DCD     dma1_c2_handler           ; DMA1[2] Handler
                DCD     bod_handler               ; BOD Handler
                DCD     uart2_handler             ; UART 2 Handler
                DCD     commsubsystem_handler     ; CommSubsystem Handler
                DCD     Pwm0_Handler              ; PWM0 Handler
                DCD     Pwm1_Handler              ; PWM1 Handler
                DCD     Pwm2_Handler              ; PWM2 Handler
                DCD     Pwm3_Handler              ; PWM3 Handler
                DCD     Pwm4_Handler              ; PWM4 Handler
                DCD     Sadc_Handler              ; SADC Handler
                DCD     crypto_handler            ; Crypto Handler
                DCD     Timer2_Handler            ; TIMER2 Handler
                DCD     qspi1_handler             ; QSPI1 Handler
                DCD     Sw_Handler                ; Software Handler
                DCD     trng_handler              ; TRNG Handler
                DCD     Comp_Handler              ; COMPARATOR Handler
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler\
                PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler\
                PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT gpio_handler               [WEAK]
                EXPORT Timer0_Handler             [WEAK]
                EXPORT Timer1_Handler             [WEAK]
                EXPORT Wdt_Handler                [WEAK]
                EXPORT uart0_handler              [WEAK]
                EXPORT i2cm_handler               [WEAK]
                EXPORT flashctl_handler           [WEAK]
                EXPORT uart1_handler              [WEAK]
                EXPORT rtc_handler                [WEAK]
                EXPORT dma0_c0_handler            [WEAK]
                EXPORT dma0_c1_handler            [WEAK]
                EXPORT dma0_c2_handler            [WEAK]
                EXPORT dma0_c3_handler            [WEAK]
                EXPORT qspi0_handler              [WEAK]
                EXPORT I2s_Handler                [WEAK]
                EXPORT Timer3_Handler             [WEAK]
                EXPORT Timer4_Handler             [WEAK]
                EXPORT dma1_c2_handler            [WEAK]
                EXPORT bod_handler                [WEAK]
                EXPORT uart2_handler              [WEAK]
                EXPORT commsubsystem_handler      [WEAK]
                EXPORT Pwm0_Handler               [WEAK]
                EXPORT Pwm1_Handler               [WEAK]
                EXPORT Pwm2_Handler               [WEAK]
                EXPORT Pwm3_Handler               [WEAK]
                EXPORT Pwm4_Handler               [WEAK]
                EXPORT Sadc_Handler               [WEAK]
                EXPORT crypto_handler             [WEAK]
                EXPORT Timer2_Handler             [WEAK]
                EXPORT qspi1_handler              [WEAK]
                EXPORT Sw_Handler                 [WEAK]
                EXPORT trng_handler               [WEAK]
                EXPORT Comp_Handler               [WEAK]
					
gpio_handler
Timer0_Handler
Timer1_Handler
Wdt_Handler
uart0_handler
i2cm_handler
flashctl_handler
uart1_handler
rtc_handler
dma0_c0_handler
dma0_c1_handler
dma0_c2_handler
dma0_c3_handler
qspi0_handler
I2s_Handler
Timer3_Handler
Timer4_Handler
dma1_c2_handler
bod_handler
uart2_handler
commsubsystem_handler
Pwm0_Handler
Pwm1_Handler
Pwm2_Handler
Pwm3_Handler
Pwm4_Handler
Sadc_Handler
crypto_handler
Timer2_Handler
qspi1_handler
Sw_Handler
trng_handler
Comp_Handler
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
