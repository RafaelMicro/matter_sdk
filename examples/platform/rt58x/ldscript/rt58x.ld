
/* Linker script to configure memory regions.
 * Need modifying for a specific board.
 *   FLASH.ORIGIN: starting address of flash
 *   FLASH.LENGTH: length of flash
 *   RAM.ORIGIN: starting address of RAM bank 0
 *   RAM.LENGTH: length of RAM bank 0
 */

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a)


__STACK_SIZE = DEFINED(__STACK_SIZE) ? __STACK_SIZE : 2K;
__CACHE_SIZE = DEFINED(__CACHE_SIZE) ? __CACHE_SIZE : 4K;

MEMORY
{
	/* LMA regions MA_BOOT	(rx) : ORIGIN = 0x00000000, LENGTH = 32K*/
	LMA_BOOT	(rx) : ORIGIN = 0x00000000, LENGTH = 32K
	LMA_CORE	(rx) : ORIGIN = 0x00008000, LENGTH = 928K
	LMA_USER	(rx) : ORIGIN = 0x001F0000, LENGTH = 32K
	LMA_MP		(rx) : ORIGIN = 0x001F0800, LENGTH = 32K

	/* VMA regions */
	VMA_DATA	(rwx) : ORIGIN = 0x20000000, LENGTH = 144K - __STACK_SIZE
	VMA_STACK	(rwx) : ORIGIN = ORIGIN(VMA_DATA) + LENGTH(VMA_DATA), LENGTH = __STACK_SIZE
}

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 */
ENTRY(Reset_Handler)

SECTIONS
{
	.text :
	{
		KEEP(*(.isr_vector))
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
	} > LMA_CORE


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

	__etext = .;

	.data : AT (__etext)
	{
		__data_start__ = .;
		
		*(vtable)
		*(.data*)

		__reloc_vma_start__ = ALIGN(4);
		KEEP(*(reloc_text*))
		KEEP(*(reloc_data*))
		__reloc_vma_end__ = ALIGN(4);			
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
	} > VMA_DATA

	.bss :
	{
		. = ALIGN(4);
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
		__end__ = .;
		end = __end__;		
	} > VMA_DATA

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


	/* .stack_dummy section doesn't contains any symbols. It is only
	 * used for linker to calculate size of stack sections, and assign
	 * values to stack symbols later */
	.stack_dummy (COPY):
	{
		*(.stack*)
	} > VMA_STACK

	/* Set stack top to end of RAM, and stack limit move down by
	 * size of stack_dummy section */
	__StackLimit = __StackTop - SIZEOF(.stack_dummy);
	PROVIDE(__stack = __StackTop);

	/* Check if data + heap + stack exceeds RAM limit */
	ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}