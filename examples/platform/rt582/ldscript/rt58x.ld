
/* Linker script to configure memory regions.
 * Need modifying for a specific board.
 *   FLASH.ORIGIN: starting address of flash
 *   FLASH.LENGTH: length of flash
 *   RAM.ORIGIN: starting address of RAM bank 0
 *   RAM.LENGTH: length of RAM bank 0
 */

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a)

__STACK_SIZE = DEFINED(__STACK_SIZE) ? __STACK_SIZE : 0x200;
__HEAP_SIZE = DEFINED(__HEAP_SIZE) ? __HEAP_SIZE : 32K;

MEMORY
{
	/* LMA regions */
	LMA_BOOT	(rx) : ORIGIN = 0x00000000, LENGTH = 32K
	LMA_CORE	(rx) : ORIGIN = 0x00008000, LENGTH = 928K
	LMA_USER	(rx) : ORIGIN = 0x000F0000, LENGTH = 32K
	LMA_MP		(rx) : ORIGIN = 0x000F0800, LENGTH = 32K

	/* VMA regions */
	VMA_DATA	(rwx) : ORIGIN = 0x20000000, LENGTH = 144K - __HEAP_SIZE - __STACK_SIZE
	VMA_HEAP	(rwx) : ORIGIN = 0x20024000 - __STACK_SIZE - __HEAP_SIZE, LENGTH = __HEAP_SIZE
	VMA_STACK	(rwx) : ORIGIN = 0x20024000 - __STACK_SIZE, LENGTH = __STACK_SIZE
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
	} > VMA_DATA

	.heap (COPY) :
	{
		__end__ = .;
		end = __end__;
		KEEP(*(reloc_heap*))
		__heap_end__ = .;
	} > VMA_HEAP

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
