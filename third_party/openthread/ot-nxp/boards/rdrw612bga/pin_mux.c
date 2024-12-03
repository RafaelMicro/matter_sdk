/*
 * Copyright 2021-2023 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_io_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    IO_MUX_SetPinMux(IO_MUX_FC3_USART_DATA);
    IO_MUX_SetPinMux(IO_MUX_QUAD_SPI_FLASH);

#ifdef OT_STACK_ENABLE_LOG
    /* USART0 is used to print OT logs */
    IO_MUX_SetPinMux(IO_MUX_FC0_USART_DATA);
#endif
}
void BOARD_InitENETPins(void)
{
    IO_MUX_SetPinMux(IO_MUX_ENET_CLK);
    IO_MUX_SetPinMux(IO_MUX_ENET_RX);
    IO_MUX_SetPinMux(IO_MUX_ENET_TX);
    IO_MUX_SetPinMux(IO_MUX_ENET_MDIO);
    IO_MUX_SetPinMux(IO_MUX_GPIO21);
    IO_MUX_SetPinMux(IO_MUX_GPIO55);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
