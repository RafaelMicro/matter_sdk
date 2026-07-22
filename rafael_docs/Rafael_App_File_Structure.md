# Application File Structure
## Supported Applications
You can find example applications in `matter_sdk/examples/` folder. We support the following examples so far. 

- [contact-sensor-app](#1)
- [humidity-sensor-app](#2)
- [light-switch-app](#3)
- [lighting-app](#4)
- [lighting-zigbee](#5)
- [lock-app](#6)
- [occupancy-sensor-app](#7)
- [smart-plug-app](#8)
- [smoke-alarm-app](#9)
- [temperature-measurement-app](#10)
- [thermostat](#11)
- [water-valve-app](#12)
- [window-app](#13)

<span id = 1>contact-sensor-app</span>
- In `matter_sdk/examples/contact-sensor-app/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize contact sensor application, identity function
   |ContactManager.cpp         |Manage contact sensor state (open/closed) and toggle boolean state
   |PowerManager.cpp           |Simulate battery percentage remaining and handle power status updates
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

<span id = 2>humidity-sensor-app</span>
- In `matter_sdk/examples/humidity-sensor-app/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize humidity sensor application, identity function
   |HumidityManager.cpp        |Manage humidity measurement readings with simulated periodic updates
   |PowerManager.cpp           |Simulate battery percentage remaining and handle power status updates
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

<span id = 3>light-switch-app</span>
- In `matter_sdk/examples/light-switch-app/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize light switch application, identity function
   |LightSwitchMgr.cpp         |Manage light switch endpoints and state configuration
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

<span id = 4>lighting-app</span>
- In `matter_sdk/examples/lighting-app/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize light application, identity function
   |LightingManager.cpp        |Manage lighting state including color control, brightness, and RGB color space
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

<span id = 5>lighting-zigbee</span>
- This is a dual-protocol application that supports both Matter and Zigbee. The protocol in use is selected at runtime via `MultiApp.cpp`. Matter-side source files are located in `rt58x/src`, and Zigbee-side source files are located in `rt58x/zigbee_src`.

- In `matter_sdk/examples/lighting-zigbee/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize Matter lighting application, identity function
   |LightingManager.cpp        |Manage RGB lighting with color control and brightness settings
   |MultiApp.cpp               |Handle protocol switching between Matter and Zigbee, factory reset logic
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

- In `matter_sdk/examples/lighting-zigbee/rt58x/zigbee_src` folder, you can modify the following files to fit your Zigbee application.

   |File Name                    |Description                     
   |:----------------------------|:----------------------------------------------
   |zcl_construction.c           |Define Zigbee ZCL cluster attributes and endpoint descriptor construction
   |zigbee_api.c                 |Zigbee application layer API, stack initialization, OTA, identify, and task management
   |zigbee_zcl_msg_handler.c     |Handle incoming Zigbee ZCL messages for on/off, level control, color control, and scenes

<span id = 6>lock-app</span>
- In `matter_sdk/examples/lock-app/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize lock application, identity function
   |BoltLockManager.cpp        |Manage bolt lock state transitions, timers, and lock/unlock actions
   |PowerManager.cpp           |Simulate battery percentage remaining and handle power status updates
   |UartTask.cpp               |Manage UART communication with GPIO interrupt handling for wake-up signals
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

<span id = 7>occupancy-sensor-app</span>
- In `matter_sdk/examples/occupancy-sensor-app/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize occupancy sensor application, identity function
   |OccupancyManager.cpp       |Manage occupancy detection state with hold time limits
   |PowerManager.cpp           |Simulate battery percentage remaining and handle power status updates
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

<span id = 8>smart-plug-app</span>
- In `matter_sdk/examples/smart-plug-app/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize smart plug application, identity function
   |SmartPlugManager.cpp       |Manage plug on/off state and action callbacks
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

<span id = 9>smoke-alarm-app</span>
- In `matter_sdk/examples/smoke-alarm-app/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize smoke alarm application, identity function
   |SmokeManager.cpp           |Manage smoke alarm state, self-testing, and alarm priority order
   |PowerManager.cpp           |Simulate battery percentage remaining and handle power status updates
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

<span id = 10>temperature-measurement-app</span>
- In `matter_sdk/examples/temperature-measurement-app/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize temperature measurement application, identity function
   |TemperatureManager.cpp     |Manage temperature sensor readings with simulated periodic updates
   |PowerManager.cpp           |Simulate battery percentage remaining and handle power status updates
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

<span id = 11>thermostat</span>
- In `matter_sdk/examples/thermostat/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize thermostat application, identity function
   |SensorManager.cpp          |Simulate temperature sensor readings using timer-based periodic updates
   |TemperatureManager.cpp     |Manage thermostat setpoints, current temperature, and system mode state
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

<span id = 12>water-valve-app</span>
- In `matter_sdk/examples/water-valve-app/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize water valve application, identity function
   |WaterValveManager.cpp      |Manage valve open/close operations with level percent control
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization

<span id = 13>window-app</span>
- In `matter_sdk/examples/window-app/rt58x/src` folder, you can modify the following files to fit your application.

   |File Name                  |Description                     
   |:--------------------------|:----------------------------------------------
   |AppTask.cpp                |Initialize window application, identity function
   |WindowControl.cpp          |Control window position by calculating and driving lift percentage
   |WindowManager.cpp          |Manage window position state, level values, and movement callbacks
   |ZclCallbacks.cpp           |Handle ZCL attribute change callbacks for application clusters
   |init_device_environment.cpp|Peripherals initialization
