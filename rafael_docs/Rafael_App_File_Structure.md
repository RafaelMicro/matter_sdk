# Application File Structure
## Supported Applications
You can find example applications in `matter_sdk/examples/` folder. We support the following examples so far. 

- [light-switch-app](#1)
- [lighting-app](#2)
- [lock-app](#3)
- [smart-plug-app](#4)
- [thermostat](#5)
- [window-app](#6)

<span id = 1>light-switch-app</span>
- In `matter_sdk/examples/light-switch-app/rt582/src` folder, you can modify the following files to fit your application.

   |File Name                               |Description                     
   |:---------------------------------------|:----------------------------------------------
   |AppTask.cpp                             |Initial light switch application, identify function 
   |binding-handler.cpp                     |Handle unicast binding command, group binding command and light switch
   |init_light_switch_app_rt582Platform.cpp |Peripherals initialization

<span id = 2>lighting-app</span>
- In `matter_sdk/examples/lighting-app/rt582/src` folder, you can modify the following files to fit your application.

   |File Name                       |Description                     
   |:-------------------------------|:----------------------------------------------
   |AppTask.cpp                     |Initial light application, identify function 
   |LightingManager.cpp             |Handle On/Off, Level Control application
   |init_lighting_rt582Platform.cpp |Peripherals initialization

<span id = 3>lock-app</span>
- In `matter_sdk/examples/lock-app/rt582/src` folder, you can modify the following files to fit your application.

   |File Name                       |Description                     
   |:-------------------------------|:----------------------------------------------
   |AppTask.cpp                     |Initial lock application, identify function 
   |LockManager.cpp                 |Handle lock application
   |init_lock_app_rt582Platform.cpp |Peripherals initialization

<span id = 4>smart-plug-app</span>
- In `matter_sdk/examples/smart-plug-app/rt582/src` folder, you can modify the following files to fit your application.

   |File Name                             |Description                     
   |:-------------------------------------|:----------------------------------------------
   |AppTask.cpp                           |Initial smart plug application
   |SmartPlugManager.cpp                  |Smart plug application
   |init_smart-plug-app_rt582Platform.cpp |Peripherals initialization

<span id = 5>thermostat</span>
- In `matter_sdk/examples/thermostat/rt582/src` folder, you can modify the following files to fit your application.

   |File Name                         |Description                     
   |:---------------------------------|:----------------------------------------------
   |AppTask.cpp                       |Initial thermostat application
   |SensorManager.cpp                 |Simulate temperature sensor behavier
   |TemperatureManager.cpp            |Handle thermostat attribute change  
   |init_thermostat_rt582Platform.cpp |Peripherals initialization

<span id = 6>window-app</span>
- In `matter_sdk/examples/window-app/rt582/src` folder, you can modify the following files to fit your application.

   |File Name                       |Description                     
   |:-------------------------------|:----------------------------------------------
   |AppTask.cpp                     |Initial window application
   |WindowCovering.cpp              |Handle window covering behavier
   |WindowManager.cpp               |Window covering controller
   |init_window_rt582Platform.cpp   |Peripherals initialization