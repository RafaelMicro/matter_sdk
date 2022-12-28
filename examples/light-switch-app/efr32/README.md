# CHIP RT58x Light Switch Example
An example showing the use of CHIP on the Rafael Micro RT58x.

---
-   [CHIP RT58x Light Switch Example](#CHIP-RT58x-Light-Switch-Example)
    -   [Introduction](#Introduction)
    -   [Building](#Building)
    -   [Flashing the Application](#Flashing-the-Application)
    -   [Viewing Logging Output](#Viewing-Logging-Output)
    -   [Running the Complete Example](#Running-the-Complete-Example)
---
<a name="intro"></a>
## Introduction


The Rafael Light Switch Example demonstrates a matter’s light switch, which remotely control matter's dimmable light bulbs. You can follow this example as a reference for creating your application.

The example’s commission with the Rendez-vous exchanges security information over Bluetooth Low Energy (BLE) and is controlled by a Chip controller over the Openthread network (ex: Thread).

The UART console on RT58x shows a URL for a QR Code containing the needed commissioning information for the BLE connection and starting the Rendez-vous procedure.

For creating products, the lighting example shows production templates, which employ Rafael’s SDK platform to develop. By the way, Rafael also provides an RCP dangle solution for the matter’s border router.

<a name="building"></a>

## Building

<!-- -  ISP -->
<!-- -   Supported hardware: -->

*   Build the example application:

          cd ~/matter_sdk
         ./scripts/examples/gn_rt582_example.sh ./examples/light-switch-app/rt582 ./out/light-switch-app

-   To delete generated executable, libraries and object files use:

          $ cd ~/matter_sdk
          $ rm -rf ./out/

    OR use GN/Ninja directly

          $ cd ~/matter_sdk/examples/light-switch-app/RT582
          $ git submodule update --init
          $ source third_party/matter_sdk/scripts/activate.sh
          $ gn gen out/debug
          $ ninja -C out/debug

<a name="flashing"></a>

## Flashing the Application

<!-- -   On the command line:

          $ cd ~/connectedhomeip/examples/lighting-app/rt582
          $ python3 out/debug/chip-rt582-lighting-example.flash.py

-   Or with the Ozone debugger, just load the .out file. -->

<a name="view-logging"></a>

## Viewing Logging Output

The example application is built to use the UART for log output. For log output, you can use [Tera Term](https://ttssh2.osdn.jp/index.html.en) or other tools (ex: minicom) to show the log output.

<a name="running-complete-example"></a>

## Running the Complete Example

-   It is assumed here that you already have an OpenThread border router configured and running. If not see the following guide [Openthread_border_router](https://github.com/RafaelMicro/matter_sdk/blob/master/docs/guides/openthread_border_router_pi.md) for more information on how to setup a border router on a raspberryPi.
-   For this example to work, it is necessary to have a second rt58x device running the [lighting app example](https://github.com/RafaelMicro/matter_sdk/blob/master/examples/lighting-app/rt582/README.md) commissioned on the same openthread network

-   User interface : This QR Code is be scanned by the CHIP Tool app For the Rendez-vous procedure over BLE

        * The QR Code's URL can be found in the UART logs.
          <info  > [SVR] Copy/paste the below URL in a browser to see the QR Code:
          <info  > [SVR] https://project-chip.github.io/connectedhomeip/qrcode.html?data=...

    **LED D0** shows the states of the device and its connectivity. The following states are possible:

        -   _Short Flash On (50 ms on/950 ms off)_ ; The device is in the
            unprovisioned (unpaired) state and is waiting for a commissioning
            application to connect.

        -   _Rapid Even Flashing_ ; (100 ms on/100 ms off)_ &mdash; The device 
            is in the unprovisioned state and a commissioning application is 
            connected through Bluetooth LE.

        -   _Short Flash Off_ ; (950ms on/50ms off)_ &mdash; The device is fully
            provisioned, but does not yet have full Thread network or service
            connectivity.

        -   _Solid On_ ; The device is fully provisioned and has full Thread
            network and service connectivity.

    **LED D3** Simulates the switch The following states are possible:

        -   _Solid On_ ; switch is on
        -   _Off_ ; switch is off

    **Push Button SW2**

        -   _Pressed and hold for 6 s_ : Initiates the factory reset of the device.
            Releasing the button within the 6-second window cancels the factory reset
            procedure. **LEDs** blink in unison when the factory reset procedure is
            initiated.

    **Push Button SW3**

        -   Sends a Toggle command to bound light app

    **Matter shell**

    **_OnOff Cluster_**

        -  'switch onoff on'            : Sends unicast On command to bound device
        -  'switch onoff off'           : Sends unicast Off command to bound device
        -  'switch onoff toggle'        : Sends unicast Toggle command to bound device

        -  'switch groups onoff on'     : Sends On group command to bound group
        -  'switch groups onoff off'    : Sends On group command to bound group
        -  'switch groups onoff toggle' : Sends On group command to bound group

    **_Binding Cluster_**

        - 'switch binding unicast  <fabric index> <node id> <endpoint>' : Creates a unicast binding
        - 'switch binding group <fabric index> <group id>'              : Creates a group binding

*   You can provision and control the Chip device using the python controller,
    [CHIPTool](https://github.com/project-chip/connectedhomeip/blob/master/examples/chip-tool/README.md) standalone, Android or iOS app

    Here is an example with the CHIPTool for unicast commands only:

    ```
    chip-tool pairing ble-thread 1 hex:<operationalDataset> 20202021 3840

    chip-tool accesscontrol write acl '[{"fabricIndex": 1, "privilege": 5, "authMode": 2, "subjects": [<chip-tool-node-id>], "targets": null }{"fabricIndex": 1, "privilege": 3, "authMode": 2, "subjects": [1], "targets": null }]' <lighting-node-id> 0

    chip-tool binding write binding '[{"fabricIndex": 1, "node": <lighting-node-id>, "endpoint": 1, "cluster":6}]' 1 1
    ```

    Here is an example with the CHIPTool for groups commands only:

    ```
    chip-tool pairing ble-thread 1 hex:<operationalDataset> 20202021 3840

    chip-tool tests TestGroupDemoConfig --nodeId 1

    chip-tool tests TestGroupDemoConfig --nodeId <lighting-node-id>

    chip-tool binding write binding '[{"fabricIndex": 1, "group": 257}]' 1 1
    ```

    To run the example with unicast and groups commands, run the group
    configuration commands and replace the last one with binding this command

    ```
    chip-tool binding write binding '[{"fabricIndex": 1, "group": 257},{"fabricIndex": 1, "node": <lighting-node-id>, "endpoint": 1, "cluster":6} ]' 1 1
    ```

    To acquire the chip-tool node id, read the acl table right after
    commissioning

    ```
    ./connectedhomeip/out/chip-tool/chip-tool accesscontrol read acl <nodeid> 0
    ```