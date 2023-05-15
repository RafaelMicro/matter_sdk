# Application Development
## Supported Applications
You can find example applications in `matter_sdk/examples/` folder. We support the following examples so far. 

- light-switch-app
- [lighting-app](#2)
- lock-app
- smart-plug-app
- thermostat
- window-app

## Application Notes

<span id = 2>lighting-app</span>

- In `main.cpp` file, we initialize RT58x's platforms, memory alloction, matter protocol stack and application.

- `LightingManager.cpp` offers lighting control fucntions. For example
   
   - Init()
   - SetCallbacks()
   - GetLevel()
   - GetRgb()
   - InitiateAction()
   - SetLevel()
   - SetColor()
   - SetColorTemperature()
   - Set()

- In `AppTask.cpp` file, you can implement callback function `ActionInitiated()` and `ActionCompleted()` of lighting device and register the callback function using `LightMgr().SetCallbacks()`.

   - `ActionInitiated()` can be used to initiaze on/off or level value before light status changes.

   - `ActionCompleted()` can be used to control light behavier, on/off, levle, color...etc.

   - For exmaple, in `ActionCompleted()` callback function, you can use `PWM2`, `PWM3` and `PWM4` to turn light ON with level when `aActin` variable is equal to `LightingManager::ON_ACTION` status or turn light OFF when `aActin` variable is equal to `LightingManager::OFF_ACTION` status.

      ```cpp
      void AppTask::ActionCompleted(LightingManager::Action_t aAction)
      {
         // Placeholder for light action completed
         uint8_t current_level = 0;
         RgbColor_t RGB;
         if (aAction == LightingManager::ON_ACTION)
         {
            ChipLogProgress(NotSpecified, "Light On Action has been completed");

            current_level = LightMgr().GetLevel();
            RGB = LightMgr().GetRgb();
            rt582_led_level_ctl(2, RGB.b);
            rt582_led_level_ctl(3, RGB.r);
            rt582_led_level_ctl(4, RGB.g);
         }
         else if (aAction == LightingManager::OFF_ACTION)
         {
            ChipLogProgress(NotSpecified, "Light Off Action has been completed");
            rt582_led_level_ctl(2, 0);
            rt582_led_level_ctl(3, 0);
            rt582_led_level_ctl(4, 0); 
         }
      }
      ```

   - We register the callback function.
      ```cpp
      LightMgr().SetCallbacks(ActionInitiated, ActionCompleted);
      ```

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
