# CHIP RT58x Lock Example
An example showing the use of CHIP on the Rafael Micro RT58x.

---
-   [CHIP RT58x Lock Example](#CHIP-RT58x-Lock-Example)
    -   [Introduction](#Introduction)
    -   [Building](#Building)
    -   [Flashing the Application](#Flashing-the-Application)
    -   [Viewing Logging Output](#Viewing-Logging-Output)
    -   [Running the Complete Example](#Running-the-Complete-Example)
---

<a name="intro"></a>

## Introduction

The Rafael Lighting Example demonstrates a matter's door lock, which uses a remote controller (ex: Chip controller) to control the state of these changes. You can follow this example as a reference for creating your application.

The example's commission with the Rendez-vous exchanges security information over Bluetooth Low Energy (BLE) and is controlled by a Chip controller over the Openthread network (ex: Thread).

The UART console on RT58x shows a URL for a QR Code containing the needed commissioning information for the BLE connection and starting the Rendez-vous procedure.

For creating products, the lighting example shows production templates, which employ Rafael's SDK platform to develop. By the way, Rafael also provides an RCP dangle solution for the matter's border router.

<a name="building"></a>

## Building

<!-- -  ISP -->
<!-- -   Supported hardware: -->

*   Build the example application:

          cd ~/matter_sdk
         ./scripts/examples/gn_rt582_example.sh ./examples/lighting-app/rt582 ./out/lighting-app

-   To delete generated executable, libraries and object files use:

          $ cd ~/matter_sdk
          $ rm -rf ./out/

    OR use GN/Ninja directly

          $ cd ~/matter_sdk/examples/lighting-app/RT582
          $ git submodule update --init
          $ source third_party/matter_sdk/scripts/activate.sh
          $ gn gen out/debug
          $ ninja -C out/debug

<!-- *   Build the example as Sleepy End Device (SED) -->
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

-   User interface : This QR Code is be scanned by the CHIP Tool app For the Rendez-vous procedure over BLE

        * The QR Code's URL can be found in the UART logs.
          <info  > [SVR] Copy/paste the below URL in a browser to see the QR Code:
          <info  > [SVR] https://project-chip.github.io/connectedhomeip/qrcode.html?data=...

    **LED D0** shows the states of the device and its connectivity. The following states are possible:

        -   _Short Flash On (50 ms on/950 ms off)_ ; The device is in the
            unprovisioned (unpaired) state and is waiting for a commissioning
            application to connect.

        -   _Rapid Even Flashing_ ; (100 ms on/100 ms off)_ &mdash; The device is in the
            unprovisioned state and a commissioning application is connected through
            Bluetooth LE.

        -   _Short Flash Off_ ; (950ms on/50ms off)_ &mdash; The device is fully
            provisioned, but does not yet have full Thread network or service
            connectivity.

        -   _Solid On_ ; The device is fully provisioned and has full Thread
            network and service connectivity.

    **LED D6** Simulates the Lock The following states are possible:

        -   _Solid On_ ; Bolt is unlocked
        -   _Blinking_ ; Bolt is moving to the desired state
        -   _Off_ ; Bolt is locked

    **Push Button SW2-6**

        -   _Pressed and hold for 6 s_ : Initiates the factory reset of the device.
            Releasing the button within the 6-second window cancels the factory reset
            procedure. **LEDs** blink in unison when the factory reset procedure is
            initiated.

-   You can provision and control the Chip device using the python controller, Chip tool standalone, Android or iOS app

    [CHIPTool](https://github.com/RafaelMicro/matter_sdk/blob/master/examples/chip-tool/README.md)

Here is some CHIPTool examples:

    Pairing with chip-tool:
    ```
    chip-tool pairing ble-thread 1 hex:<operationalDataset> 20202021 3840
    ```

    Set a user:
    ```
    ./out/chip-tool doorlock set-user OperationType UserIndex UserName UserUniqueId UserStatus UserType CredentialRule node-id/group-id
    ./out/chip-tool doorlock set-user 0 1 "mike" 5 1 0 0 1 1 --timedInteractionTimeoutMs 1000
    ```

    Set a credential:
    ```
    ./out/chip-tool doorlock set-credential OperationType Credential CredentialData UserIndex UserStatus UserType node-id/group-id
    ./out/chip-tool doorlock set-credential 0 '{ "credentialType": 1, "credentialIndex": 1 }' "123456" 1 null null 1 1 --timedInteractionTimeoutMs 1000
    ```

    Changing a credential:
    ```
    ./out/chip-tool doorlock set-credential OperationType Credential CredentialData UserIndex UserStatus UserType node-id/group-id
    ./out/chip-tool doorlock set-credential 2 '{ "credentialType": 1, "credentialIndex": 1 }' "123457" 1 null null 1 1 --timedInteractionTimeoutMs 1000
    ```

    Get a user:
    ```
    ./out/chip-tool doorlock get-user UserIndex node-id/group-id
    ./out/chip-tool doorlock get-user 1 1 1
    ```

    Unlock door:
    ```
    ./out/chip-tool doorlock unlock-door node-id/group-id
    ./out/chip-tool doorlock unlock-door 1 1
    ```

    Lock door:
    ```
    ./out/chip-tool doorlock lock-door node-id/group-id
    ./out/chip-tool doorlock lock-door 1 1
    ```