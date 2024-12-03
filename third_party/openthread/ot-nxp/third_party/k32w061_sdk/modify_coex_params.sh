#!/bin/bash
#
# Copyright 2023 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

if [ $# != 3 -a $# != 1 ]; then
	echo "Usage: $0 <ELF file> [<variable> <new_value>]

	Variable can be one of the following:

	gMWS_CoexPrioSignalTime_d
	gMWS_CoexConfirmWaitTime_d
	gMWS_CoexRfDenyActiveState_d

	For value you can use any 32bit value, provided as decimal

	If only the ELF file is provided, it's converted to .bin file

	"

	exit 1
fi

# sanity checks
arm-none-eabi-objcopy &> /dev/null
if [ $? == 127 ]; then
	echo "arm-none-eabi-objcopy must be in your path"
	exit 2
fi

if [ $# != 1 ]; then
	arm-none-eabi-readelf &> /dev/null
	if [ $? == 127 ]; then
		echo "arm-none-eabi-readelf must be in your path"
		exit 3
	fi
fi

if [ -f $1 ]; then
	file_type=`file $1`
	if [[ ! "$file_type" =~ "ELF 32-bit LSB executable, ARM" ]]; then
		echo "$1 is not a 32 bit ARM ELF..."
		exit 4
	fi
else
	echo "$1 is not found or not a file..."
	exit 5
fi

#end of sanity checks

function find_var_offset()
{
	echo "Searching for $2 in $1..."
	var_line=`arm-none-eabi-readelf -s -Wl $1 | grep $2 | grep "OBJECT  GLOBAL DEFAULT"`
	if [ -z "$var_line" ]; then
		echo "Couldn't find \"$2\" or not a variable"
		return 1
	fi
	vaddr_hex=`echo ${var_line} | cut -d' ' -f 2`
	vsize=`echo ${var_line} | cut -d' ' -f 3`

	return 0
}

function find_var_section()
{
	echo "Searching for section for 0x$2 in $1..."
	IFS=$'\n'; sections=($(arm-none-eabi-readelf -S $1 | grep -E "\[( )?[0-9]*\]"))
	for section in "${sections[@]}"; do

		section=`echo $section | tr -d \[\]`
		section=`echo $section | tr -s [:space:] ' '`

		#echo $section

		section_name=`echo $section | cut -d' ' -f3`
		section_start_addr=0x`echo $section | cut -d' ' -f5`
		section_file_off=0x`echo $section | cut -d' ' -f6`
		section_size=0x`echo $section | cut -d' ' -f7`

		# do the end of the section
		section_end_addr=`printf "0x%X\n" $(( $section_start_addr + $section_size ))`
		
		# what's the size of the var?
		var_end=`printf "0x%X\n" $(( 0x$2 + 0x$3 ))`

		# now does this fit ?
		diff=$(( $section_end_addr - $var_end ))

		if [ $diff -gt 0 ]; then
			echo "Found section ${section_name} (${section_start_addr} - ${section_end_addr}) for 0x$2"
			vfileoff=$(( 0x$2 - ${section_start_addr} + ${section_file_off} ))
			return 0
		fi
	done

	return 1
}

function modify_elf_file()
{
	echo "Copying data from start to $2..."

	dd if=$1 bs=1 count=$(( $2 )) of=$1.tmp &> /dev/null

	new_val=`printf "%08X\n" $4`
	echo "Splicing in new value 0x${new_val}"

	echo -ne \\x${new_val:6:2}\\x${new_val:4:2}\\x${new_val:2:2}\\x${new_val:0:2} | dd bs=1 count=$(( $3 )) seek=$(( $2 )) of=$1.tmp conv=notrunc &> /dev/null

	echo "Copying remaining file content"
	dd if=$1 bs=1 seek=$(( $2 + $3 )) skip=$(( $2 + $3 )) of=$1.tmp conv=notrunc &> /dev/null

	echo "Overwriting original file"
	
	mv $1.tmp $1
}

function create_bin()
{
	echo "Converting to bin file"
	arm-none-eabi-objcopy -O binary $1 $1.bin
}

if [ $# == 1 ]; then
	create_bin $1
	exit 0
fi

find_var_offset "$1" "$2"
if [ $? != 0 ]; then
	exit 1
fi

find_var_section "$1" "$vaddr_hex" "$vsize"
if [ $? != 0 ]; then
	exit 2
fi

modify_elf_file "$1" "$vfileoff" "$vsize" "$3"

create_bin $1
