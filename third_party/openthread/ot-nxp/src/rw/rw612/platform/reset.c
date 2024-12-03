/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fwk_platform_ot.h"
#include <openthread/error.h>

#if (OT_NXP_INDEPENDENT_RST == 1)
otError otPlatResetOt(void)
{
    otError error = OT_ERROR_NONE;

    if (PLATFORM_ResetOt() < 0)
        error = OT_ERROR_FAILED;

    return error;
}
#endif
