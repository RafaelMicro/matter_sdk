/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MULTICORE_H_
#define MULTICORE_H_

void multicore_init(void);

void multicore_send_data(uint8_t *pData, uint32_t size);

#endif /* MULTICORE_H_ */
