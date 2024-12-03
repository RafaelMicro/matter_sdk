/*
 * Copyright (c) 2010-2014, 2016, GreenPeak Technologies
 * Copyright (c) 2017-2018, Qorvo Inc
 *
 * gpSched_runOn.h
 *   This file holds the internal defines, typedefs,... of the gpSched_runOn
 *
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 *
 */

#ifndef _GP_SCHED_RUNON_H_
#define _GP_SCHED_RUNON_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "global.h"
/*****************************************************************************
 *                    Precompiler checks
 *****************************************************************************/

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
typedef UInt32(*gpSched_RunOnCallback_t)(void*);

void gpSched_RunOnInit(void);
UInt32 gpSched_RunOn(gpSched_RunOnCallback_t externFunction, void* arg);
void gpSched_RunOnMainLoopReached(void);

#endif
