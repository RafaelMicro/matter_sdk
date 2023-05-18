# Application Development
## Supported Applications
You can find example applications in `matter_sdk/examples/` folder. We support the following examples so far. 

- [light-switch-app](#1)
- [lighting-app](#2)
- [lock-app](#3)
- [smart-plug-app](#4)
- [thermostat](#5)
- [window-app](#6)

## Application Notes

- <span id = 1>**light-switch-app**</span>

   - In `main.cpp` file, we initialize RT58x's platforms, memory alloction, matter protocol stack and application.

   - In the `FunctionTimerEventHandler` callback function of `AppTask.cpp` file, you can implement Software Timer event by using `StarTimer()` and `CancelTimer()` function. You also can implement button event in the `ButtonEventHandler()` callback function. You can create `AppEvent` event and post to event queue in application layer

      - For example, we use matter system timer and create a software timer event using `AppEvent::kEventType_Timer` type and `FunctionTimerEventHandler` handler. Then, RTOS will call `FunctionTimerEventHandler` callback handler and do factory reset.

      ```cpp
      void AppTask::CancelTimer()
      {
          chip::DeviceLayer::SystemLayer().CancelTimer(TimerEventHandler, this);
          mFunctionTimerActive = false;
      }
      ```

      ```cpp
      void AppTask::StartTimer(uint32_t aTimeoutInMs)
      {
          CHIP_ERROR err;

          chip::DeviceLayer::SystemLayer().CancelTimer(TimerEventHandler, this);
          err = chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Milliseconds32(aTimeoutInMs), TimerEventHandler, this);
          SuccessOrExit(err);

          mFunctionTimerActive = true;
      exit:
          if (err != CHIP_NO_ERROR)
          {
              ChipLogError(NotSpecified, "StartTimer failed %s: ", chip::ErrorStr(err));
          }
      }
      ```
      
      ```cpp
      void AppTask::TimerEventHandler(chip::System::Layer * aLayer, void * aAppState)
      {
          AppEvent event;
          event.Type               = AppEvent::kEventType_Timer;
          event.TimerEvent.Context = aAppState;
          event.Handler            = FunctionTimerEventHandler;
          sAppTask.PostEvent(&event);
      }
      ```
      ```cpp
      void AppTask::FunctionTimerEventHandler(AppEvent * aEvent)
      {
          if (aEvent->Type != AppEvent::kEventType_Timer)
          {
              return;
          }

          // If we reached here, the button was held past FACTORY_RESET_TRIGGER_TIMEOUT,
          // initiate factory reset
          else if (sAppTask.mFunctionTimerActive && sAppTask.mFunction == kFunction_FactoryReset)
          {
              // Actually trigger Factory Reset
              sAppTask.mFunction = kFunction_NoneSelected;
              chip::Server::GetInstance().ScheduleFactoryReset();
          }
      }
      ```
      
      - You can create event to execute switch (On/Off/Toggle) function in the `binding-handler.cpp` file. 

      ```cpp
      void AppTask::SwitchActionEventHandler(AppEvent * aEvent)
      {
          if (aEvent->Type == AppEvent::kEventType_Button_ON)
          {
              ChipLogProgress(NotSpecified, "SwitchState: ON");
              gpio_pin_clear(21);
              BindingCommandData * data = chip::Platform::New<BindingCommandData>();
              data->commandId           = chip::app::Clusters::OnOff::Commands::On::Id;
              data->clusterId           = chip::app::Clusters::OnOff::Id;
              PlatformMgr().ScheduleWork(SwitchWorkerFunction, reinterpret_cast<intptr_t>(data));
          }
          else if (aEvent->Type == AppEvent::kEventType_Button_OFF)
          {
              ChipLogProgress(NotSpecified, "SwitchState: OFF");
              gpio_pin_clear(21);
              BindingCommandData * data = chip::Platform::New<BindingCommandData>();
              data->commandId           = chip::app::Clusters::OnOff::Commands::Off::Id;
              data->clusterId           = chip::app::Clusters::OnOff::Id;
              PlatformMgr().ScheduleWork(SwitchWorkerFunction, reinterpret_cast<intptr_t>(data));
          }
      }
      ```

   - In this case, we call the `init_light_switch_app_pin_mux()` function in the `init_light_switch_app_rt582Platform.cpp` file to set pin mux. You can modify your application code and initialize RT583's peripherals which you might need. And then you can also call the `init_light_switch_app_rt582Platform()` function to set TIMER to periodically update LED onoff status. RT583's peripheral SDK is in the matter_sdk/third_party/rafael/sdk/Driver/Peripheral folder.

      - Configurate GPIO20 for switch on and GPIO21 for switch off

      ```cpp
      static void init_light_switch_app_pin_mux(void)
      {
          gpio_cfg_output(20);
          gpio_cfg_output(21);
          gpio_pin_set(20);
          gpio_pin_set(21);
          return;
      }
      ```

      - Configurate TIMER2 for flashing LED

      ```cpp
      void init_light_switch_app_rt582Platform(void)
      {
          timer_config_mode_t cfg;

          init_light_switch_app_pin_mux();

          cfg.int_en = ENABLE;
          cfg.mode = TIMER_PERIODIC_MODE;
          cfg.prescale = TIMER_PRESCALE_32;

          Timer_Open(2, cfg, _timer_isr_handler);
          Timer_Int_Priority(2, 3);

          Timer_Start(2, 999);

          memset(flash_table, 0, sizeof(flash_table));    
      }
      ```

- <span id = 2>**lighting-app**</span>

   - In `main.cpp` file, we initialize RT58x's platforms, memory alloction, matter protocol stack and application.

   - `LightingManager.cpp` supports lighting control fucntions. For example
      
      |Function Name         | Description                 |
      |:---------------------|:----------------------------|
      |Init()                | Initialize light manager    |
      |SetCallbacks()        | Register callback function  |
      |GetLevel()            | Get light level             |
      |GetRgb()              | Get light RGB               |
      |InitiateAction()      | Control light               |
      |SetLevel()            | Set light level             |
      |SetColor()            | Set light Color             |
      |SetColorTemperature() | Set light Color Temperature |
      |Set()                 | Set On/Off                  |

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

      - You can register the callback function.
         ```cpp
         LightMgr().SetCallbacks(ActionInitiated, ActionCompleted);
         ```

   - In this case, we call the `init_lighting_pin_mux()` function in the `init_lighting_rt582Platform.cpp` file to set pin mux. You can modify your application code and initialize RT583's peripherals which you might need. And then you can also call the `init_lighting_app_rt582Platform()` function to control RGB LED using `PWMs` for dimmable light or set `TIMER` to periodically update LED onoff status. RT583's peripheral SDK is in the `matter_sdk/third_party/rafael/sdk/Driver/Peripheral` folder. 
      - Configurate GPIO21, GPIO22, GPIO23 for PWM2, PWM3, PWM4
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

      - Initialize PWM2, PWM3, PWM4 for LED level control and configurate TIMER2 for flashing LED

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

- <span id = 3>**lock-app**</span>

   - In `main.cpp` file, we initialize RT58x's platforms, memory alloction, matter protocol stack and application.

   - In the `ActionInitiated()` function of the `AppTask.cpp` file, we use GPIO22, GPIO23 and GPIO24 to simulate door locked or unlocked.
      - `ActionInitiated()` can be used to simulate door locked or unlocked by determining aAction value. When aAction is equal to `LockManager::LOCK_ACTION`, it means the door is locked and LED turn on the light.
      - For example
         ```cpp
         void AppTask::ActionInitiated(LockManager::Action_t aAction, int32_t aActor)
         {
            if (aAction == LockManager::UNLOCK_ACTION || aAction == LockManager::LOCK_ACTION)
            {
               bool locked = (aAction == LockManager::LOCK_ACTION);
               ChipLogProgress(NotSpecified, "%s Action has been initiated", (locked) ? "Lock" : "Unlock");
               if (!locked)
               {
                     gpio_pin_clear(22);
                     gpio_pin_clear(23);
                     gpio_pin_clear(24);
               }
               else
               {
                     gpio_pin_set(22);
                     gpio_pin_set(23);
                     gpio_pin_set(24);
               }
            }

            if (aActor == AppEvent::kEventType_Button)
            {
               sAppTask.mSyncClusterToButtonAction = true;
            }
         }
         ```

   - In `init_lock_app_rt582Platform.cpp` file, you can call `init_lock_app_pin_mux()` function to set GPIO pin mux. You can modify your application code and initialize RT583's peripherals which you might need. And then you can also call the `init_lock_app_rt582Platform()` function to set `TIMER` to periodically update LED onoff status. RT583's peripheral SDK is in the `matter_sdk/third_party/rafael/sdk/Driver/Peripheral` folder.
   
      - Configurate GPIO22, GPIO23, GPIO24 for simulating door lock or unlock.

         ```cpp
         static void init_lock_app_pin_mux(void)
         {
            pin_set_mode(22, MODE_GPIO);
            pin_set_mode(23, MODE_GPIO);
            pin_set_mode(24, MODE_GPIO);
            
            gpio_cfg_output(22);
            gpio_pin_set(22);

            gpio_cfg_output(23);
            gpio_pin_set(23);

            gpio_cfg_output(24);
            gpio_pin_set(24);    
            return;
         }
         ```
      - 

- <span id = 4>**smart-plug-app**</span>

   - In `main.cpp` file, we initialize RT58x's platforms, memory alloction, matter protocol stack and application.

   - In `AppTask.cpp` file, you can implement callback function `ActionInitiated()` and `ActionCompleted()` of smart plug device and register the callback function using `PlugMgr().SetCallbacks()`.

      - `ActionCompleted()` can be used to control smart plug behavier.

      - For example, in `ActionCompleted()` callback function, you can call `smart_plug_trigger()` function to control smart plug.

         ```cpp
         void AppTask::ActionCompleted(SmartPlugManager::Action_t aAction)
         {
            smart_plug_trigger(PlugMgr().IsTurnedOn());
            if (aAction == SmartPlugManager::ON_ACTION)
            {
               ChipLogProgress(NotSpecified, "Plug On Action has been completed");
            }
            else if (aAction == SmartPlugManager::OFF_ACTION)
            {
               ChipLogProgress(NotSpecified, "Plug Off Action has been completed");
            }
         }
         ```

   - In `init_smart-plug-app_rt582Platform.cpp` file, you can call `init_smart_plug_app_pin_mux()` function to set GPIO pin mux. You can modify your application code and initialize RT583's peripherals which you might need. And then you can also call the `init_smart_plug_app_rt582Platform()` function to set `TIMER` to periodically update LED onoff status. RT583's peripheral SDK is in the `matter_sdk/third_party/rafael/sdk/Driver/Peripheral` folder.

      - Configurate GPIO21
      
         ```cpp
         #define SMART_PLUG_TRIGGER_GPIO 21

         static void init_smart_plug_app_pin_mux(void)
         {
            pin_set_mode(SMART_PLUG_TRIGGER_GPIO, MODE_GPIO);

            gpio_cfg_output(SMART_PLUG_TRIGGER_GPIO);
            gpio_pin_set(SMART_PLUG_TRIGGER_GPIO);

            return;
         }
         ```

- <span id = 5>**thermostat**</span>

   - In `main.cpp` file, we initialize RT58x's platforms, memory alloction, matter protocol stack and application.

   - In `init_thermostat_rt582Platform.cpp` file, you can call `init_thermostat_pin_mux()` function to set GPIO pin mux. You can modify your application code and initialize RT583's peripherals which you might need. And then you can also call the `init_thermostat_rt582Platform()` function to set `TIMER` to periodically update LED onoff status. RT583's peripheral SDK is in the `matter_sdk/third_party/rafael/sdk/Driver/Peripheral` folder.
      
      - Configurate GPIO20, GPIO21
         ```cpp
         static void init_thermostat_pin_mux(void)
         {
            gpio_cfg_output(20);
            gpio_cfg_output(21);
            gpio_pin_set(20);
            gpio_pin_set(21);
            return;
         }
         ```

- <span id = 6>**window-app**</span>

   - In `main.cpp` file, we initialize RT58x's platforms, memory alloction, matter protocol stack and application.

   - In `init_window_rt582Platform.cpp` file, you can call `init_window_pin_mux()` function to set GPIO pin mux. You can modify your application code and initialize RT583's peripherals which you might need. And then you can also call the `init_window_app_rt582Platform()` function to set `TIMER` to periodically update LED onoff status. RT583's peripheral SDK is in the `matter_sdk/third_party/rafael/sdk/Driver/Peripheral` folder.