#include "init_rt58x_platform.h"
#include "DTM.h"
#include "DTM_RF.h"
#include "hosal_lpm.h"
#include "CHIPProjectConfig.h"

/*wait dtm*/
#define DTM_WAIT_TIMEOUT_MS 3000
extern hosal_uart_dev_t uart0_dev;
uint8_t uart_buf[64];
uint8_t dtm_start_command[3] ={'D','T','M'}; 
uint32_t offset = 0;

static TaskHandle_t sRFTestTaskHandle;
static StackType_t rfTestStack[1024 / sizeof(StackType_t)];
static StaticTask_t rfTestTaskStruct;

uint8_t receive_dtm_start_command(void)
{
    uint32_t search_len = (offset < sizeof(uart_buf)) ? offset : sizeof(uart_buf);
    if(search_len < sizeof(dtm_start_command))
        return 0;
    for(uint32_t i = 0; i <= search_len - sizeof(dtm_start_command); i++)
    {
        if(memcmp(&uart_buf[i], dtm_start_command, sizeof(dtm_start_command)) == 0)
        {
            return 1;
        }
    }
    return 0;
}

uint8_t wait_dtm(void)
{
    uint8_t ret = 0;
    uint8_t temp_buf[16];
    uint32_t timer_id = 1;
    uint32_t cur_tick = 0;
    uint32_t len = 0;
    hosal_slow_timer_config_t cfg;
    hosal_slow_timer_tick_config_t tick_cfg;

    cfg.counting_mode = HOSAL_SLOW_TIMER_DOWN_COUNTING;
    cfg.int_enable = HOSAL_SLOW_TIMER_INT_ENABLE;
    cfg.mode = HOSAL_SLOW_TIMER_PERIODIC_MODE;
    cfg.one_shot_mode = HOSAL_SLOW_TIMER_ONE_SHOT_DISABLE;
    cfg.prescale = HOSAL_SLOW_TIMER_PRESCALE_32;
    cfg.user_prescale = 0;
    cfg.repeat_delay = 0;

    tick_cfg.timeload_ticks = DTM_WAIT_TIMEOUT_MS + 50;
    tick_cfg.timeout_ticks = 0;
#if defined(CONFIG_RT584H) ||  defined(CONFIG_RT584HA4) || defined(CONFIG_RT584L)
    NVIC_DisableIRQ((IRQn_Type)(SlowTimer1_IRQn));
    NVIC_SetPriority((IRQn_Type)(SlowTimer1_IRQn), 1);
#else
    NVIC_DisableIRQ((IRQn_Type)(Timer4_IRQn));
    NVIC_SetPriority((IRQn_Type)(Timer4_IRQn), 1);
#endif
    hosal_slow_timer_current_get(timer_id, &cur_tick);
    hosal_slow_timer_init(timer_id, cfg, (void*)nullptr);
    hosal_slow_timer_stop(timer_id);
#if defined(CONFIG_RT584H) ||  defined(CONFIG_RT584HA4) || defined(CONFIG_RT584L)
    NVIC_EnableIRQ((IRQn_Type)(SlowTimer1_IRQn));
#else
    NVIC_EnableIRQ((IRQn_Type)(Timer4_IRQn));
#endif
    hosal_slow_timer_start(timer_id, tick_cfg);
    
    while(1)
    {
        len = hosal_uart_receive(&uart0_dev, temp_buf, 16);
        if(len > 0)
        {
            uint32_t available = sizeof(uart_buf) - offset;
            if(available > 0)
            {
                uint32_t copy_len = (len < available) ? len : available;
                memcpy(&uart_buf[offset], temp_buf, copy_len);
                offset += copy_len;
            }
            if(receive_dtm_start_command())
            {
                ret = 1;
                hosal_uart_finalize(&uart0_dev);
                hosal_slow_timer_stop(timer_id);
                break;
            }
        }
        /* Check if timer expired*/
        hosal_slow_timer_current_get(timer_id, &cur_tick);
        if(cur_tick < 50)
        {
            hosal_slow_timer_stop(timer_id);
            ret = 0;
            break;
        }
        delay_ms(10);
    }
    return ret;
}

void dtm_init(void)
{
    uart_stdio_init();
    printf("Enter DTM Mode\r\n");
    lpm_low_power_mask(LOW_POWER_MASK_BIT_RESERVED31);
    rf_init();
    cli_init();
    vTaskStartScheduler();
}
static int
_cli_cmd_channel(int argc, char **argv, cb_shell_out_t log_out, void *pExtra)
{
    if(argc < 2)
    {
        printf("invalid param\r\n");
        return 0;
    }
    uint8_t channel = utility_strtol(argv[1], 0);
    if(channel < 11 || channel > 26)
    {
        printf("invalid channel\r\n");
        return 0;
    }
    set_channel((lmac154_channel_t)(channel-11));
    printf("Set channel to %d\r\n", channel);

    return 0;
}
static int
_cli_cmd_cw(int argc, char **argv, cb_shell_out_t log_out, void *pExtra)
{
    if(argc < 2)
    {
        printf("invalid param\r\n");
        return 0;
    }
    uint8_t enable = utility_strtol(argv[1], 0);
    if(enable)
    {
        tx_single_tone(1);
        printf("Start TX CW\r\n");
    }
    else
    {
        tx_single_tone(0);
        printf("Stop TX CW\r\n");
    }
    return 0;
}
static int
_cli_cmd_tx(int argc, char **argv, cb_shell_out_t log_out, void *pExtra)
{
    if(argc < 2)
    {
        printf("invalid param\r\n");
        return 0;
    }
    uint32_t count = utility_strtol(argv[1], 0);
    printf("TX %d packets\r\n", count);
    tx_pkt_cnt(count);
    printf("TX done\r\n");
    return 0;
}
static int
_cli_cmd_rx(int argc, char **argv, cb_shell_out_t log_out, void *pExtra)
{
    if(argc < 2)
    {
        printf("invalid param\r\n");
        return 0;
    }
    uint32_t enable = utility_strtol(argv[1], 0);
    if(enable)
    {
        printf("Set RX on\r\n");
        set_auto_state(1);
    }
    else
    {
        printf("Set RX off\r\n");
        set_auto_state(0);
    }
    return 0;
}
const sh_cmd_t  g_cli_cmd_channel STATIC_CLI_CMD_ATTRIBUTE =
{
    .pCmd_name      = "channel",
    .pDescription   = "Set TX/RX channel\n"
    "  usage: channel [channel]\n",
    .cmd_exec       = _cli_cmd_channel,
};

const sh_cmd_t  g_cli_cmd_cw STATIC_CLI_CMD_ATTRIBUTE =
{
    .pCmd_name      = "cw",
    .pDescription   = "TX continous wave\n"
    "  usage: cw [enable]\n",
    .cmd_exec       = _cli_cmd_cw,
};

const sh_cmd_t  g_cli_cmd_tx STATIC_CLI_CMD_ATTRIBUTE =
{
    .pCmd_name      = "tx",
    .pDescription   = "TX packet\n"
    "  usage: tx [packet sounts]\n",
    .cmd_exec       = _cli_cmd_tx,
};
const sh_cmd_t  g_cli_cmd_rx STATIC_CLI_CMD_ATTRIBUTE =
{
    .pCmd_name      = "rx",
    .pDescription   = "Set RX on/off\n"
    "  usage: rx [enable]\n",
    .cmd_exec       = _cli_cmd_rx,
};