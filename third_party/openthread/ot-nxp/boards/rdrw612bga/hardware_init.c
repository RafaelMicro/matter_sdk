/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockLPR();
    CLOCK_InitT3RefClk(kCLOCK_T3MciIrc48m);
    CLOCK_EnableClock(kCLOCK_T3PllMci256mClk);
    CLOCK_EnableClock(kCLOCK_Otp);
    CLOCK_EnableClock(kCLOCK_Els);
    CLOCK_EnableClock(kCLOCK_ElsApb);
    RESET_PeripheralReset(kOTP_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kELS_APB_RST_SHIFT_RSTn);
    BOARD_InitSleepPinConfig();
    BOARD_InitAppConsole();

#ifdef OT_STACK_ENABLE_LOG
    BOARD_InitDebugConsole();
#endif
}
/*${function:end}*/
