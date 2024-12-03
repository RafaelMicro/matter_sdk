/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/**
 * @file
 *   This file includes definitions for border router FreeRTOS settings.
 *   Each flag which is defined in this file, should be guarded in ../FreeRTOSConfigCommon.h
 *   This file will only be used when FREERTOS_CONFIG_FILE_DIRECTORY will be set to point to this location, i.e
 *   /boards/rdrw612bga/freertos/br.
 */

#define configUSE_TICKLESS_IDLE 0
#define configTOTAL_HEAP_SIZE ((size_t)(159 * 1024))
#include "../FreeRTOSConfigCommon.h"

#endif /* FREERTOS_CONFIG_H */
