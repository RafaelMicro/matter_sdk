/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "AppConfig.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <assert.h>
#include <string.h>

#include "init_lighting_rt582Platform.h"
#include "cm3_mcu.h"

#define PWM_COUNTER_MODE   PWM_COUNTER_MODE_UP         /*UP is up counter Mode ;UD is up and down Mode*/
#define PWM_SEQ_ORDER      PWM_SEQ_ORDER_R             /*Rseq or Tseq Selection or both*/
#define PWM_ELEMENT_NUM    1                           /*genrator Pulse number*/
#define PWM_REPEAT_NUM     0                           /*Repeat Pulse number*/
#define PWM_DLY_NUM        0                           /*Pulse delay number*/
#define PWM_CNT_END_VAL    10000                       /*Count end Value*/
#define PWM_CLK_DIV        PWM_CLK_DIV_4               /*PWM Input Clock Div*/
#define PWM_TRIG_SRC       PWM_TRIGGER_SRC_SELF        /*PWM Trigger Source by self or PWM1~PWM4*/
#define PWM_SEQ_MODE       PWM_SEQ_MODE_CONTINUOUS     /*Continuous and Noncontinuous mode*/
#define PWM_PLAY_CNT       0                           /*0:is infinite*/
#define PWM_SEQ_NUM        PWM_SEQ_NUM_1               /*use rdma single simple or two simple*/
#define PWM_DMA_SML_FMT    PWM_DMA_SMP_FMT_1           /*Pwm DMA Simple Format 0 or 1*/


typedef struct {
    uint32_t onTimeMs;
    uint32_t offTimeMs;
    uint32_t nowTimeMs;
    uint32_t state  : 1;
    uint32_t used   : 1;
    uint32_t        : 30;
} flash_info_t;

static flash_info_t flash_table[32];

static pwm_seq_para_head_t pwm_para_config[5];
static uint32_t pwm_rdma0_addr_temp;
static uint32_t pwm_rdma1_addr_temp;

static uint16_t pwm_count_end_val = PWM_CNT_END_VAL;
static pwm_clk_div_t pwm_clk_div = PWM_CLK_DIV;
static pwm_seq_order_t pwm_seq_order = PWM_SEQ_ORDER;
static pwm_trigger_src_t pwm_trigger_src = PWM_TRIG_SRC;
static uint16_t pwm_play_cnt = PWM_PLAY_CNT;
static pwm_seq_num_t pwm_seq_num = PWM_SEQ_NUM;
static pwm_seq_mode_t pwm_seq_mode = PWM_SEQ_MODE;
static pwm_counter_mode_t pwm_counter_mode = PWM_COUNTER_MODE;
static pwm_dma_smp_fmt_t pwm_dma_smp_fmt = PWM_DMA_SML_FMT;
static uint8_t pwm_element_arr = PWM_ELEMENT_NUM;
static uint8_t pwm_rep_arr = PWM_REPEAT_NUM;
static uint8_t pwm_dly_arr = PWM_DLY_NUM;

static void init_lighting_pin_mux(void)
{
    gpio_cfg_output(21);
    gpio_pin_set(21);

    gpio_cfg_output(22);
    gpio_pin_set(22);

    gpio_cfg_output(23);
    gpio_pin_set(23);

    pin_set_mode(21, MODE_PWM2);
    pin_set_mode(22, MODE_PWM3);
    pin_set_mode(23, MODE_PWM4);

    gpio_cfg_output(20);
    gpio_pin_set(20);
    return;
}

static void _timer_isr_handler(uint32_t timer_id)
{
    int i;
    for(i=0;i<32;i++)
    {
        if(!flash_table[i].used)
            continue;

        if(flash_table[i].state == 0)
        {
            flash_table[i].nowTimeMs++;
            if(flash_table[i].nowTimeMs > flash_table[i].offTimeMs)
            {
                flash_table[i].state = 1;
                gpio_pin_clear(i);
                flash_table[i].nowTimeMs = 0;
            }          
        }
        else
        {
            flash_table[i].nowTimeMs++;
            if(flash_table[i].nowTimeMs > flash_table[i].onTimeMs)
            {
                flash_table[i].state = 0;
                gpio_pin_set(i);
                flash_table[i].nowTimeMs = 0;
            }            
        }
    }
    //printf("2");
}

void init_rt582_led_level_ctl(uint32_t id)
{
    pwm_seq_para_t *pwm_seq = NULL;
    pwm_seq_para_head_t *p_pwm_para_config = &pwm_para_config[id];

    p_pwm_para_config->pwm_id            = (pwm_id_t)id;
    p_pwm_para_config->pwm_play_cnt      = pwm_play_cnt; //0 means continuous
    p_pwm_para_config->pwm_seq_order     = pwm_seq_order;
    p_pwm_para_config->pwm_seq_num       = pwm_seq_num;
    p_pwm_para_config->pwm_seq_mode      = pwm_seq_mode;
    p_pwm_para_config->pwm_triggered_src = pwm_trigger_src;
    p_pwm_para_config->pwm_clk_div       = pwm_clk_div;
    p_pwm_para_config->pwm_counter_mode  = pwm_counter_mode;
    p_pwm_para_config->pwm_dma_smp_fmt   = pwm_dma_smp_fmt;
    p_pwm_para_config->pwm_count_end_val = 0x00;

    pwm_seq = &(p_pwm_para_config->pwm_seq0);
    pwm_seq->pwm_rdma_addr  = (uint32_t)&pwm_rdma0_addr_temp;
    pwm_seq->pwm_element_num    = pwm_element_arr;
    pwm_seq->pwm_repeat_num     = pwm_rep_arr;
    pwm_seq->pwm_delay_num      = pwm_dly_arr;

    Pwm_Init(&pwm_para_config[id]);
}

void rt582_led_level_ctl(uint32_t id, uint8_t current_lv)
{
    uint32_t dutycycle ;
    dutycycle = 4000 * current_lv / 255;

    pwm_seq_para_t *pwm_seq;
    uint32_t *rdma_addr;

    pwm_seq = &(pwm_para_config[id].pwm_seq0);
    rdma_addr = (uint32_t *)pwm_seq->pwm_rdma_addr;
    *(rdma_addr)  = PWM_FILL_SAMPLE_DATA_MODE1(0, 4000 - dutycycle, 4000);

    Pwm_Start(&pwm_para_config[id]);
}

void init_rt582_led_flash(uint32_t pin, uint32_t onTimeMs, uint32_t offTimeMs)
{
    if(onTimeMs == 0 && offTimeMs == 0)
    {
        flash_table[pin].state = 0;
        flash_table[pin].used = 0;
    }
    else
    {
        flash_table[pin].onTimeMs = onTimeMs;
        flash_table[pin].offTimeMs = offTimeMs;
        flash_table[pin].state = 0;
        flash_table[pin].used = 1;
    }
    gpio_pin_set(pin);
}

void init_lighting_app_rt582Platform(void)
{
    timer_config_mode_t cfg;

    init_lighting_pin_mux();

    init_rt582_led_level_ctl(2);
    init_rt582_led_level_ctl(3);
    init_rt582_led_level_ctl(4);
    rt582_led_level_ctl(2, 0);
    rt582_led_level_ctl(3, 0);
    rt582_led_level_ctl(4, 0);

    cfg.int_en = ENABLE;
    cfg.mode = TIMER_PERIODIC_MODE;
    cfg.prescale = TIMER_PRESCALE_32;

    Timer_Open(2, cfg, _timer_isr_handler);
    Timer_Int_Priority(2, 3);

    Timer_Start(2, 999);

    memset(flash_table, 0, sizeof(flash_table));    
}

#ifdef __cplusplus
}
#endif
