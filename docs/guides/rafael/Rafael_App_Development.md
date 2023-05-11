# Application Development
## Supported Applications
You can find example applications in `matter_sdk/examples/` folder. We support the following examples so far. 

- [light-switch-app](#1)
- [lighting-app](#2)
- [lock-app](#3)
- [smart-plug-app](#4)
- [thermostat](#5)
- [window-app](#6)

<span id = 1>light-switch-app</span>


<span id = 2>lighting-app</span>

- In this case, we call the `init_lighting_pin_mux()` function in the `init_lighting_rt582Platform.cpp` file to set pin mux. You can modify your application code and initialize RT583's peripherals which you might need. And then you can also call the `init_lighting_app_rt582Platform()` function to control RGB LED using `PWMs` for dimmable light or set `TIMER` to periodically update LED onoff status. RT583's peripheral SDK is in the `matter_sdk/third_party/rafael/sdk/Driver/Peripheral` folder. 
   - configurate GPIO21, GPIO22, GPIO23 for PWM2, PWM3, PWM4
      ```cpp
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
      ```

   - initialize PWM2, PWM3, PWM4 for LED level control and configurate TIMER2 for flashing light

      ```cpp
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
      ```

<span id = 3>lock-app</span>

<span id = 4>smart-plug-app</span>

<span id = 5>thermostat</span>

<span id = 6>window-app</span>
