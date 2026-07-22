# Rafael Micro — Matter SoC Flash Memory Layout Guide

## Overview

This document describes the internal flash memory partitioning for Rafael Micro's Matter-enabled SoC products: **RT583**, **RT584H/L**, and **RT584HA4**.

Each chip divides its flash into fixed regions: Bootloader, Application, OTA Bank, Matter Data, and MP Sector. The RT584HA4 additionally includes a dedicated **Zigbee Data** region.

---

## Partition Summary

| Region | Description |
|---|---|
| **Bootloader** | Power-on initialization, firmware integrity check, and OTA image swap logic |
| **Application** | Main firmware image running on the device |
| **OTA Bank** | Staging area that holds a new firmware image received over-the-air before it is applied |
| **Zigbee Data** | Non-volatile storage for Zigbee network configuration and credentials *(RT584HA4 only)* |
| **Matter Data** | Non-volatile storage for Matter commissioning data, credentials, and protocol configuration |
| **MP Sector** | Manufacturing/production sector; stores DAC, BLE, calibration, and device-specific parameters |

---

## RT583 — 2 MB Flash

**Base Address:** `0x00000000`

| Region | Start | End | Size |
|---|---|---|---|
| Bootloader | `0x00000000` | `0x00008000` | 32 KB |
| Application | `0x00008000` | `0x0012F000` | 1,180 KB |
| OTA Bank | `0x0012F000` | `0x001E0000` | 708 KB |
| Matter Data | `0x001E0000` | `0x001F0000` | 64 KB |
| MP Sector | `0x001F0000` | `0x00200000` | 64 KB |

```
 RT583 — 2 MB Flash
 ┌────────────────────────┐ 0x00000000
 │                        │
 │       Bootloader       │  32 KB
 │                        │
 ├────────────────────────┤ 0x00008000
 │                        │
 │                        │
 │                        │
 │                        │
 │                        │
 │      Application       │  1,180 KB
 │                        │
 │                        │
 │                        │
 │                        │
 │                        │
 ├────────────────────────┤ 0x0012F000
 │                        │
 │                        │
 │        OTA Bank        │  708 KB
 │                        │
 │                        │
 │                        │
 ├────────────────────────┤ 0x001E0000
 │      Matter Data       │  64 KB
 ├────────────────────────┤ 0x001F0000
 │       MP Sector        │  64 KB
 └────────────────────────┘ 0x00200000
```

---

## RT584H/L — 2 MB Flash

**Base Address:** `0x10000000`

| Region | Start | End | Size |
|---|---|---|---|
| Bootloader | `0x10000000` | `0x10010000` | 64 KB |
| Application | `0x10010000` | `0x10132000` | 1,160 KB |
| OTA Bank | `0x10132000` | `0x101E0000` | 696 KB |
| Matter Data | `0x101E0000` | `0x101F0000` | 64 KB |
| MP Sector | `0x101F0000` | `0x10200000` | 64 KB |

```
 RT584H/L — 2 MB Flash
 ┌────────────────────────┐ 0x10000000
 │                        │
 │       Bootloader       │  64 KB
 │                        │
 ├────────────────────────┤ 0x10010000
 │                        │
 │                        │
 │                        │
 │                        │
 │                        │
 │      Application       │  1,160 KB
 │                        │
 │                        │
 │                        │
 │                        │
 │                        │
 ├────────────────────────┤ 0x10132000
 │                        │
 │                        │
 │        OTA Bank        │  696 KB
 │                        │
 │                        │
 │                        │
 ├────────────────────────┤ 0x101E0000
 │      Matter Data       │  64 KB
 ├────────────────────────┤ 0x101F0000
 │       MP Sector        │  64 KB
 └────────────────────────┘ 0x10200000
```

---

## RT584HA4 — 4 MB Flash

**Base Address:** `0x10000000`

| Region | Start | End | Size |
|---|---|---|---|
| Bootloader | `0x10000000` | `0x10010000` | 64 KB |
| Application | `0x10010000` | `0x10200000` | 1,984 KB |
| OTA Bank | `0x10200000` | `0x103D0000` | 1,856 KB |
| Zigbee Data | `0x103D0000` | `0x103E0000` | 64 KB |
| Matter Data | `0x103E0000` | `0x103F0000` | 64 KB |
| MP Sector | `0x103F0000` | `0x10400000` | 64 KB |

```
 RT584HA4 — 4 MB Flash
 ┌────────────────────────┐ 0x10000000
 │       Bootloader       │  64 KB
 ├────────────────────────┤ 0x10010000
 │                        │
 │                        │
 │                        │
 │                        │
 │                        │
 │                        │
 │      Application       │  1,984 KB (~1.94 MB)
 │                        │
 │                        │
 │                        │
 │                        │
 │                        │
 │                        │
 ├────────────────────────┤ 0x10200000
 │                        │
 │                        │
 │                        │
 │                        │
 │                        │
 │        OTA Bank        │  1,856 KB (1.8125 MB)
 │                        │
 │                        │
 │                        │
 │                        │
 │                        │
 ├────────────────────────┤ 0x103D0000
 │      Zigbee Data       │  64 KB
 ├────────────────────────┤ 0x103E0000
 │      Matter Data       │  64 KB
 ├────────────────────────┤ 0x103F0000
 │       MP Sector        │  64 KB
 └────────────────────────┘ 0x10400000
```

---

## Comparison at a Glance

```
         RT583 (2 MB)          RT584H/L (2 MB)        RT584HA4 (4 MB)
        Base: 0x00000000       Base: 0x10000000        Base: 0x10000000

 ┌──────────────────────┐  ┌──────────────────────┐  ┌──────────────────────┐
 │   Bootloader  32 KB  │  │   Bootloader  64 KB  │  │   Bootloader  64 KB  │
 ├──────────────────────┤  ├──────────────────────┤  ├──────────────────────┤
 │                      │  │                      │  │                      │
 │                      │  │                      │  │                      │
 │  Application         │  │  Application         │  │  Application         │
 │  1,180 KB            │  │  1,160 KB            │  │  1,984 KB            │
 │                      │  │                      │  │                      │
 │                      │  │                      │  │                      │
 ├──────────────────────┤  ├──────────────────────┤  │                      │
 │                      │  │                      │  │                      │
 │  OTA Bank            │  │  OTA Bank            │  │                      │
 │  708 KB              │  │  696 KB              │  ├──────────────────────┤
 │                      │  │                      │  │                      │
 │                      │  │                      │  │  OTA Bank            │
 ├──────────────────────┤  ├──────────────────────┤  │  1,856 KB            │
 │  Matter Data  64 KB  │  │  Matter Data  64 KB  │  │                      │
 ├──────────────────────┤  ├──────────────────────┤  ├──────────────────────┤
 │  MP Sector    64 KB  │  │  MP Sector    64 KB  │  │  Zigbee Data  64 KB  │
 └──────────────────────┘  └──────────────────────┘  ├──────────────────────┤
                                                     │  Matter Data  64 KB  │
                                                     ├──────────────────────┤
                                                     │  MP Sector    64 KB  │
                                                     └──────────────────────┘
```

---

## MP Sector Detail (64 KB, All Variants)

The MP Sector is 64 KB on all supported chips and is further subdivided as follows:

| Sub-region | Offset | Size | Description |
|---|---|---|---|
| Reserved (Customer) | `0x000` ~ `0x4000` | 16 KB | Available for customer-defined data |
| Matter DAC | `0x4000` ~ `0xC000` | 32 KB | Matter Device Attestation Certificate (DAC)|
| BLE Data | `0xC000` ~ `0xE000` | 8 KB | Bluetooth LE configuration data |
| Calibration Data | `0xE000` ~ `0xF000` | 4 KB | RF / hardware calibration parameters |
| Information Data | `0xF000` ~ `0x10000` | 4 KB | Device information |

```
 MP Sector — 64 KB (offset from MP Sector base)
 ┌────────────────────────┐ 0x0000
 │                        │
 │  Reserved (Customer)   │  16 KB
 │                        │
 │                        │
 ├────────────────────────┤ 0x4000
 │                        │
 │                        │
 │      Matter DAC        │  32 KB
 │                        │
 │                        │
 │                        │
 │                        │
 ├────────────────────────┤ 0xC000
 │       BLE Data         │  8 KB
 │                        │
 ├────────────────────────┤ 0xE000
 │   Calibration Data     │  4 KB
 ├────────────────────────┤ 0xF000
 │   Information Data     │  4 KB
 └────────────────────────┘ 0x10000
```

---

## Notes

1. **Zigbee Data** is present only on the RT584HA4; it occupies 64 KB between the OTA Bank and Matter Data regions.
2. Partition boundaries are fixed in the bootloader and must not be changed without a corresponding bootloader rebuild.
3. The **Reserved (Customer)** sub-region inside MP Sector (first 16 KB) is available for customer-specific factory data.
