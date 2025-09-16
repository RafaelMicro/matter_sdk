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

#include "init_device_environment.h"
#include "mcu.h"

static void init_lighting_pin_mux(void)
{
    gpio_cfg_output(20);
    hosal_gpio_pin_set(20);
    return;
}

void init_pwm(void)
{
    hosal_pwm_dev_t pwm_dev;

    pwm_dev.config.id = HOSAL_PWM_ID_1;
    pwm_dev.config.frequency = 16000;//16K
    pwm_dev.config.pin_out = 21;	
    pwm_dev.config.count_end_val = 3000;	
    hosal_pwm_init_fmt0(&pwm_dev);

    pwm_dev.config.id = HOSAL_PWM_ID_2;
    pwm_dev.config.pin_out = 22;	
    hosal_pwm_init_fmt0(&pwm_dev);

    pwm_dev.config.id = HOSAL_PWM_ID_3;
    pwm_dev.config.pin_out = 23;	
    hosal_pwm_init_fmt0(&pwm_dev);
}
uint8_t calc_duty(uint8_t level)
{
    uint8_t duty;
    duty = level * 100 / 254;
    if(level > 0 && duty == 0) {
        duty = 1;
    }
    /* Inverted for rt58x evk */
    duty = 100 - duty;
    return duty;
}
void pwm_set_color(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t duty_r,duty_g,duty_b;
    duty_r = calc_duty(r);
    duty_g = calc_duty(g);
    duty_b = calc_duty(b);
    hosal_pwm_fmt0_duty(HOSAL_PWM_ID_1, duty_b);
    hosal_pwm_fmt0_duty(HOSAL_PWM_ID_2, duty_r);
    hosal_pwm_fmt0_duty(HOSAL_PWM_ID_3, duty_g);
}

void init_lighting_app_rt58xPlatform(void)
{
    init_lighting_pin_mux();
    init_pwm();
}

#ifdef __cplusplus
}
#endif
