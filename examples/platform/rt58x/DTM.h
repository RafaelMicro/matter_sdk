#include "uart.h"
#include "mcu.h"
#include "hosal_dma.h"
#include "hosal_gpio.h"
#include "hosal_rf.h"
#include "hosal_slow_timer.h"
#include "hosal_sysctrl.h"
#include "hosal_timer.h"
#include "hosal_uart.h"
#include "flashctl.h"
#include "cli.h"
#include "log.h"
#include "util_string.h"
#include "lmac15p4.h"
#include "ble_common.h"
#include "uart_stdio.h"
#include "FactoryDataParser.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t wait_dtm(void);
void dtm_init(void);
#ifdef __cplusplus
}
#endif