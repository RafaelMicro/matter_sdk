/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a)


__STACK_SIZE =  0xC00;   /* 3 KB */
__RAM_SIZE   = 192K;
__DATA_SIZE  = __RAM_SIZE - __STACK_SIZE - 8;

MEMORY
{
    LMA_BOOT (rx)    : ORIGIN = 0x10000000, LENGTH = 64K
    LMA_CORE (rx)    : ORIGIN = 0x10010000, LENGTH = 1984K
    LMA_USER (r)     : ORIGIN = 0x103F0000, LENGTH = 16K
    LMA_MPSC (r)     : ORIGIN = 0x103FE000, LENGTH = 8K
	/* VMA (RAM) */
    VMA_DATA  (rwx) : ORIGIN = 0x30000000, LENGTH = __DATA_SIZE
    VMA_STACK (rwx) : ORIGIN = ORIGIN(VMA_DATA) + LENGTH(VMA_DATA), LENGTH = __STACK_SIZE + 8
}

/* 定義程式區段（PHDRS）屬性 */
PHDRS
{
    rx PT_LOAD FLAGS(5);   /* Read + Execute */
    rw PT_LOAD FLAGS(6);   /* Read + Write */
}

/* Entry point. */
ENTRY(Reset_Handler)

SECTIONS
{    
    /* Vector Table Section. */   
    .text :
    {
        KEEP(*(.vectors))
        KEEP(*(.vector_tail))
        __Vectors_End = .;
        __Vectors_Size = __Vectors_End - __Vectors;        
        __end__ = .;        
        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))


        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
         *crtbegin.o(.dtors)
         *crtbegin?.o(.dtors)
         *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
         *(SORT(.dtors.*))
         *(.dtors)

        *(.rodata*)

        KEEP(*(.eh_frame*))
    } > LMA_CORE :rx
	
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > LMA_CORE

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > LMA_CORE
    __exidx_end = .;

	.cli_cmd_pool :
	{
		__cli_cmd_pool_start__ = ALIGN(4);
		KEEP(*(cli_cmd_pool*))
		__cli_cmd_pool_end__ = ALIGN(4);
		/* static log code1 */
		. = ALIGN(4);
		_rf_static_logcomponent_code_start = .;
		KEEP(SORT(*)(.static_logcomponent_code*))
		*(.static_logcomponent_code*)
		_rf_static_logcomponent_code_end = .;

		/* static log code2 */
		. = ALIGN(4);
		_rf_static_logfile_code_start = .;
		KEEP(SORT(*)(.static_logfile_code*))
		*(.static_logfile_code*)
		_rf_static_logfile_code_end = .;

		/* static log code3 */
		. = ALIGN(4);
		_rf_static_logpri_code_start = .;
		KEEP(SORT(*)(.static_logpri_code*))
		*(.static_logpri_code*)
		_rf_static_logpri_code_end = .;

	} > LMA_CORE

    /* To copy multiple ROM to RAM sections,
     * define etext2/data2_start/data2_end and
     * define __STARTUP_COPY_MULTIPLE in startup_cmsdk_mps2_an519_bl2.S */
    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (__data_start__)
        LONG ((__data_end__ - __data_start__) / 4)
        LONG (DEFINED(__etext2) ? __etext2 : 0)
        LONG (DEFINED(__data2_start__) ? __data2_start__ : 0)
        LONG (DEFINED(__data2_start__) ? ((__data2_end__ - __data2_start__) / 4) : 0)
        __copy_table_end__ = .;
    } > LMA_CORE

    /* To clear multiple BSS sections,
     * uncomment .zero.table section and,
     * define __STARTUP_CLEAR_BSS_MULTIPLE in startup_cmsdk_mps2_an519_bl2.S */
    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG ((__bss_end__ - __bss_start__) / 4)
        LONG (DEFINED(__bss2_start__) ? __bss2_start__ : 0)
        LONG (DEFINED(__bss2_start__) ? ((__bss2_end__ - __bss2_start__) / 4) : 0)
        __zero_table_end__ = .;
    } > LMA_CORE :rx
    
    __etext = ALIGN(4);
    
    /* Place TrustZone Secure Gateway Stubs Section in SG_veneer_table'
     * (Non-Secure Callable) memory. */
    .gnu.sgstubs : ALIGN (32)
    {
        . = ALIGN(32);
        _start_sg = .;
        *(.gnu.sgstubs*)
        . = ALIGN(32);
        _end_sg = .;
    } > LMA_CORE

    
    .data : AT (__etext)
    {
        __data_start__ = .;
        *(vtable)
        *(.data*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);


        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;

    } > VMA_DATA :rw

    Image$$ER_DATA$$Base = ADDR(.data);

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > VMA_DATA  :rw
    PROVIDE(end = .);

    bss_size = __bss_end__ - __bss_start__;

	/* first Heap region： RAM end */
	. = ALIGN(8);
	PROVIDE(_heap_start = .);
	. = ORIGIN(VMA_DATA) + LENGTH(VMA_DATA);
	PROVIDE(_heap_end = .);
	PROVIDE(_heap_size = _heap_end - _heap_start);

	/* seconde Heap region */
	PROVIDE(_heap2_start = 0);
	PROVIDE(_heap2_end   = 0);
	PROVIDE(_heap2_size  = 0);

	/* log */
	PROVIDE( _ld_rf_static_logcomponent_code_start = _rf_static_logcomponent_code_start );
	PROVIDE( _ld_rf_static_logcomponent_code_end   = _rf_static_logcomponent_code_end );
	PROVIDE( _ld_rf_static_logfile_code_start = _rf_static_logfile_code_start );
	PROVIDE( _ld_rf_static_logfile_code_end   = _rf_static_logfile_code_end );
	PROVIDE( _ld_rf_static_logpri_code_start = _rf_static_logpri_code_start );
	PROVIDE( _ld_rf_static_logpri_code_end   = _rf_static_logpri_code_end );

    .stack (NOLOAD) : ALIGN(8)
    {
        __StackLimit = .;
        . = . + __STACK_SIZE;
        __StackTop = .;
    } > VMA_STACK

    Image$$ARM_LIB_STACK$$ZI$$Base = ADDR(.stack);
    Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.stack) + SIZEOF(.stack);

    .stack_seal_res (NOLOAD) : ALIGN(8)
    {
        __StackSeal = .;
        . = . + 8;
    } > VMA_STACK
    PROVIDE(__stack_top = ORIGIN(VMA_STACK) + LENGTH(VMA_STACK));
    PROVIDE(__stack = __StackTop);
    PROVIDE(Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.stack) + SIZEOF(.stack));
    
    /* Provide basic symbols giving location and size of main text block,
     * including initial values of RW data sections. Note that these will need
     * extending to give a complete picture with complex images
     * (e.g multiple Flash banks). */
    _image_start    = LOADADDR(.text);
    _image_end      = LOADADDR(.data) + SIZEOF(.data);
    _image_size     = _image_end - _image_start;
}
