# Rafael Doorlock UART command manual

- [Introduction](#Introduction)
- [GPIO Assignments](#GPIO-Assignments)
- [Control the Doorlock ](#Control-the-Doorlock)
- [Command Structure](#Command-Structure)
- [Commands](#Commands)

## Introduction

Rafael's doorlock example provide UART interface to control the doorlock module.
This document provides an example UART command format and some simple commands to control the doorlock module.

## GPIO Assignments

* GPIO 0:  Hold for 6s to factory reset the device and enter pairing mode.
* GPIO 1:  Press to toggle the lock state.
* GPIO 7:  Grounded to wake up device.
* GPIO 21:  Indicate lock state, on if unlocked.
* GPIO 16(Rx)/17(Tx): UART 0, used to print debug message.
* GPIO 28(Tx)/29(Rx): UART 1, used to communicate with device.

## Control the Doorlock 

To communicate with doorlock using UART, the doorlock must wakeup first.

If the doorlock is build as sleepy end device, GPIO 7 must be grounded before and during the communication.

If the device is build ad router, there's no need to do anything.

The wake-up pin and UART configuration are defined in UartTask.c

## Command Structure

The uart command using following structure in little-endian format 

| Header | Length | Command ID | Payload | Checksum |
|:-:|:-:|:-:|:-:|:-:|
| 4 octets | 1 octets | 2 octets | N octets | 1 octets |

* **Header**: The command header is 4 bytes long and should be formatted as 0xFF 0xFC 0xFC 0xFF.
* **Length**: The length field is the length sum of command and payload field.
* **Command ID**: Command id is 4 bytes long which defined in the following description. 
* **Payload**: The payload is variable bytes long used by command, following section has more information. 
* **Checksum**: The checksum is 1-byte long and to confirm the received data correctly. Its value is bitwise not(~) of the sum of all command data fields but header field excluded. Checksum value = ~(length[0]+command id[0]+ command id[1]+payload[0]…+payload[n]).

## Commands

- [Lock Command](#Lock-Command)
- [Unlock Command](#Unlock-Command)
- [Lock Status Report](#Lock-Status-Report)

    ### Lock Command
    * The command is used to lock the doorlock.
    * Command ID: 0x0001
    * Direction: MCU->RT58x
    * Payload: This command has no payload
    * Example:

    | Header | Length | Command ID | Payload | Checksum |
    |:-:|:-:|:-:|:-:|:-:|
    | FF FC FC FF | 02 | 01 00 | - | FC |


    ### Unlock Command
    * The command is used to unlock the doorlock.
    * Command ID: 0x0002
    * Direction: MCU->RT58x
    * Payload: This command has no payload
    * Example:

    | Header | Length | Command ID | Payload | Checksum |
    |:-:|:-:|:-:|:-:|:-:|
    | FF FC FC FF | 02 | 02 00 | - | FB |


    ### Lock Status Report
    * The command is sent by doorlock module when lockstate changed.
    * Command ID: 0x8001
    * Direction: RT58x->MCU
    * Payload:     

        | Lock Status |
        |:-:|
        | 1 octets |

        Lock status: Indicates doorlock state.

        0: doorlock is unlocked.

        1: doorlock is locked.
    * Example:

    | Header | Length | Command ID | Payload | Checksum | Description |
    |:-:|:-:|:-:|:-:|:-:|:-:|
    | FF FC FC FF | 03 | 01 80 | 00 | 7B | Doorlock is Locked |
    | FF FC FC FF | 03 | 01 80 | 01 | 7A | Doorlock is Unlocked |
